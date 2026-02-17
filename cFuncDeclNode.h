#pragma once
//**********************************
// cFuncDeclNode.h
//
// Defines parse logic for function declarations.
//
// Author: Aden Ratliff
// aden.ratliff@oit.edu

#include "cAstNode.h"
#include "cDeclNode.h"
#include "cParamsNode.h"
class cFuncDeclNode : public cDeclNode
{
public:
    cFuncDeclNode(cSymbol* type, cSymbol* name, cParamsNode* params)
        : m_type(type), m_name(name), m_params(params), m_decls(nullptr), m_stmts(nullptr)
    {
        cSymbol* existing= g_symbolTable.FindLocal(name->GetName());
        if (existing)
        {
            cDeclNode* existingDecl = existing->GetDecl();
            if(existingDecl && existingDecl->IsVar())
            {
                SemanticParseError("Symbol " + existing->GetName() + " already defined in current scope");
            }
            cFuncDeclNode* existingFunc = dynamic_cast<cFuncDeclNode*>(existing->GetDecl());
            if (existingFunc)
            {
                if(!existingFunc->IsFunc())
                {
                    SemanticParseError("Symbol " + existing->GetName() + " already defined in current scope");
                }
                else if (m_stmts)
                {
                    existingFunc->SetStmts(m_stmts);
                }
                m_stmts = existingFunc->GetStmts();
            }
            m_name = existing;
        }
        else
        {
            g_symbolTable.Insert(name);
            m_name = name;
            m_name->SetDecl(this);
        }
        AddChild(m_type);
        AddChild(m_name);
        AddChild(m_params);
        if (m_stmts) AddChild(m_stmts);
    }
    cFuncDeclNode(cSymbol* type, cSymbol* name, cParamsNode* params, cStmtsNode* stmts)
        : m_type(type), m_name(name), m_params(params), m_decls(nullptr), m_stmts(stmts)
    {
        cSymbol* existing = g_symbolTable.FindLocal(name->GetName());
        if (existing)
        {
            cDeclNode* existingDecl = existing->GetDecl();
            if(existingDecl && existingDecl->IsVar())
            {
                SemanticParseError("Symbol " + existing->GetName() + " already defined in current scope");
            }
            cFuncDeclNode* existingFunc = dynamic_cast<cFuncDeclNode*>(existing->GetDecl());
            if (existingFunc)
            {
                if(!existingFunc->IsFunc())
                {
                    SemanticParseError("Symbol " + existing->GetName() + " already defined in current scope");
                }
                else if (m_stmts)
                {
                    existingFunc->SetStmts(m_stmts);
                }
                m_stmts = existingFunc->GetStmts();
            }
            m_name = existing;
        }
        else
        {
            g_symbolTable.Insert(name);
            m_name = name;
            m_name->SetDecl(this);
        }
        AddChild(m_type);
        AddChild(m_name);
        AddChild(m_params);
        AddChild(m_stmts);
    }
    cFuncDeclNode(cSymbol* type, cSymbol* name, cParamsNode* params, cDeclsNode* decls, cStmtsNode* stmts)
        : m_type(type), m_name(name), m_params(params), m_decls(decls), m_stmts(stmts)
    {
        cSymbol* existing = g_symbolTable.FindLocal(name->GetName());
        if (existing)
        {
            cDeclNode* existingDecl = existing->GetDecl();
            if(existingDecl && existingDecl->IsVar())
            {
                SemanticParseError("Symbol " + existing->GetName() + " already defined in current scope");
            }
            cFuncDeclNode* existingFunc = dynamic_cast<cFuncDeclNode*>(existing->GetDecl());
            if (existingFunc)
            {
                if(!existingFunc->IsFunc())
                {
                    SemanticParseError("Symbol " + existing->GetName() + " already defined in current scope");
                }
                else if (m_stmts)
                {
                    existingFunc->SetStmts(m_stmts);
                }
                m_stmts = existingFunc->GetStmts();
            }
            m_name = existing;
        }
        else
        {
            g_symbolTable.Insert(name);
            m_name = name;
            m_name->SetDecl(this);
        }
        AddChild(m_type);
        AddChild(m_name);
        AddChild(m_params);
        AddChild(m_decls);
        AddChild(m_stmts);
    }
    virtual string NodeType() override { return "func"; }
    virtual bool IsFunc() override { return true; }
    virtual void Visit(cVisitor* visitor) override { visitor->Visit(this); }
    virtual cDeclNode* GetType() override { return this; }
    virtual cDeclNode* GetDecl() { return this; }
    bool HasBody() const { return m_stmts != nullptr; }
    cStmtsNode* GetStmts() const { return m_stmts; }
    cDeclsNode* GetDecls() const { return m_decls; }
    cParamsNode* GetParams() const { return m_params; }
    void SetStmts(cStmtsNode* stmts) { m_stmts = stmts; }
private:
    cSymbol* m_type;
    cSymbol* m_name;
    cParamsNode* m_params;
    cDeclsNode* m_decls;
    cStmtsNode* m_stmts;
};

