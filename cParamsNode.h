#pragma once
//***********************************
// cParamsNode.h
//
// Parse logic for decls
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"

class cParamsNode : public cAstNode
{

    public:

        cParamsNode(cDeclNode* decl)
        {
            AddChild(decl);
            m_children.push_back(decl);
        }

        void Insert(cDeclNode* decl)
        {
            AddChild(decl);
            m_children.push_back(decl);
        }

        int Count()
        {
            return NumChildren();
        }

        const std::vector<cAstNode*>& GetChildren() const
        {
            return m_children;
        }


    string AttributesToString()
    {
        if (GetSize() == 0) return "";
        return " size =\"" + std::to_string(GetSize()) + "\"";
    }

    virtual string NodeType() { return string("args"); }
    virtual void Visit(cVisitor *visitor) {visitor->Visit(this); }

    int GetSize()
    {
        return m_size;
    } 
    void SetSize(int size)
    {
        m_size = size;
    }

    std::vector<cAstNode*> GetChildren()
    {
        return m_children;
    }

    protected:
    std::vector<cAstNode*> m_children;
    int m_size;

};
