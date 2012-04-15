
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

long get_ticks()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

float lirp(float v, float vf, float vt, float df, float dt)
{
	return (v-vf)/(vt-vf)*(dt-df) + df;
}

void draw_sphere(int nslice, int nstack)
{
	glutWireSphere(1, nslice, nstack);
}

#include "demo.c"

void idle()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix(); {
		glTranslatef(0, 0, -10);
		render();
	} glPopMatrix();

	glutSwapBuffers();
}

void key_down(unsigned char k, int x, int y)
{
	switch (k) {
		case '\e':
			exit(0);
	}
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(win_w, win_h);
	glutCreateWindow("Demolino Runtime");
	glutIdleFunc(&idle);
	glutKeyboardFunc(&key_down);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, (float)win_w / (float)win_h, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	glutMainLoop();

	return 0;
}
