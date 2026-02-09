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
            m_isType = false;
        }

        // Return name of symbol
        string GetName() { return m_name; }

        virtual string AttributesToString()
        {
            string result = " id=\"" + std::to_string(m_id) + "\" name=\"" + m_name + "\"";
            return result;
        }

        bool isType()
        {
            return m_isType;
        }

        void setType(bool type)
        {
            m_isType = type;
        }

        void SetId(int id)
        {
            m_id = id;
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
        bool m_isType;
        string m_name;              // Symbol name
};
