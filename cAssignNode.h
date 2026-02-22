#pragma once
//***************************************
// cAssignNode.h
//
// Defines parse logic for variable assignments.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cStmtNode.h"
#include "cExprNode.h"
#include "cVarExprNode.h"

class cAssignNode : public cStmtNode
{
    public:

    cAssignNode(cVarExprNode* var, cExprNode* expr) : cStmtNode()
    {
        AddChild(var);
        AddChild(expr);
    }

    cVarExprNode* GetLhs() { return dynamic_cast<cVarExprNode*>(GetChild(0)); }
    cExprNode*    GetRhs() { return dynamic_cast<cExprNode*>(GetChild(1)); }

    virtual string NodeType() { return string("assign"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
