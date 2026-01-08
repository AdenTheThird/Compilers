//**************************************************************
// cSymbol.cpp - Implement the class defined by cSymbol.h
//
// Author: Aden Ratliff
//**************************************************************

#include "cSymbolTable.h"

cSymbolTable::cSymbolTable()
{
    scopes.push_back(symbolTable_t{});
}




