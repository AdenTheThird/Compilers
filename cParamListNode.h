#pragma once
// *******************************
// cParamListNode.h
//
// Parse logic for parameter lists
//
// Author: Aden Ratliff
// aden.ratliff@Oit.edu
// 

#include "cAstNode.h"
#include "cParamsNode.h"

class cParamListNode : public cParamsNode
{
    std::vector<cDeclNode*> m_params;

    public:

        cParamListNode() : cParamsNode(nullptr)
        {
        }

        
        void Insert(cDeclNode* param) 
        {
            m_params.push_back(param);
        }

        const std::vector<cDeclNode*>& GetParams() const { return m_params; }

        string AttributesToString()
        {
            if (GetSize() == 0) return "";
            return " size =\"" + std::to_string(GetSize()) +  "\"";
        }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        void SetSize(int size)
        {
            m_size = size;
        }

        int GetSize()
        {
            return m_size;
        }

    protected:
        int m_size;
};
