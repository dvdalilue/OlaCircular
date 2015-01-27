#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

void DrawCircle(float cx, float cy, float r, int num_segments) 
{ 
	glBegin(GL_LINE_LOOP); 
		for(int i = 0; i < num_segments; i++) 
		{ 
			float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);

			float x = r * cosf(theta);
			float y = r * sinf(theta);

			glVertex2f(x + cx, y + cy);
		} 
	glEnd(); 
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawCircle(0.0f, 0.0f, 0.5f, 25);

	glutSwapBuffers( ); 
}

int main (int argc, char** argv) {
	//Init GLUT & create windows
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutInitWindowPosition (500, 100);
	glutCreateWindow("Ola Circular");

	glutDisplayFunc(render);
	//glutReshapeFunc(changeViewport);

	glutMainLoop();
	return 0;
}