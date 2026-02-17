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

class cFuncExprNode : public cExprNode
{
public:
    cFuncExprNode(cSymbol* name, cParamsNode* params)
    {
        if (!g_symbolTable.Find(name->GetName()))
        {
            SemanticParseError("Symbol " + name->GetName() + " not defined");
        }
        AddChild(name);
        AddChild(params);
    }

    cFuncExprNode() {} 

    virtual string NodeType() { return "funcCall"; }
    virtual void Visit(cVisitor* visitor) { visitor->Visit(this); }
    virtual cDeclNode* GetDecl()
    {
        cSymbol* sym = dynamic_cast<cSymbol*>(GetChild(0));
        if (!sym) return nullptr;

        cDeclNode* decl = sym->GetDecl();
        if (!decl || !decl->IsFunc()) return nullptr;

        cFuncDeclNode* func = static_cast<cFuncDeclNode*>(decl);
        return func->GetType();
    }

    virtual cDeclNode* GetType() override
    {
        cDeclNode* decl = GetDecl();
        return decl ? decl->GetType() : nullptr;
    }
        

};

