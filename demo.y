%{
#include <string.h>
#include "demo.h"
#include "util.h"
#include "object.h"

%}

%union
{
	const char * str;
	float		 num;
	Data   * data;
	Object * obj;
	Link   * link;
	Param  * param;
}

%token <str> TKID
%token <str> TKOBJ
%token <num> TKNUM

%type <obj> obj_decl
%type <obj> obj_body
%type <link> params;
%type <param> param;

%type <data> data;
%type <data> data_num;
%type <data> data_obj;
%type <data> data_anim;
%type <data> data_list;
%type <link> data_list_body;

%%

demo: demo obj_decl {
}
| obj_decl {
}
;

obj_decl: TKOBJ obj_body {
	$2->name = $1;
	$$ = $2;
}
;

obj_body: '{' TKID  params '}' {
	$$ = NEW(Object);
	link_addtail(objs, $$);

	$$->type = object_get_type_by_name($2);

	$$->name = NULL;
	$$->param = $3;
}
;

params: params param {
	$$ = $1;
	link_addtail($$, $2);
}
| param {
	$$ = NEW(Link);
	link_init($$);
	link_addtail($$, $1);
}
;

param: TKID data {
	$$ = NEW(Param);
	$$->name = $1;
	$$->data = $2;
}
;

data: data_num
| data_obj
| data_anim
| data_list
;

data_num: TKNUM {
	$$ = NEW(Data);
	$$->type = DT_NUM;
	$$->n = $1;
}
;

data_obj: TKOBJ {
	$$ = NEW(Data);
	$$->type = DT_OBJ;
	$$->o.name = $1;
}
;

data_anim: '[' data data data data ']' {
	$$ = NEW(Data);
	$$->type = DT_ANIM;
	$$->a.s = $2;
	$$->a.e = $3;
	$$->a.ts = $4;
	$$->a.te = $5;
}
;

data_list: '(' data_list_body ')' {
	$$ = NEW(Data);
	$$->type = DT_LIST;
	$$->l = $2;
}
| '(' ')' {
	$$ = NEW(Data);
	$$->type = DT_LIST;
	$$->l = NEW(Link);
	link_init($$->l);
}
;

data_list_body: data_list_body data {
	$$ = $1;
	link_addtail($$, $2);
}
| data {
	$$ = NEW(Link);
	link_init($$);
	link_addtail($$, $1);
}
;

%%

