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
    void Visit(cProgramNode* node);
    void Visit(cBlockNode* node);
    void Visit(cDeclsNode* node);
    void Visit(cStmtsNode* node);
    void Visit(cAssignNode* node);
    void Visit(cVarDeclNode* node);
    void Visit(cParamsNode* node);
    void Visit(cFuncDeclNode* node);
    void Visit(cVarExprNode* node);
    void Visit(cFuncExprNode* node);

protected:
    int m_currentOffset;
    int m_highWaterMark;
};
