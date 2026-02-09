#pragma once
//**************************************
// cReturnNode.h
//
// Defines a node for returning a value.
// 
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cStmtNode.h"

class cReturnNode : public cStmtNode
{
    public:
        cReturnNode(cExprNode* expr)
        {
            AddChild(expr);
        }

        virtual string NodeType() { return string("return"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
