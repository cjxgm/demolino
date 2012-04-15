
#include "object.h"
#include "util.h"
#include "compiler.h"
#include <string.h>

Link * objs;

typedef struct ObjectCompiler
{
	void (*decl)(Object * o);
	void (*call)(Object * o);
	void (*def)(Object * o);
}
ObjectCompiler;

static void timeline_decl(Object * o);
static void timeline_call(Object * o);
static void timeline_def(Object * o);
static void sphere_call(Object * o);
static void trans_call(Object * o);

ObjectCompiler ocs[] = {
	{NULL, NULL, NULL},
	{NULL, NULL, NULL},
	{&timeline_decl, &timeline_call, &timeline_def},
	{NULL, &trans_call, NULL},
	{NULL, &sphere_call, NULL},
};

const char * type2name[] = {
	"(UNDEFINED)",
	"demo",
	"timeline",
	"trans",
	"sphere"
};

Object * object_get_by_name(const char * name)
{
	{ TRAVERSE(objs, Object, p)
		if (!strcmp(p->name, name))
			return p;
	}
	return NULL;
}

ObjectType object_get_type_by_name(const char * name)
{
	int i;
	for (i=0; i<LEN(type2name); i++)
		if (!strcmp(type2name[i], name))
			return i;
	yyerror("unknown object type");
	return 0;
}

Data * object_get_param_data_by_name(Object * o, const char * name)
{
	{ TRAVERSE(o->param, Param, p)
		if (!strcmp(p->name, name))
			return p->data;
	}
	return NULL;
}

void object_decl(Object * o)
{
	if (ocs[o->type].decl)
		ocs[o->type].decl(o);
}

void object_call(Object * o)
{
	if (ocs[o->type].call)
		ocs[o->type].call(o);
}

void object_def(Object * o)
{
	if (ocs[o->type].def)
		ocs[o->type].def(o);
}



////////////////////////////////////////
//
// timeline
//

static void timeline_decl(Object * o)
{
	printf("void __tl_%s(long __t);\n", o->name);
}

static void timeline_call(Object * o)
{
	printf("\t\t__tl_%s(__rt);\n", o->name);
}

static void timeline_def(Object * o)
{
	printf("\nvoid __tl_%s(long __t)\n", o->name);
	printf("{\n");
	printf("\tlong __rt = __t;\n");
	Data * d = object_get_param_data_by_name(o, "objs");
	CHECK(d->type == DT_LIST, "invalid `objs' for `timeline': list expected");
	{ TRAVERSE(d->l, Data, p)
		compile_sequencer(p);
	}
	printf("}\n");
}

static void sphere_call(Object * o)
{
	Data * d;
	printf("\t\tdraw_sphere(");
	if ((d = object_get_param_data_by_name(o, "nslice")))
		compile_data(d);
	else printf("16");
	printf(", ");
	if ((d = object_get_param_data_by_name(o, "nstack")))
		compile_data(d);
	else printf("16");
	printf(");\n");
}

static void trans_call(Object * o)
{
	printf("\t\tglPushMatrix();\n");

	Data * d;

	d = object_get_param_data_by_name(o, "obj");
	CHECK(d->type == DT_OBJ, "invalid `obj' for `trans'");

	{ TRAVERSE(o->param, Param, p) {
		if (!strcmp(p->name, "obj")) continue;
		if (!strcmp(p->name, "mov")) {
			printf("\t\tglTranslatef(");
			compile_vector(p->data);
			printf(");");
			continue;
		}
		if (!strcmp(p->name, "scale")) {
			printf("\t\tglScalef(");
			compile_vector(p->data);
			printf(");");
			continue;
		}
		if (!strcmp(p->name, "rot")) {
			Data * d = p->data;
			CHECK(d->type == DT_LIST,
					"invalid `rot' in `trans': list expected");
			CHECK(link_length(d->l) == 3,
					"invalid `rot' in `trans': 3 elements expected");

			d = (Data *)d->l->prev;
			printf("\t\tglRotatef(");
			compile_data(d);
			printf(", 0, 0, 1);\n");

			d = d->prev;
			printf("\t\tglRotatef(");
			compile_data(d);
			printf(", 0, 1, 0);\n");

			d = d->prev;
			printf("\t\tglRotatef(");
			compile_data(d);
			printf(", 1, 0, 0);\n");

			continue;
		}
		yyerror("invalid parameter in `trans'");
	}}

	object_call(d->o.o);
	printf("\t\tglPopMatrix();\n");
}

