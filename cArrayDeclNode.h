#pragma once
//******************************
// cArrayDeclNode.h
//
// Defines parse logic for arrays.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu
//

#include "cAstNode.h"
#include "cDeclsNode.h"
#include "cSymbol.h"

class cArrayDeclNode : public cDeclNode
{
    public:

        cArrayDeclNode(cSymbol* type, int size, cSymbol* name)
        {
            if (g_symbolTable.FindLocal(name->GetName()))
            {
                SemanticParseError("Symbol " + name->GetName() + " already defined in current scope");
            }
            else
            {
                g_symbolTable.Insert(name);
                name->SetDecl(this);
            }
            m_name = name;
            m_type = type->GetName();
            m_size = std::to_string(size);
            AddChild(type);
            AddChild(name);

        }

        virtual string AttributesToString()
        {
            return " count=\"" + m_size + "\"";
        }

    virtual string NodeType() { return string("array_decl"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    string GetArrayType() override { return m_type; }

    virtual bool IsArray() override { return true; }
    virtual bool IsType() override { return true; }
    virtual cDeclNode* GetType() {
        return this;
    }
    string GetTypeName() override { return m_name->GetName(); }

    protected: 
    std::string m_size;
    cSymbol* m_name;
    std::string m_type;
};
