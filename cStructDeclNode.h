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
            if (g_symbolTable.FindLocal(s->GetName()))
            {
                SemanticParseError("Symbol " + s->GetName() + " already defined in current scope");
            }
            else
            {
                g_symbolTable.Insert(s);
                s->SetDecl(this);
            }
            m_type = s;
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

    string GetTypeName() override { return m_type->GetName(); }
    protected:
        cSymbol* m_type;
};
