
#ifdef DEBUG
#include "debug.h"
#include "demo.h"
#include <stdio.h>

static void print_data(Data * d);

void debug_print_objs()
{
	{ TRAVERSE(objs, Object, p) {
		printf("%s(%d)<%4.4X>\n", (p->name ? p->name : "*UNNAMED*"), p->type, (int)p & 0xFFFF);
		{ TRAVERSE(p->param, Param, q) {
			printf("    %s: ", q->name);
			print_data(q->data);
			printf("\n");
		}}
	}}
}

static void print_data(Data * d)
{
	switch (d->type) {
		case DT_NUM:
			printf("%g", d->n);
			break;
		case DT_OBJ:
			printf("@%s<%4.4X>", d->o.name, (int)d->o.o & 0xFFFF);
			break;
		case DT_ANIM:
			printf("[");
			print_data(d->a.s);
			printf(" ");
			print_data(d->a.e);
			printf(" ");
			print_data(d->a.ts);
			printf(" ");
			print_data(d->a.te);
			printf("]");
			break;
		case DT_LIST:
			printf("(");
			if (link_length(d->l)) {
				TRAVERSE(d->l, Data, p) {
					print_data(p);
					printf("%c", ((Link *)p->next == d->l ? ')' : ' '));
				}
			}
			else printf(")");
			break;
	}
}

#endif

