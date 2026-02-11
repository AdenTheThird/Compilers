#pragma once
//**********************************
// cFuncDeclNode.h
//
// Defines parse logic for function declarations.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cParamsNode.h"

class cFuncDeclNode : public cDeclNode
{
    public:
        cFuncDeclNode(cSymbol* type, cSymbol* name, cParamsNode* params)
        {
            AddChild(type);
            AddChild(name);
            AddChild(params);
        }

        cFuncDeclNode(cSymbol* type, cSymbol* name, cParamsNode* params,
                cStmtsNode* stmts)
        {
            AddChild(type);
            AddChild(name);
            AddChild(params);
            AddChild(stmts);
        }
        
        cFuncDeclNode(cSymbol* type, cSymbol* name, cParamsNode* params,
                cDeclsNode* decls, cStmtsNode* stmts)
        {
            AddChild(type);
            AddChild(name);
            AddChild(params);
            AddChild(decls);
            AddChild(stmts);
        }

        virtual string NodeType() { return string("func"); }
        virtual bool IsFunc() override { return true; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual cDeclNode* GetType() override {
            return this;
        }
        virtual cDeclNode* GetDecl() {
            return this;
        }
};
