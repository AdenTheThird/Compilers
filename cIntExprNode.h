#pragma once
//**************************************
// cIntExprNode.h
//
// Defines an AST node for an integer constant (literals).
//
// Inherits from cExprNode so that integer constants can be used anywhere 
// expressions are used.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cExprNode.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cIntExprNode : public cExprNode
{
    public:
        // param is the value of the integer constant
        cIntExprNode(int value) : cExprNode()
        {
            m_value = value;
        }

        virtual string AttributesToString() 
        {
            return " value=\"" + std::to_string(m_value) + "\"";
        }
        virtual string NodeType() { return string("int"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }
        
        cDeclNode *GetType() 
        {
            if (m_value >= 0 && m_value <= 127)
            {
                cSymbol* s = g_symbolTable.Find("char");
                return s->GetDecl();
            }
            else
            {
                cSymbol* s = g_symbolTable.Find("int");
                return s->GetDecl();
            }
        }
                
    protected:
        int m_value;        // value of integer constant (literal)
};
