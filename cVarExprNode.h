#pragma once
//****************************************
// cVarExprNode.h
//
// Defines an AST node for a variable.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu
#include "cAstNode.h"
#include "cExprNode.h"
#include "cVarExprNode.h"

class cVarExprNode : public cExprNode 
{ 
    public: cVarExprNode(cSymbol* s) 
            {
                if(!g_symbolTable.Find(s->GetName()))
                {
                    SemanticParseError("Symbol " + s->GetName() + " not defined");
                }
                AddChild(s);
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

    protected:
    std::vector<cExprNode*> indices;

};


