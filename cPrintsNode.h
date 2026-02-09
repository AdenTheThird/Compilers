#pragma once
//******************************
// cPrintsNode.h
//
// Defines parse logic for print literal statements.
// 
// Aurhor: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cStmtNode.h"

class cPrintsNode : public cStmtNode
{
    public:
        cPrintsNode(std::string input) : cStmtNode()
        {
            m_input = input;
        }

        virtual string AttributesToString()
        {
            return " value=\"" + m_input + "\"";
        }

        virtual string NodeType() { return string("prints"); } 
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
    protected:
        std::string m_input;
};
