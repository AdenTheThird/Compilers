#pragma once
//**************************************
// cDeclsNode.h
//
// Defines a class to represent a list of declarations.
//
// Author: Phil Howard 
// phil.howard@oit.edu
//

#include "cAstNode.h"
#include "cDeclNode.h"

class cDeclsNode : public cAstNode
{
    public:
        // param is the first decl in this decls
        cDeclsNode(cDeclNode *decl) : cAstNode()
        {
            AddChild(decl);
            m_children++;
            m_decls.push_back(decl);
        }

        // Add a decl to the list
        void Insert(cDeclNode *decl)
        {
            AddChild(decl);
            m_children++;
            m_decls.push_back(decl);
        }

        string AttributesToString()
        {
            if (GetSize() == 0) return "";  // no size → no attributes
            return " size=\"" + std::to_string(GetSize()) + "\"";
        }

        virtual string NodeType() { return string("decls"); }
        virtual void Visit(cVisitor *visitor) { visitor->Visit(this); }

        int GetNumChildren()
        {
            return m_children;
        }

        std::vector<cDeclNode*> GetChildren()
        {
            return m_decls;
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
        std::vector<cDeclNode*> m_decls;

};
