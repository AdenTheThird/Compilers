#include "cSemantics.h"

void cSemantics::Visit(cProgramNode* node)
{
    //std::cout << "Visiting program node\n";
    Visit(node->GetBlock());

    int blockSize = m_highWaterMark;

    if(blockSize > 1)
    {
        blockSize = ((blockSize + 3) / 4) * 4;
    }

    node->SetSize(blockSize);
}
void cSemantics::Visit(cBlockNode* node)
{
    int savedHighWater = m_highWaterMark;
    int startOffset = m_currentOffset;

    m_highWaterMark = m_currentOffset;

    Visit(node->GetDecls());
    Visit(node->GetStmts());

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
    int size = node->GetType()->GetSize();

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
    int savedOffset = m_currentOffset;
    int savedHighWater = m_highWaterMark;

    m_currentOffset = 0;
    m_highWaterMark = 0;

    if (node->GetParams())
    {
        Visit(node->GetParams());
        int paramOffset = -8;
        int paramSize = 0;

        for (auto param : node->GetParams()->GetChildren())
        {
            cVarDeclNode* var = dynamic_cast<cVarDeclNode*>(param);
            if (!var) continue;

            int size = var->GetType()->GetSize();

            paramOffset -= size;
            paramSize += size;

            var->SetVarSize(size);
            var->SetOffset(paramOffset);
        }
        //node->GetParams()->SetSize(paramSize);
    }

    if (node->GetDecls())
    {
        int declOffset = m_currentOffset;
        m_currentOffset = 0;
        Visit(node->GetDecls());
        m_currentOffset = declOffset;
    }

    if (node->GetStmts())
    {
        Visit(node->GetStmts());
    }

    node->SetSize(savedHighWater - node->GetParams()->GetSize()); // Look at this later

    m_currentOffset = savedOffset;
    m_highWaterMark = savedHighWater;
}

void cSemantics::Visit(cAssignNode* node)
{
    //std::cout << "Visiting assign node\n";
    cVarExprNode* lhs = node->GetLhs();
    cExprNode* rhs = node->GetRhs();

    if (!lhs || !rhs)
    {
        std::cerr << "Internal error: null expression in assignment." << std::endl;
        return;
    }

    cDeclNode* lhsType = lhs->GetType();
    cDeclNode* rhsType = rhs->GetType();

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

void cSemantics::Visit(cFuncExprNode* node) 
{
    //std::cout << "Visiting func expr node\n";
    cDeclNode* decl = node->GetDecl();
    cFuncDeclNode* funcDecl = dynamic_cast<cFuncDeclNode*>(decl);
    if (!funcDecl) return;

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

}

