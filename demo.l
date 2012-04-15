
%{
#include "demo.h"
#include "demo.tab.h"
#include <string.h>
%}

ID		[a-zA-Z_][a-zA-Z0-9_]*
OBJ		@{ID}
NUM		[+\-]?[0-9]+(\.[0-9]+)?s?
SYMBOL	[{}\[\]()]
COMMENT	((#|\/\/).*$)|(\$[^\$]*\$)

%%

{ID} {
	yylval.str = strdup(yytext);
	return TKID;
}
{OBJ} {
	yylval.str = strdup(&yytext[1]);
	return TKOBJ;
}
{NUM} {
	yylval.num = atof(yytext);
	if (yytext[yyleng-1] == 's') yylval.num *= 1000.0f;
	return TKNUM;
}
{SYMBOL} {
	return yytext[0];
}
[ \t\n\r] ;
{COMMENT} ;
. {
	yyerror("invalid character");
	return -1;
}

%%

