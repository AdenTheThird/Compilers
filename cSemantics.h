#pragma once

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"
#include "cAssignNode.h"
#include <iostream>

class cSemantics : public cVisitor
{
public:
    void Visit(cAssignNode* node);
};
