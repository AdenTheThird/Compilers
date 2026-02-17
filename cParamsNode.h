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
        }

        void Insert(cDeclNode* decl)
        {
            AddChild(decl);
        }

        int Count()
        {
            return NumChildren();
        }
    virtual string NodeType() { return string("args"); }
    virtual void Visit(cVisitor *visitor) {visitor->Visit(this); }

};
