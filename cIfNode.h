#pragma once
//***********************************
// cIfNode.h
//
// Defines parse logic for if expressions.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cExprNode.h"
#include "cStmtNode.h"
#include "cStmtsNode.h"

class cIfNode : public cStmtNode
{
    public:

        cIfNode(cExprNode* expr, cStmtsNode* if_b) : cStmtNode()
        {
            AddChild(expr);
            AddChild(if_b);
        }
        
        cIfNode(cExprNode* expr, cStmtsNode* if_b, cStmtsNode* else_b) : cStmtNode()
        {
            AddChild(expr);
            AddChild(if_b);
            AddChild(else_b);
        }

        virtual string NodeType() { return string("if"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};


