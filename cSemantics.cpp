#include "cSemantics.h"

void cSemantics::Visit(cProgramNode* node)
{
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
    int oldOffset = m_currentOffset;
    m_currentOffset = 0;
    m_highWaterMark = 0;

    Visit(node->GetDecls());

    node->SetSize(m_highWaterMark);
    m_currentOffset = oldOffset;
}

void cSemantics::Visit(cDeclsNode* node)
{
    for (auto decl : node->GetChildren())
    {
        decl->Visit(this);
    }

    node->SetSize(m_highWaterMark);

}
void cSemantics::Visit(cVarDeclNode* node)
{
    int size = node->GetType()->GetSize();
    node->SetVarSize(size);

    node->SetOffset(m_currentOffset);

    m_currentOffset += size;
    if (m_currentOffset > m_highWaterMark)
    {
        m_highWaterMark = m_currentOffset;
    }
}
void cSemantics::Visit(cAssignNode* node)
{
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
    cDeclNode* decl = node->GetDecl();
    cFuncDeclNode* funcDecl = dynamic_cast<cFuncDeclNode*>(decl);
    if (!funcDecl) return;

    int exprCount = node->ExprCount();
    if (exprCount > 0) exprCount--;

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

