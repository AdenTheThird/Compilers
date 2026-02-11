#pragma once
//********************************************************
// cSymbol.h - Define a class for symbols
//
// Author: Philip Howard
//
#include <string>

using std::string;

#include "cAstNode.h"

class cSymbol : public cAstNode
{
    public:
        // Construct a symbol given its name
        cSymbol(string name) : cAstNode ()
        {
            m_id = ++nextId;
            m_name = name;
            m_instance = nullptr;
            //std::cout << "\n" << m_id << " " << m_name << "\n";
        }

        // Return name of symbol
        string GetName() { return m_name; }

        virtual string AttributesToString()
        {
            string result = " id=\"" + std::to_string(m_id) + "\" name=\"" + m_name + "\"";
            return result;
        }

        cDeclNode* GetDecl()
        {
            return m_instance;
        }

        void SetDecl(cDeclNode* instance)
        {
            m_instance = instance;
        }

        void SetInstance(cDeclNode* instance)
        {
            m_instance = instance;
        }

        static int GetNextSymbolId()
        {
            return nextId;
        }

        virtual string NodeType() {return string("sym"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        static long long nextId;    // keeps track of unique symbol I
        long long m_id;             // Unique ID for this symbol
        cDeclNode* m_instance;
        string m_name;              // Symbol name
};
