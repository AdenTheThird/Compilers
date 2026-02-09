#pragma once
//************************************
// cFuncExprNode.h
//
// Defines AST parsing for function calls
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu
//


#include "cAstNode.h"

class cFuncExprNode : public cAstNode
{
public:
    cFuncExprNode(cSymbol* name, cParamsNode* params)
    {
        AddChild(name);
        AddChild(params);
    }

    cFuncExprNode() {} 

    virtual string NodeType() { return "funcCall"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
};

