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

        cParamListNode() : cParamsNode(nullptr) {}
        
        void Insert(cDeclNode* param) 
        {
            m_params.push_back(param);
        }

        const std::vector<cDeclNode*>& GetParams() const { return m_params; }

        virtual string NodeType() { return string("params"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

};
