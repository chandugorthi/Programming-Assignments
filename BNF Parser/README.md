The program reads 8-bit ASCII strings from standard input. The input is tokenized using the below specification.

letter -> [a-zA-Z]
digit -> [0-9]
newline -> \n
for -> for
while -> while
if -> if
else -> else
identifier -> (letter|_)(letter|digit|_)*
integer -> digit+
float -> (digit+\.digit*)|(\.digit+)
string -> "[^"\n]*"
whitespace -> (' ' | \t)+
comment -> #.*
operator -> '!' | '%' | '&' | '|' | '+' | '-' | '*' | '/' | '{' | '}' | '[' | ']' | ';' | ',' | '<' | '>' | '=' | '<=' | '>=' | '!=' | ':='

Lexical Pattern   Numeric Type  English Type  Value to output
identifier              1             ID        Exact match; exactly what the user input
string                  2             STR       The user’s input minus the quotes
integer                 3             INT       Exact match
float                   4             FLOAT     Exact match
whitespace              5             WS        None
newline                 6             NEWLINE   None
comment                 7             COMMENT   Exact match
for                     8             FOR       Exact match
while                   9             WHILE     Exact match
if                      10            IF        Exact match
else                    11            ELSE      Exact match
'!'                     12            !         None
'%'                     13            %         None
'&'                     14            &         None
'|'                     15            |         None
'+'                     16            +         None
'-'                     17            -         None
'*'                     18            *         None
'/'                     19            /         None
'{'                     20            {         None
'}'                     21            }         None
'['                     22            [         None
']'                     23            ]         None
';'                     24            ;         None
','                     25            ,         None
'<'                     26            <         None
'>'                     27            >         None
'='                     28            =         None
'<='                    29            <=        None
'>='                    30            >=        None
'!='                    31            !=        None
‘:='                    32            :=        None
'('                     33            (         None
‘)'                     34            )         None
n/a                     97            ERR1      None
n/a                     98            ERR2      None
n/a                     99            ERR3      None
n/a                     100           ERR4      None

The tokenized stream is then parsed into a parse tree using the following BNF Specification

<stmt> ::= <while_stmt> <stmt> | <for_stmt> <stmt> | <if_stmt> <stmt> | <assignment_stmt> ';' <stmt> | <output_stmt> <stmt> | ε
<while_stmt> ::= 'while' '(' <cond_expr> ')' '{' <stmt> '}'
<for_stmt> ::= 'for' '(' <opt_assign> ';' <opt_cond_expr> ';' <opt_assign> ')' '{' <stmt> '}'
<if_stmt> ::= 'if' '(' <cond_expr> ')' '{' <stmt> '}' <opt_else>
<opt_else> ::= 'else' '{' <stmt> '}' | ε
<assignment_stmt> ::= ID ':=' <expr>
<output_stmt> ::= '[' <expr> ']' ';'
<expr> ::= <expr> '+' <mult_expr> | <mult_expr>
<mult_expr> ::= <mult_expr> '*' <unary_expr> | <unary_expr>
<unary_expr> ::= '!' <expr> | '+' <expr> | '-' <expr> | <paren_expr>
<paren_expr> ::= '(' <expr> ')' | <basic_expr>
<basic_expr> ::= ID | STR | NUM | FLOAT
<cond_expr> ::= <cond_expr> '|' <and_expr> | <and_expr>
<and_expr> ::= <and_expr> '&' <cmp_expr> | <eq_expr>
<eq_expr> ::= <eq_expr> '=' <cmp_expr> | <eq_expr> '!=' <cmp_expr> | <cmp_expr>
<cmp_expr> ::= <cmp_expr> '<' <expr> | <cmp_expr> '>' <expr> | <cmp_expr> '<=' <expr> | <cmp_expr> '>=' <expr> | <expr>
<opt_cond_expr> ::= <cond_expr> | ε
<opt_assign> ::= <assign_stmt> | ε
