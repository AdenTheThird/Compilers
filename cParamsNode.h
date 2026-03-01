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

        cParamsNode(cAstNode* decl, std::string type)
        {
            m_num_children++;
            m_type = type;
            AddChild(decl);
            m_children.push_back(decl);
        }

        void Insert(cAstNode* decl)
        {
            m_num_children++;
            AddChild(decl);
            m_children.push_back(decl);
        }

        int Count() { return m_num_children;
        }

        const std::vector<cAstNode*>& GetChildren() const
        {
            return m_children;
        }


    string AttributesToString()
    {
        if (GetSize() == 0) return "";
        return " size =\"" + std::to_string(m_size) + "\"";
    }

    virtual string NodeType() { return string(m_type); }
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
    std::string m_type;
    std::vector<cAstNode*> m_children;
    int m_num_children = 0;
    int m_size;

};
