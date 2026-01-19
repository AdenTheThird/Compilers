%{
//**************************************
// lang.l
//
// Scanner definition file for CST320 Lab 2
// Author: Aden Ratliff
//

#include "lex.h"
#include "tokens.h"
#include "cSymbolTable.h"
#include "cSymbol.h"

// Debug output macro
//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
    #define DO_RETURN(a) { return Return(a); }
#else
    #define DO_RETURN(a) { return (a); }
#endif

int Return(int val);

int IncreaseScope();
int DecreaseScope();
static int ProcessIdentifier(const char* text);
%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

%%

"("     { DO_RETURN('('); }
")"     { DO_RETURN(')'); }
"{"     { IncreaseScope(); DO_RETURN('{'); }
"}"     { DecreaseScope(); DO_RETURN('}'); }
"["     { DO_RETURN('['); }
"]"     { DO_RETURN(']'); }
";"     { DO_RETURN(';'); }
","     { DO_RETURN(','); }
"."     { DO_RETURN('.'); }
"+"     { DO_RETURN('+'); }
"-"     { DO_RETURN('-'); }
"*"     { DO_RETURN('*'); }
"/"     { DO_RETURN('/'); }
"%"     { DO_RETURN('%'); }
"="     { DO_RETURN('='); }
">"     { DO_RETURN('>'); }
"<"     { DO_RETURN('<'); }

"!="    { DO_RETURN(NOT_EQUALS); }
"=="    { DO_RETURN(EQUALS); }
"&&"    { DO_RETURN(AND); }
"||"    { DO_RETURN(OR); }
">="    { DO_RETURN(GE); }
"<="    { DO_RETURN(LE); }

"program"  { DO_RETURN(PROGRAM); }
"if"       { DO_RETURN(IF); }
"else"     { DO_RETURN(ELSE); }
"endif"    { DO_RETURN(ENDIF); }
"while"    { DO_RETURN(WHILE); }
"print"    { DO_RETURN(PRINT); }
"prints"   { DO_RETURN(PRINTS); }
"char"     { DO_RETURN(CHAR); }
"int"      { DO_RETURN(INT); }
"long"     { DO_RETURN(LONG); }
"float"    { DO_RETURN(FLOAT); }
"double"   { DO_RETURN(DOUBLE); }
"struct"   { DO_RETURN(STRUCT); }
"array"    { DO_RETURN(ARRAY); }
"return"   { DO_RETURN(RETURN); }

[_a-zA-Z][_a-zA-Z0-9]*  { DO_RETURN(ProcessIdentifier(yytext)); }
[0-9]+\.[0-9]+         { DO_RETURN(FLOAT_VAL); }
[0-9]+                 { DO_RETURN(INT_VAL); }
\"[^\"]*\"             { DO_RETURN(STRING_LIT); }

[ \t\r\n]+             ; 
"//".*                 ; 

.         { DO_RETURN(JUNK_TOKEN); }
%%

int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}

static int ProcessIdentifier(const char* text)
{
    cSymbol* symbol = g_symbolTable.FindLocal(text);
    if(!symbol)
    {
        symbol = new cSymbol(text);
        g_symbolTable.Insert(symbol);
    }
    yylval.symbol = symbol;
    return IDENTIFIER;
}

int IncreaseScope()
{
    g_symbolTable.IncreaseScope(); 
    return '{';
}

int DecreaseScope()
{
    g_symbolTable.DecreaseScope(); 
    return '}';
}
