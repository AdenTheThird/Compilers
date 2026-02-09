// ***************************************
//
// Class for a binary expression node.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu
//

#include "cOpNode.h"
#include "cExprNode.h"
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
};
