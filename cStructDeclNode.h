#pragma once
//***************************************
// cStructDeclNode.h
//
// Defines parse logic for structs.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

class cStructDeclNode : public cDeclNode
{
    public:

        cStructDeclNode(cDeclsNode* decls, cSymbol* s)
        {
            AddChild(decls);
            AddChild(s);
        }

    virtual string NodeType() { return string("struct_decl"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    virtual bool IsStruct() override { return true; }
    virtual bool IsType() override { return true; }
    virtual cDeclNode* GetType() override {
        return this;
    }
    virtual cDeclNode* GetDecl() {
        return this;
    }
};
