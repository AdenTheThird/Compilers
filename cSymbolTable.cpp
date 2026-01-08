//**************************************************************
// cSymbol.cpp - Implement the class defined by cSymbol.h
//
// Author: Aden Ratliff
//**************************************************************

#include "cSymbolTable.h"
#include "cSymbol.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Creates new empty symbol table
cSymbolTable::cSymbolTable()
{
    scopes.push_back(symbolTable_t{});
}

// Adds a new scope and returns pointer 
// to that scope
symbolTable_t* cSymbolTable::IncreaseScope()
{
    scopes.push_back(symbolTable_t{});
    return &scopes.back();
}

// Pops the outermost scope.
symbolTable_t* cSymbolTable::DecreaseScope()
{
    if(scopes.size() > 1)
    {
        scopes.pop_back();
    }
    else
    {
        std::cout << "Warning: Outermost scope reached." << std::endl;
    }

    return &scopes.back();
}

// Inserts new symbol into the current scope
void cSymbolTable::Insert(cSymbol *sym)
{
    symbolTable_t &currentScope = scopes.back();
    currentScope.insert({sym->GetName(), sym});
}

// Finds a value given a name across all scopes.
cSymbol* cSymbolTable::Find(string name)
{
    for (int i = scopes.size() - 1; i >= 0; i--)
    {
        auto it = scopes[i].find(name);
        if (it != scopes[i].end())
        {
            return it->second;
        }
    }
    return nullptr;
}

// Finds a value given a name in the current scope only.
cSymbol* cSymbolTable::FindLocal(string name)
{
    auto it = scopes.back().find(name);
    if (it != scopes.back().end())
    {
        return it->second;
    }
    return nullptr;
}

