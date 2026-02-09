%{
//**************************************
// lang.l
//
// Scanner definition file for CST320 Lab 2
// Author: Aden Ratliff
//

#include "langparse.h"
#include "cSymbolTable.h"
#include "cSymbol.h"
#include "astnodes.h"

// Debug output macro
//#define DEBUG_OUTPUT
#ifdef DEBUG_OUTPUT
    #define DO_RETURN(a) { return Return(a); }
#else
    #define DO_RETURN(a) { return (a); }
#endif

int Return(int val);

%}

%option noyywrap
%option noinput
%option nounput
%option yylineno

%%

"("     { DO_RETURN('('); }
")"     { DO_RETURN(')'); }
"{"     { DO_RETURN('{'); }
"}"     { DO_RETURN('}'); }
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
"char"     { yylval.symbol = g_symbolTable.Find("char"); return TYPE_ID; }
"int"      { yylval.symbol = g_symbolTable.Find("int"); return TYPE_ID; }
"float"    { yylval.symbol = g_symbolTable.Find("float"); return TYPE_ID; }
"long"     { yylval.symbol = g_symbolTable.Find("long"); return TYPE_ID; }
"double"   { yylval.symbol = g_symbolTable.Find("double"); return TYPE_ID; }
"struct"   { DO_RETURN(STRUCT); }
"array"    { DO_RETURN(ARRAY); }
"return"   { DO_RETURN(RETURN); }

[_a-zA-Z][_a-zA-Z0-9]*  {
    std::string* id = new std::string(yytext);
    cSymbol* c = g_symbolTable.Find(*id);

    if(c && c->isType()) {
        yylval.symbol = c; 
        delete id;        
        return TYPE_ID;
    } else {
        yylval.str_val = id; 
        return IDENTIFIER;
    }
}

[0-9]+\.[0-9]+         { yylval.float_val = atof(yytext);
                         DO_RETURN(FLOAT_VAL); }
[0-9]+                 { yylval.int_val = atoi(yytext);
                         DO_RETURN(INT_VAL); }
\"[^\"]*\"             { yylval.str_val = new std::string(yytext + 1, yyleng - 2);
                         
                         return STRING_LIT;}

[ \t\r\n]+             ; 
"//".*                 ; 

.         { DO_RETURN(JUNK_TOKEN); }
%%

int Return(int val)
{
    printf("Scanned '%s': %d\n", yytext, val);
    return val;
}


