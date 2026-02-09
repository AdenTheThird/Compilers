#pragma once
//***********************************
// cWhileNode.h
//
// Defines parse logic for while loops.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu
//
// my head hurts

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"

class cWhileNode : public cStmtNode
{
    public:

    cWhileNode(cExprNode* expr, cStmtNode* stmt) : cStmtNode() 
    {
        AddChild(expr);
        AddChild(stmt);
    }

    virtual string NodeType() { return string("while"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); } 
};

