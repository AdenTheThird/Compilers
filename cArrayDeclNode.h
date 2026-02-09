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

    protected: 
    std::string m_size;
};
