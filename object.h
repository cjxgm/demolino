
#ifndef __OBJECT_H
#define __OBJECT_H

#include "demo.h"

Object * object_get_by_name(const char * name);
ObjectType object_get_type_by_name(const char * name);
Data * object_get_param_data_by_name(Object * o, const char * name);

void object_decl(Object * o);
void object_call(Object * o);
void object_def(Object * o);

#endif

