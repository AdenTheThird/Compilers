#pragma once
//**********************************
// cVarDeclNode.h
//
// Defines a variable declaration node statement.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cDeclNode.h"

class cVarDeclNode : public cDeclNode
{
    public:
        cVarDeclNode(cSymbol* s1, cSymbol* s2)
        {
            AddChild(s1);
            AddChild(s2);
        }

        virtual string NodeType() { return string("var_decl"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
};
