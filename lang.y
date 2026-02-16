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
cAstNode* yyast_root;

#define CHECK_ERROR() { if (g_semanticErrorHappened) { g_semanticErrorHappened = false; } }
#define PROP_ERROR() { if (g_semanticErrorHappened) { g_semanticErrorHappened = false; YYERROR; } }
%}

%locations
%code requires {
    #include <string>

    class cBaseClassNode;
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
    char*           symbolName;
    struct sFuncHeader*    func_header_info;
    cParamsNode*    params_node;
    }

%start  program

%token <symbol>    IDENTIFIER
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
%type <decl_node> param
%type <expr_node> expr
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
                            {  }
        |   error ';'
                            {  }

var_decl:   TYPE_ID IDENTIFIER
                                 {
                                    cSymbol* typeSym = g_symbolTable.Find($1->GetName());
                                    cSymbol* idSym = g_symbolTable.FindLocal($2->GetName());
                                    if (!idSym)
                                    { 
                                        idSym = new cSymbol($2->GetName());
                                        g_symbolTable.Insert(idSym);

                                    }
                                    $$ = new cVarDeclNode(typeSym, idSym);
                                }
                                           
struct_decl:  STRUCT open decls close IDENTIFIER 
                                { 
                                    cSymbol* typeSym = $5;
                                    cStructDeclNode* decl = new cStructDeclNode($3, typeSym);
                                    typeSym->SetDecl(decl);
                                    

                                    g_symbolTable.Insert(typeSym);
                                    $$ = decl;
                                    }
                

array_decl:   ARRAY TYPE_ID '[' INT_VAL ']' IDENTIFIER
                                { cSymbol* c = $6;
                                cArrayDeclNode* decl  = new cArrayDeclNode($2, $4, c);
                                c->SetDecl(decl);
                                g_symbolTable.Insert(c);
                                $$ = decl;
                                }

func_decl:  func_header ';'
                                { 
                                $$ = new cFuncDeclNode($1->type, $1->name, $1->params);
                                g_symbolTable.DecreaseScope(); }
        |   func_header  '{' decls stmts '}'
                                { 
                                  $$ = new cFuncDeclNode($1->type, $1->name, $1->params, $3, $4); 
                                g_symbolTable.DecreaseScope(); }
        |   func_header  '{' stmts '}'
                                { $$ = new cFuncDeclNode($1->type, $1->name, $1->params, $3); 
                                g_symbolTable.DecreaseScope(); }
func_header: func_prefix paramsspec ')'
                                { $1->params = $2;
                                  $$ = $1;  }
        |    func_prefix ')'
                            { $1->params = nullptr;
                              $$ = $1;  }
func_prefix: TYPE_ID IDENTIFIER '('
                                {
                                  sFuncHeader* s = new sFuncHeader;
                                  s->type = $1;
                                  s->name = $2;
                                  if(!s->name)
                                  {
                                      s->name = new cSymbol(*$2);
                                      g_symbolTable.Insert(s->name);
                                  }
                                  s->params = nullptr;
                                  g_symbolTable.IncreaseScope();
                                  $$ = s;
                                }
paramsspec:  paramsspec ',' paramspec
                                { $1->Insert($3);
                                  $$ = $1; }
        |   paramspec
                            { $$ = new cParamsNode($1);
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
                            { cSymbol* c = $1;
                              if(!c)
                                {
                                    yyerror("Call to undeclared function!\n");
                                }
                              $$ = new cFuncExprNode(c, $3); } 
        |   IDENTIFIER '(' ')'
                            { cSymbol* c = $1;
                              if(!c)
                                {
                                    yyerror("Call to undeclared function!\n");
                                }
                              $$ = new cFuncExprNode(); } 
varref:   varref '.' IDENTIFIER
                            {
                            cSymbol* s = $3;
                            if(!s)
                            {
                            }
                                $1->Insert(s);                  
                                $$ = $1;                                
                            }
        | varref '[' expr ']'
                            {  
                              $1->Insert($3);
                              $$ = $1;
                            }
                              

        | varpart       
                            { $$ = $1; }

varpart:  IDENTIFIER
                                {
                                    cSymbol* sym = g_symbolTable.Find($1->GetName());
                                    $$ = new cVarExprNode(sym);
                                }

lval:     varref
                                { $$ = $1; }

params:   params ',' param
                            { $1->Insert($3);
                              $$ = $1;   }
        |   param
                            { $$ = new cParamListNode();  
                              $$->Insert($1); }

param:      expr
                                {  }

expr:       expr EQUALS addit
                                { cOpNode* op = new cOpNode(EQUALS);
                                 $$ = new cBinaryExprNode($1, op, $3);  }
        |   addit
                            { $$ = $1; }

addit:      addit '+' term
                                { cOpNode* op = new cOpNode('+');
                                  $$ = new cBinaryExprNode($1, op, $3); }
        |   addit '-' term
                            { cOpNode* op = new cOpNode('-');
                              $$ = new cBinaryExprNode($1, op, $3); }
        |   term
                            { $$ = $1; }

term:       term '*' fact
                                { cOpNode* op = new cOpNode('*');
                                  $$ = new cBinaryExprNode($1, op, $3); }
        |   term '/' fact
                            { cOpNode* op = new cOpNode('/');
                              $$ = new cBinaryExprNode($1, op, $3); }
        |   term '%' fact
                            { cOpNode* op = new cOpNode('%');
                              $$ = new cBinaryExprNode($1, op, $3); }
        |   fact
                            { $$ = $1; }

fact:       '(' expr ')'
                                { $$ = $2; }
        |   INT_VAL
                             { $$ = new cIntExprNode($1); }
        |   FLOAT_VAL
                            { $$ = new cFloatExprNode($1); }
        |   varref
                            {  }
        |   func_call
                            {  }

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

