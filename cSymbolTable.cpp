//**************************************************************
// cSymbol.cpp - Implement the class defined by cSymbol.h
//
// Author: Aden Ratliff
//**************************************************************

#include "cSymbolTable.h"
#include "cSymbol.h"

cSymbolTable::cSymbolTable()
{
    scopes.push_back(symbolTable_t{});
}

symbolTable_t *increaseScope()
{
    scopes.push_back(symbolTable_t{});
    return &scopes.back();
}

symbolTable_t *decreaseScope()
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

void Insert(cSymbol *sym)
{
    symbolTable_t &currentScope = scopes.back();
    currentScope.insert({sym->GetName(), sym);
}
