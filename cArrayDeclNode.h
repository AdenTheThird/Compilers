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
            m_baseType = type;
            m_type = type->GetName();
            m_size = size;
            AddChild(type);
            AddChild(name);

        }

        virtual string AttributesToString()
        {
            return " count=\"" + std::to_string(m_size) + "\"";
        }

    virtual string NodeType() { return string("array_decl"); }
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

    string GetArrayType() override { return m_type; }

    cSymbol* GetBaseType()
    {
        return m_baseType;
    }

    virtual bool IsArray() override { return true; }
    virtual bool IsType() override { return true; }
    int GetCount()
    {
        return m_size;
    }

    virtual cDeclNode* GetType() {
        return this;
    }
    string GetTypeName() override { return m_name->GetName(); }

    protected: 
    int m_size;
    cSymbol* m_name;
    cSymbol* m_baseType;
    std::string m_type;
};
