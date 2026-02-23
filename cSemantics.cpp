#include "cSemantics.h"

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
/*
    if (node->isFieldAccess && !decl->IsStruct())
    {
        PostParseError(node->GetVarName() + " is not a struct", node->GetLine());
        return;
    }
    */
}
