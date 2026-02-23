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
    cSymbol* sym = dynamic_cast<cSymbol*>(node->GetDecl());

    if (!decl)
    {
        SemanticParseError("Undeclared symbol used");
        return;
    }

    if (decl->IsFunc())
    {
        PostParseError("Symbol " + sym->GetName() + " is a function, not a variable", node->GetLine());
        return;
    }

    /*
    // array checks
    if (!decl->IsArray() && node->GetIndexCount() > 0)
    {
        PostParseError(node->GetName() + " is not an array", node->GetLine());
    }

    for (auto idx : node->GetIndices())
    {
        if (idx->GetType()->GetTypeName() != "int")
        {
            PostParseError("Index of " + node->GetName() + " is not an int", node->GetLine());
        }
    }
    */
}
