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

    virtual string NodeType() { return string("args"); }
    virtual void Visit(cVisitor *visitor) {visitor->Visit(this); }

    protected:
    std::vector<cAstNode*> m_children;

};
