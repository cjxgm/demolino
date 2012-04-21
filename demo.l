
%{
#include "demo.h"
#include "demo.tab.h"
#include <string.h>
%}

OBJ		@[a-zA-Z_][a-zA-Z0-9_]*
ID		[a-zA-Z_\-][a-zA-Z0-9_\-]*
NUM		[+\-]?[0-9]+(\.[0-9]+)?s?
SYMBOL	[{}\[\]()]
CMT1	#.*$
CMT2	\/\/.*$
CMT3	\$[^\$]*\$

%%

{OBJ} {
	yylval.str = strdup(&yytext[1]);
	return TKOBJ;
}
{ID} {
	yylval.str = strdup(yytext);
	return TKID;
}
{NUM} {
	yylval.num = atof(yytext);
	if (yytext[yyleng-1] == 's') yylval.num *= 1000.0f;
	return TKNUM;
}
{SYMBOL} {
	return yytext[0];
}
[ \t\n\r]	;
{CMT1}		;
{CMT2}		;
{CMT3}		;
. {
	yyerror("invalid character");
	return -1;
}

%%

