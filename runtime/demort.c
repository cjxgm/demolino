
#include <stdio.h>

static long ticks = 0;

long get_ticks()
{
	return ticks++;
}

float lirp(float v, float vf, float vt, float df, float dt)
{
	return (v-vf)/(vt-vf)*(dt-df) + df;
}

void draw_sphere(int nslice, int nstack)
{
	printf("[%10d]sphere: %d %d\n", ticks, nslice, nstack);
}

#include "demo.c"

int main()
{
	while (1) render();
	return 0;
}
