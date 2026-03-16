#include "cSemantics.h"

void cSemantics::Visit(cProgramNode* node)
{

    if(node->GetBlock() && node->GetBlock()->GetDecls())
    {
        for (auto decl : node->GetBlock()->GetDecls()->GetChildren())
        {
            if(auto func = dynamic_cast<cFuncDeclNode*>(decl))
            {
                Visit(func);
            }
            else
            {
                decl->Visit(this);
            }
        }
    }

    EmitString(".function main\n");
    EmitString("main:\n");


    int blockSize = m_highWaterMark;
    EmitString(" ADJSP " + std::to_string(blockSize) + "\n");

    if(blockSize > 1)
    {
        blockSize = ((blockSize + 3) / 4) * 4;
    }

    node->SetSize(blockSize);

    if(node->GetBlock()->GetStmts())
    {
        for(auto stmt : node->GetBlock()->GetStmts()->GetChildren())
        {
            if(!stmt) continue;
            if(dynamic_cast<cFuncDeclNode*>(stmt)) continue;
            stmt->Visit(this);
        }
    }

    EmitString("PUSH 0\n");
    EmitString("RETURNV\n");
}

void cSemantics::Visit(cPrintNode* node)
{
    node->GetChild(0)->Visit(this);
    EmitString(" CALL @print\n");
}

void cSemantics::Visit(cIfNode* node)
{
    std::string elseLabel = GenerateLabel();
    std::string endLabel = GenerateLabel();

    node->GetChild(0)->Visit(this);

    if(node->NumChildren() > 2)
    {
        EmitString(" JUMPE @" + elseLabel + "\n");

        node->GetChild(1)->Visit(this);

        EmitString(" JUMP @" + endLabel + "\n");

        EmitString(elseLabel + ":\n");

        node->GetChild(2)->Visit(this);
    }
    else
    {
        EmitString(" JUMPE @" + endLabel + "\n");
        node->GetChild(1)->Visit(this);
    }

    EmitString(endLabel + ":\n");

}

void cSemantics::Visit(cBinaryExprNode* node)
{
    node->GetLeft()->Visit(this);
    node->GetRight()->Visit(this);

    string op = node->GetOp();
    if (op == "+") EmitString(" PLUS\n"); 
    if (op == "-") EmitString(" MINUS\n"); 
    if (op == "*") EmitString(" TIMES\n"); 
    if (op == "/") EmitString(" DIVIDE\n"); 
    if (op == "%") EmitString(" MOD\n"); 
    if (op == "==") EmitString(" EQ\n");
    if (op == "!=") EmitString(" NE\n");
}

void cSemantics::Visit(cBlockNode* node)
{
    int savedHighWater = m_highWaterMark;
    int startOffset = m_currentOffset;

    m_highWaterMark = m_currentOffset;

    if(node->GetDecls())
    {
        Visit(node->GetDecls());
    }
    
    if(node->GetStmts())
    {
        Visit(node->GetStmts());
    }

    int blockHighWater = m_highWaterMark;

    node->SetSize(blockHighWater - startOffset);

    m_currentOffset = startOffset;
    m_highWaterMark = std::max(savedHighWater, blockHighWater);

}

void cSemantics::Visit(cDeclsNode* node)
{
    //std::cout << "Visiting decls node\n";

    int startOffset = m_currentOffset;

    for (auto decl : node->GetChildren())
    {
        decl->Visit(this);
    }

    int declsSize = m_currentOffset - startOffset;

    node->SetSize(declsSize);
}

void cSemantics::Visit(cParamsNode* node)
{

    int startOffset = m_currentOffset;

    for (auto decl : node->GetChildren())
    {
        decl->Visit(this);
    }
    
    int paramsSize = m_currentOffset - startOffset;

    node->SetSize(paramsSize);

}

void cSemantics::Visit(cStmtsNode* node)
{

    //std::cout << "Visiting stmts node\n";
    for (auto decl : node->GetChildren())
    {
        decl->Visit(this);
    }

    node->SetSize(0);
}

void cSemantics::Visit(cVarDeclNode* node)
{
    //std::cout << "Visiting var decl node\n";

    int size;
    cDeclNode* type = node->GetType();
    if (type->IsArray())
    {
        cArrayDeclNode* arrayNode = dynamic_cast<cArrayDeclNode*>(type);
        if(arrayNode->GetBaseType()->GetName() != "char")
        {
            size = arrayNode->GetCount() * 4;
        }
        else
        {
            size = arrayNode->GetCount();
        }   

    }
    else
    {
        size = node->GetType()->GetSize();
    }

    if(size > 1)
    {
        m_currentOffset = ((m_currentOffset + 3) / 4) * 4;
    }

    node->SetVarSize(size);

    node->SetOffset(m_currentOffset);

    m_currentOffset += size;
    if (m_currentOffset > m_highWaterMark)
    {
        m_highWaterMark = m_currentOffset;
    }
}

void cSemantics::Visit(cFuncDeclNode* node)
{
    if (!node->HasBody())
    {
        return;
    }
    int savedOffset = m_currentOffset;
    int savedHighWater = m_highWaterMark;

    m_currentOffset = 0;
    m_highWaterMark = 0;

    if (node->GetParams() != nullptr)
    {
        Visit(node->GetParams());
        int paramOffset = -8;
        int paramTotal = 0;

        for (auto param : node->GetParams()->GetChildren())
        {
            int paramSize = 0;
            cVarDeclNode* var = dynamic_cast<cVarDeclNode*>(param);
            if (!var) continue;

            int size = var->GetType()->GetSize();

            paramOffset -= size;
            paramSize += size;

            if(abs(paramOffset) > 1)
            {
                int savedOffset = paramOffset;
                paramOffset = abs(paramOffset);
                paramOffset  = ((paramOffset + 3) / 4) * 4;
                if (savedOffset < 0)
                {
                    paramOffset = paramOffset - (2 * paramOffset);
                }
            }

            var->SetVarSize(paramSize);
            var->SetOffset(paramOffset);
            if(paramSize % 4 != 0)
            {
                paramTotal  = paramTotal + ((paramSize + 3) / 4) * 4;
            }
        }

        if(paramTotal > 0)
        {
            node->GetParams()->SetSize(paramTotal);
        }
    }

    if (node->GetDecls())
    {
        int declOffset = m_currentOffset;
        m_currentOffset = 0;
        Visit(node->GetDecls());
        m_currentOffset = declOffset;
    }

    std::string name = node->GetName();
    EmitString(".function " + name + "\n");
    EmitString(name + ":\n");

    int funcSize = node->GetSize();
    if(funcSize > 0)
    {
        EmitString(" ADJSP " + std::to_string(funcSize) + "\n");
    }

    if (node->GetStmts())
    {
        Visit(node->GetStmts());
    }

    EmitString("RETURNV\n");

    if(node->GetDecls())
    {
        node->SetSize(node->GetDecls()->GetSize());
    }


    m_currentOffset = savedOffset;
    m_highWaterMark = savedHighWater;
}

void cSemantics::Visit(cAssignNode* node)
{
    //std::cout << "Visiting assign node\n";
    cVarExprNode* lhs = node->GetLhs();
    cExprNode* rhs = node->GetRhs();

    rhs->Visit(this);

    EmitString(" POPVAR ");
    EmitInt(lhs->GetDecl()->GetOffset());
    EmitString("\n");

    if (!lhs || !rhs)
    {
        std::cerr << "Internal error: null expression in assignment." << std::endl;
        return;
    }

    cDeclNode* lhsType = lhs->GetType();
    cDeclNode* rhsType = rhs->GetType();

    if(lhsType->IsArray())
    {
        cArrayDeclNode* lhsArray = dynamic_cast<cArrayDeclNode*>(lhsType);
        lhsType = lhsArray->GetBaseType()->GetDecl();
    }

    if(rhsType->IsArray())
    {
        cArrayDeclNode* rhsArray = dynamic_cast<cArrayDeclNode*>(rhsType);
        rhsType = rhsArray->GetBaseType()->GetDecl();
    }
    if (!lhsType->IsCompatibleWith(rhsType))
    {
        PostParseError("Cannot assign " + rhsType->GetTypeName() + " to "  + lhsType->GetTypeName(), lhs->GetLine());
    }
}
void cSemantics::Visit(cVarExprNode* node)
{
    //std::cout << "Visiting var expr node\n";
    cDeclNode* decl = node->GetDecl();

    cDeclNode* typeDecl = decl->GetType();
    if(!decl->IsArray())
    {
        EmitString( " PUSHVAR ");
        EmitInt(decl->GetOffset());
        EmitString("\n");
    }
    else
    {

        cArrayDeclNode* arrayDecl = dynamic_cast<cArrayDeclNode*>(decl->GetType());
        EmitString(" PUSHVAR ");
        EmitInt(decl->GetOffset());
        EmitString("\n");

        int elementSize = (arrayDecl->GetBaseType()->GetName() == "char") ? 1 : 4;

        for (auto idxNode : node->indices)
        {
            cExprNode* idxExpr = dynamic_cast<cExprNode*>(idxNode);
            if(!idxExpr) continue;

            idxExpr->Visit(this);

            if (elementSize != 1)
            {
                EmitString(" PUSH");
                EmitInt(elementSize);
                EmitString("\n");
                EmitString(" TIMES\n");
            }

            EmitString(" PLUS\n");
        }
    }


    if(typeDecl->IsArray())
    {
        cArrayDeclNode* declArray = dynamic_cast<cArrayDeclNode*>(typeDecl);
        if(declArray->GetBaseType()->GetName() == "char")
        {
            node->SetRowSize(1);
        }
        else
        {
            node->SetRowSize(4);
        }
    }


    if (decl->IsFunc())
    {
        PostParseError("Symbol " + node->GetVarName() + " is a function, not a variable", node->GetLine());
        return;
    }

    if (node->isArrayAccess && !typeDecl->IsArray())
    {
        PostParseError(node->GetVarName() + " is not an array", node->GetLine());
        return;
    }

    for (auto idx : node->indices)
    {
        cDeclNode* idxType = idx->GetType();
        if (!idxType->IsInt())
        {
            PostParseError("Index of " + node->GetVarName() + " is not an int", node->GetLine());
        }
    }
}

void cSemantics::Visit(cIntExprNode* node)
{
    EmitString(" PUSH");
    EmitInt(node->GetValue());
    EmitString("\n");
}

void cSemantics::Visit(cFuncExprNode* node) 
{
    //std::cout << "Visiting func expr node\n";
    cDeclNode* decl = node->GetDecl();
    cFuncDeclNode* funcDecl = dynamic_cast<cFuncDeclNode*>(decl);
    if (!funcDecl) return;

    if (!funcDecl->GetParams() || !node->GetParams())
    {
        EmitString("CALL @" + funcDecl->GetName() + "\n");
        return;
    }
    string name = funcDecl->GetName();
    node->GetParams()->SetSize(funcDecl->GetParams()->GetSize());
    int exprCount = node->ExprCount();
    //if (exprCount > 0) exprCount--;

    if (funcDecl->DeclCount() != exprCount) 
    {
        PostParseError(
            node->GetName() + " called with wrong number of arguments", 
            node->GetLine()
        );
        return; 
    }

    if(!funcDecl->HasBody())
    {
        PostParseError("Function " + node->GetName() + " is not fully defined", node->GetLine());
    }

    if (!funcDecl->GetParams() || !node->GetParams()) return;

    auto formalParams = funcDecl->GetParams()->GetChildren();
    auto callArgs = node->GetParams()->GetChildren();

    for (size_t i = 0; i < formalParams.size(); ++i) 
    {
        cDeclNode* formalDecl = dynamic_cast<cDeclNode*>(formalParams[i]);
        if (!formalDecl) continue;

        cDeclNode* argDecl = nullptr;
        if (auto expr = dynamic_cast<cExprNode*>(callArgs[i + 1])) 
            argDecl = expr->GetType();

        if (!argDecl) continue;


        if (!formalDecl->IsCompatibleWith(argDecl)) 
        {
            PostParseError(
                node->GetName() + " called with incompatible argument",
                node->GetLine()
            );
        }
    }

    auto callArgs2 = node->GetParams()->GetChildren();
    for (auto exprNode : callArgs2)
    {
        auto expr = dynamic_cast<cExprNode*>(exprNode);
        if(expr) expr->Visit(this);
    }

    EmitString("CALL @" + name + "\n");
}

