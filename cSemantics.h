#pragma once

#include "astnodes.h"
#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"
#include "cAssignNode.h"
#include "cFuncExprNode.h"
#include <iostream>

class cSemantics : public cVisitor
{
public:
    void Visit(cAssignNode* node);
    void Visit(cVarExprNode* node);
    void Visit(cFuncExprNode* node);

};
