#pragma once
//****************************************
// cVarExprNode.h
//
// Defines an AST node for a variable.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu
#include "cAstNode.h"
#include "cSymbolTable.h"
#include "cExprNode.h"
#include "cVarExprNode.h"
#include "cDeclNode.h"

class cVarExprNode : public cExprNode 
{ 
    public: cVarExprNode(cSymbol* s) 
            {
                if(!g_symbolTable.Find(s->GetName()))
                {
                    SemanticParseError("Symbol " + s->GetName() + " not defined");
                }
                AddChild(s);
                m_name = s->GetName();
            } 

            void Insert(cSymbol* s)
            {
                AddChild(s);
            }

            void Insert(cAstNode* node)
            {
                AddChild(node);
            }



    virtual string NodeType() { return string("varref"); } 
    virtual void Visit(cVisitor *visitor) { visitor->Visit(this); } 
    virtual cDeclNode* GetDecl() 
    {
        cSymbol* s = dynamic_cast<cSymbol*>(GetChild(0));
        return s ? s->GetDecl() : nullptr;
    }

    virtual cDeclNode* GetType() override
    {
        cDeclNode* decl = GetDecl();
        return decl ? decl->GetType() : nullptr;
    }
    std::string GetVarName()
    {
        return m_name;
    }

    protected:
    std::vector<cExprNode*> indices;
    std::string m_name;

};


