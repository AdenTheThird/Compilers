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

extern bool g_forceIntRhs;

class cBinaryExprNode : public cExprNode
{
    public:
        cBinaryExprNode(cExprNode* expr1, cOpNode* op, cExprNode* expr2) : cExprNode() 
    {
        AddChild(expr1);
        AddChild(op);
        AddChild(expr2);
        m_forceRhs = false;
    }

        cBinaryExprNode(cExprNode* expr1, cOpNode* op, cExprNode* expr2, bool forceRhs) : cExprNode() 
    {
        AddChild(expr1);
        AddChild(op);
        AddChild(expr2);
        m_forceRhs = forceRhs;
    }
    virtual string NodeType() { return string("expr"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

cDeclNode* GetDecl()
{
    cExprNode* leftExpr  = dynamic_cast<cExprNode*>(GetChild(0));
    cExprNode* rightExpr = dynamic_cast<cExprNode*>(GetChild(2));

    cDeclNode* leftType  = leftExpr->GetType();
    cDeclNode* rightType = rightExpr->GetType();

    // assign ranks to types: char=1, int=2, long=3, float=4, double=5
    auto rank = [](cDeclNode* t) {
        std::string n = t->GetTypeName();
        if (n == "char")   return 1;
        if (n == "int")    return 2;
        if (n == "long")   return 3;
        if (n == "float")  return 4;
        if (n == "double") return 5;
        return 0;
    };

    int leftRank  = rank(leftType);
    int rightRank = rank(rightType);

    // promotion: pick the higher rank
    int maxRank = std::max(leftRank, rightRank);

    // return the decl corresponding to the max rank
    if (maxRank == 1) return g_symbolTable.Find("char")->GetDecl();
    if (maxRank == 2) return g_symbolTable.Find("int")->GetDecl();
    if (maxRank == 3) return g_symbolTable.Find("long")->GetDecl();
    if (maxRank == 4) return g_symbolTable.Find("float")->GetDecl();
    if (maxRank == 5) return g_symbolTable.Find("double")->GetDecl();

    return leftType;  // fallback
}
    virtual cDeclNode* GetType() override
    {
        cDeclNode* decl = GetDecl();
        return decl ? decl->GetType() : nullptr;
    }

    protected:
    bool m_forceRhs;
};
