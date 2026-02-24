#pragma once
//**************************************
// cStmtsNode.h
//
// Defines AST node that represents a list of statements
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cStmtNode.h"

class cStmtsNode : public cAstNode
{
    public:
        // param is first stmt in the list of statements
        cStmtsNode(cStmtNode *stmt) : cAstNode()
        {
            AddChild(stmt);
            m_children++;
            m_stmts.push_back(stmt);
        }

        // Add a statement to the list
        void Insert(cStmtNode *stmt)
        {
            AddChild(stmt);
            m_children++;
            m_stmts.push_back(stmt);
        }

        virtual string NodeType() { return string("stmts"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        int GetNumChildren()
        {
            return m_children;
        }

        std::vector<cStmtNode*> GetChildren()
        {
            return m_stmts;
        }

        void SetSize(int size)
        {
            m_size = size;
        }

        int GetSize()
        {
            return m_size;
        }

    protected:
        int m_size = 0;
        int m_children = 0;
        std::vector<cStmtNode*> m_stmts;
};
