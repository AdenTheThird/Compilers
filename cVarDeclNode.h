#pragma once
//**********************************
// cVarDeclNode.h
//
// Defines a variable declaration node statement.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cDeclNode.h"

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol* s1, cSymbol* s2)
        {
            if (g_symbolTable.FindLocal(s2->GetName()))
            {
                SemanticParseError("Symbol " + s2->GetName() + " already defined in current scope");
            }
            m_type = s1;
            s2->SetDecl(this);
            g_symbolTable.Insert(s2);
            AddChild(s1);
            AddChild(s2);
        }

        string AttributesToString()
        {
            if (GetVarSize() == 0 && GetOffset() == 0) return "";
            return " size=\"" + std::to_string(GetVarSize()) +
                   "\" offset=\"" + std::to_string(GetOffset()) + "\"";
        }

        virtual string NodeType() { return "var_decl"; }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        virtual bool IsVar() override { return true; }
        virtual cDeclNode* GetType() override {
            cSymbol* typeSim = dynamic_cast<cSymbol*>(GetChild(0));
            if(!typeSim)
            {
                return nullptr;
            }
            return typeSim->GetDecl();
        }

        virtual cDeclNode* GetDecl() 
        {
            return this;
        }
        string GetTypeName() override { return m_type->GetName(); }

    protected:
        cSymbol* m_type;
};
