// ***************************************
//
// Class for a binary expression node.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu
//

#include "cOpNode.h"
#include "cExprNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "cAstNode.h"

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode* expr1, cOpNode* op, cExprNode* expr2) : cExprNode() 
    {
        AddChild(expr1);
        AddChild(op);
        AddChild(expr2);
    }

    virtual string NodeType() { return string("expr"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    virtual cDeclNode* GetDecl()
    {
        cExprNode* leftExpr = dynamic_cast<cExprNode*>(GetChild(0));
        cExprNode* rightExpr = dynamic_cast<cExprNode*>(GetChild(2));

        cDeclNode* leftType = leftExpr->GetType();
        cDeclNode* rightType = rightExpr->GetType();


        if (leftType->IsFloat() || rightType->IsFloat())
        {
            if (leftType->IsFloat()) return leftType;
            if (rightType->IsFloat()) return rightType;
        }

        if (leftType->IsInt() || rightType->IsInt())
        {
            if (leftType->IsInt()) return leftType;
            if (rightType->IsInt()) return rightType;
        }

        return g_symbolTable.Find("char")->GetDecl()->GetType();
    }

    virtual cDeclNode* GetType() override
    {
        cDeclNode* decl = GetDecl();
        return decl ? decl->GetType() : nullptr;
    }
};
