%{
//**************************************
// lang.y
//
// Parser definition file. bison uses this file to generate the parser.
//
// Author: Phil Howard 
//

#include <iostream>
#include <string>
#include "astnodes.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

int yylex();
int yyerror(const char* msg);
bool g_semanticErrorHappened = false;
bool g_isArrayIndex = false;
cAstNode* yyast_root;

%}

%locations
%code requires {
    #include <string>

    class cReturnNode;
    class cAstNode;
    class cBinaryExprNode;
    class cProgramNode;
    class cBlockNode;
    class cDeclNode;
    class cDeclsNode;
    class cIfNode;
    class cStmtNode;
    class cStmtsNode;
    class cPrintNode;
    class cExprNode;
    class cIntExprNode;
    class cFloatExprNode;
    class cVarDeclNode;
    class cVarExprNode;
    class cSymbol;
    class cWhileNode;
    class cAssignNode;
    class cFuncDeclNode;
    class cParamsNode;
    class cParamListNode;
    class cArrayDeclNode;
    class cPrintsNode;
    
    struct sFuncHeader
    {
        cSymbol* type;
        cSymbol* name;
        cParamsNode* params;
    };
}
%union{
    int             int_val;
    float           float_val;
    std::string*    str_val;
    cAstNode*       ast_node;
    cProgramNode*   program_node;
    cBlockNode*     block_node;
    cStmtNode*      stmt_node;
    cStmtsNode*     stmts_node;
    cExprNode*      expr_node;
    cIntExprNode*   int_node;
    cDeclNode*      decl_node;
    cDeclsNode*     decls_node;
    cSymbol*        symbol;
    cVarExprNode*   var_expr_node;
    struct sFuncHeader*    func_header_info;
    cParamsNode*    params_node;
    }

%start  program

%token <str_val>    IDENTIFIER
%token <symbol>    TYPE_ID
%token <int_val>   CHAR_VAL
%token <int_val>   INT_VAL
%token <float_val> FLOAT_VAL
%token <int_val>   LE
%token <int_val>   GE
%token <int_val>   AND
%token <int_val>   OR
%token <str_val>   EQUALS
%token <int_val>   NOT_EQUALS
%token <str_val>   STRING_LIT

%token  PROGRAM
%token  PRINT PRINTS
%token  WHILE IF ELSE ENDIF
%token  STRUCT ARRAY
%token  RETURN
%token  JUNK_TOKEN
%token  CHAR
%token  INT
%token  LONG
%token  FLOAT
%token  DOUBLE

%type <program_node> program
%type <block_node> block
%type <ast_node> open
%type <ast_node> close
%type <decls_node> decls
%type <decl_node> decl
%type <decl_node> var_decl
%type <decl_node> struct_decl
%type <decl_node> array_decl
%type <decl_node> func_decl
%type <func_header_info> func_header
%type <func_header_info> func_prefix
%type <ast_node> func_call
%type <params_node> paramsspec
%type <decl_node> paramspec
%type <stmts_node> stmts
%type <stmt_node> stmt
%type <var_expr_node> lval
%type <params_node> params
%type <expr_node> param
%type <expr_node> expr logic_expr rel_expr
%type <expr_node> addit
%type <expr_node> term
%type <expr_node> fact
%type <var_expr_node> varref
%type <var_expr_node> varpart

%%

program: PROGRAM block
                                { $$ = new cProgramNode($2);
                                  yyast_root = $$;
                                  if (yynerrs == 0) 
                                      YYACCEPT;
                                  else
                                      YYABORT;
                                }
block:  open decls stmts close
                                {
                                  $$ = new cBlockNode($2, $3);
                                }
    |   open stmts close
                                { $$ = new cBlockNode(nullptr, $2); }

open:   '{'
                                {  g_symbolTable.IncreaseScope();  }

close:  '}'
                                {  g_symbolTable.DecreaseScope();  }

decls:      decls decl
                                { $1->Insert($2);
                                  $$ = $1; }
        |   decl
                                { $$ = new cDeclsNode($1); }
decl:       var_decl ';'
                                { $$ = $1; }
        |   array_decl ';'
                            { $$ = $1; }
        |   struct_decl ';'
                            { $$ = $1;  }
        |   func_decl
                            { $$ = $1; }
        |   error ';'
                            {  }

var_decl:   TYPE_ID IDENTIFIER
                                    {
    cSymbol* typeSim = $1;
    std::string varName = *$2;

    cSymbol* idSym = g_symbolTable.FindLocal(varName);
    if (!idSym)
    {
        idSym = new cSymbol(varName);
    }

    $$ = new cVarDeclNode(typeSim, idSym);
                                    }
                                           
struct_decl: STRUCT open decls close IDENTIFIER
{
    std::string name = *$5;

    cSymbol* sym = g_symbolTable.FindLocal(name);
    if (!sym)
    {
        sym = new cSymbol(name);
    }

    $$ = new cStructDeclNode($3, sym);

}

array_decl: ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
{
    std::string name = *$6;

    cSymbol* sym = g_symbolTable.FindLocal(name);
    if (!sym)
    {
        sym = new cSymbol(name);
    }

    $$ = new cArrayDeclNode($2, $4, sym);

}

func_decl: func_header ';'
{
    g_symbolTable.DecreaseScope();
    $$ = new cFuncDeclNode($1->type, $1->name, $1->params, false);
    CHECK_ERROR();
}

| func_header '{' decls stmts '}'
{
    g_symbolTable.DecreaseScope();
    $$ = new cFuncDeclNode($1->type, $1->name, $1->params, $3, $4, true);
    CHECK_ERROR();

}

|   func_header  '{' stmts '}'
{
    g_symbolTable.DecreaseScope();
    $$ = new cFuncDeclNode($1->type, $1->name, $1->params, $3, true);
    CHECK_ERROR();
}
;
func_header: func_prefix paramsspec ')'
{
    $1->params = $2;

    if ($1->name)
    {
     cDeclNode* existingDecl = $1->name->GetDecl();
        if (existingDecl && existingDecl->IsFunc())
        {
            cFuncDeclNode* existingFunc = dynamic_cast<cFuncDeclNode*>(existingDecl);

            int existingCount = existingFunc->GetParams() ? existingFunc->GetParams()->Count() : 0;
            int newCount = $2 ? $2->Count() : 0;
            if (existingCount != newCount)
                SemanticParseError($1->name->GetName() + " redeclared with a different number of parameters");

        }
    }

    $$ = $1;
}


| func_prefix ')'
{
    $1->params = nullptr;
    CHECK_ERROR();
    $$ = $1;
}

func_prefix: TYPE_ID IDENTIFIER '('
{
    sFuncHeader* s = new sFuncHeader;
    s->type = $1;
    s->name = g_symbolTable.Find(*$2);

    if (s->name)
    {
        cDeclNode* existingDecl = s->name->GetDecl();
        
        if (existingDecl && existingDecl->IsFunc())
        {
            cFuncDeclNode* existingFunc = dynamic_cast<cFuncDeclNode*>(existingDecl);
        
            cSymbol* existingType = existingFunc->GetTypeSymbol();
            if (existingType->GetName() != $1->GetName())
            {
                SemanticParseError(*$2 + " previously declared with different return type"); 
            } 
            
        }
    }
    else
    {
        s->name = new cSymbol(*$2);
    }
    
    s->params = nullptr;
    g_symbolTable.IncreaseScope();
    $$ = s;
    CHECK_ERROR();
}

paramsspec:  paramsspec ',' paramspec
                                { $1->Insert($3);
                                  $$ = $1; }
        |   paramspec
                            { $$ = new cParamsNode($1, "args");
                              }

paramspec:  var_decl
                                    { $$ = $1; }

stmts:      stmts stmt
                                { $1->Insert($2);
                                  $$ = $1; }

        |   stmt
                            { $$ = new cStmtsNode($1); }

stmt:       IF '(' expr ')' stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5); }
        |   IF '(' expr ')' stmts ELSE stmts ENDIF ';'
                                { $$ = new cIfNode($3, $5, $7); } 
        |   WHILE '(' expr ')' stmt
                                { $$ = new cWhileNode($3, $5); }
        |   PRINT '(' expr ')' ';'
                                {
                                     $$ = new cPrintNode($3); }
        |   PRINTS '(' STRING_LIT ')' ';'
                                { $$ = new cPrintsNode(*$3); }
        |   lval '=' expr ';'
                            { $$ = new cAssignNode($1, $3); }
        |   func_call ';'
                            {  }
        |   block
                            {  }
        |   RETURN expr ';'
                            { $$ = new cReturnNode($2); }
        |   error ';'
                            {}

func_call:  IDENTIFIER '(' params ')'
                            { 
                              cSymbol* c = g_symbolTable.FindLocal(*$1);
                              if(!c)
                                {
                                    c = new cSymbol(*$1);
                                }
                              $$ = new cFuncExprNode(c, $3); 
                              CHECK_ERROR(); }
        |   IDENTIFIER '(' ')'
                            { 
                                cSymbol* c = g_symbolTable.Find(*$1);
                              if(!c)
                                {
                                    c = new cSymbol(*$1);
                                }
                              $$ = new cFuncExprNode(c, nullptr); 
                              CHECK_ERROR(); }

varref:   varref '.' IDENTIFIER
                            {
                            cSymbol* s = g_symbolTable.Find(*$3);
                            if(!s)
                            {
                                s = new cSymbol(*$3);
                            }

                            $1->Insert(s);                  
                            $$ = $1;                                
                            }
        | varref '[' expr ']'
                            {  
                              $1->Insert($3);
                              $1->isArrayAccess = true;
                              $$ = $1;
                            }
                              

        | varpart       
                            { $$ =$1; }


varpart: IDENTIFIER
{
                                    cSymbol* c = g_symbolTable.Find(*$1);
                                    if(!c)
                                    {
                                        c = new cSymbol(*$1);
                                    }
                                    
                                    $$ = new cVarExprNode(c);
                                    CHECK_ERROR();
                                        
}
lval:     varref
                                { $$ = $1; }

params:   params ',' param
                            { $1->Insert($3);
                              $$ = $1;   }
        |   param
                            { $$ = new cParamsNode($1, "params");  
}

param:      expr
                                {$$ = $1;  }

// Top-level expression
expr:
      expr EQUALS addit
        { $$ = new cBinaryExprNode($1, new cOpNode(EQUALS), $3); }
    | logic_expr
        { $$ = $1; }
    ;

// Logical operations
logic_expr:
      rel_expr
    | logic_expr AND rel_expr
        { $$ = new cBinaryExprNode($1, new cOpNode(AND), $3); }
    | logic_expr OR rel_expr
        { $$ = new cBinaryExprNode($1, new cOpNode(OR), $3); }
    ;

// Relational operations
rel_expr:
      addit
    | addit '>' addit
        { $$ = new cBinaryExprNode($1, new cOpNode('>'), $3); }
    | addit '<' addit
        { $$ = new cBinaryExprNode($1, new cOpNode('<'), $3); }
    | addit GE addit
        { $$ = new cBinaryExprNode($1, new cOpNode(GE), $3); }
    | addit LE addit
        { $$ = new cBinaryExprNode($1, new cOpNode(LE), $3); }
    | addit EQUALS addit
        { $$ = new cBinaryExprNode($1, new cOpNode(EQUALS), $3); }
    | addit NOT_EQUALS addit
        { $$ = new cBinaryExprNode($1, new cOpNode(NOT_EQUALS), $3); }
    ;

// Arithmetic: + / -
addit:
      addit '+' term
        { $$ = new cBinaryExprNode($1, new cOpNode('+'), $3); }
    | addit '-' term
        { $$ = new cBinaryExprNode($1, new cOpNode('-'), $3); }
    | term
        { $$ = $1; }
    ;

// Arithmetic: * / %
term:
      term '*' fact
        { $$ = new cBinaryExprNode($1, new cOpNode('*'), $3); }
    | term '/' fact
        { $$ = new cBinaryExprNode($1, new cOpNode('/'), $3); }
    | term '%' fact
        { $$ = new cBinaryExprNode($1, new cOpNode('%'), $3); }
    | fact
        { $$ = $1; }
    ;

// Leaves
fact:
      '-' fact
        { $$ = new cBinaryExprNode(new cIntExprNode(0), new cOpNode('-'), $2, true); }
    | '(' expr ')'
        { $$ = $2; }
    | INT_VAL
        { $$ = new cIntExprNode($1); }
    | FLOAT_VAL
        { $$ = new cFloatExprNode($1); }
    | varref
        { $$ = $1; }
    | func_call
        { $$ = dynamic_cast<cExprNode*>($1); }
    ;

%%

// Function to format error messages
int yyerror(const char *msg)
{
    std::cerr << "ERROR: " << msg 
         << " on line " << yylineno << "\n";

    return 0;
}

//Function that gets called when a semantic error happens
void SemanticParseError(std::string error)
{
    std::cout << "ERROR: " << error << " near line "
        << yylineno << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}

//Function that gets called when a semantic error happens
void PostParseError(std::string error, int line)
{
    std::cout << "ERROR: " << error << " near line "
        << line << "\n";
    g_semanticErrorHappened = true;
    yynerrs++;
}
