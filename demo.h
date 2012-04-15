
#ifndef __DEMO_H
#define __DEMO_H

#include "link.h"

extern Link * objs;		// in object.c

typedef enum DataType
{
	DT_NUM = 1,
	DT_OBJ,
	DT_ANIM,
	DT_LIST,
}
DataType;

typedef enum ObjectType
{
	OT_DEMO = 1,
	OT_TIMELINE,
	OT_TRANS,
	OT_SPHERE,
}
ObjectType;

typedef struct Data Data;
typedef struct Object Object;
typedef struct Param Param;

struct Data
{
	LINKABLE(Data);
	DataType type;
	union {
		float n;
		Link/*Data*/ * l;
		struct {
			const char * name;
			Object * o;
		} o;
		struct {
			Data * s;	// start
			Data * e;	// end
			Data * ts;	// time start
			Data * te;	// time end
		} a;
	};
};

struct Object
{
	LINKABLE(Object);
	ObjectType type;
	const char * name;
	Link/*Param*/ * param;
};

struct Param
{
	LINKABLE(Param);
	const char * name;
	Data	   * data;
};




int yyerror(const char * err);
int yylex();
int yyparse();

#endif

