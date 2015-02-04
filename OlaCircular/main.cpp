#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>
#include "noises.h"
#include "turbulance.h"

using namespace std;

#define DEF_floorGridScale	1.0
#define DEF_floorGridXSteps	10.0
#define DEF_floorGridZSteps	10.0

bool
	pause = false;

int 
	angle = 90,
	Xpos = 10,
	Ypos = 10,
	start = 1,
	enable = 1;
double 
	Zpos = 3.0,
	t = 0.0,
	factorTurb = 0.05;

float
	amplitud = 1.0,
	frecuencia = 1.0,
	speed = 0.1,
	deca = 1.0,
	amplitudR = 0.0,
	offsetR = 0.05,
	alturaRuido = 0.02;
	
	


GLfloat noise[21][21][2];
GLfloat ruido[21][21];
GLfloat ctlpoints[21][21][3];

GLUnurbsObj *theNurb;

void ejesCoordenada() {
	
	glLineWidth(2.5);
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0,10);
		glVertex2f(0,-10);
		glColor3f(0.0,0.0,1.0);
		glVertex2f(10,0);
		glVertex2f(-10,0);
	glEnd();

	glLineWidth(1.5);
	int i;
	glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
		for(i = -10; i <=10; i++){
			if (i!=0) {		
				if ((i%2)==0){	
					glVertex2f(i,0.4);
					glVertex2f(i,-0.4);

					glVertex2f(0.4,i);
					glVertex2f(-0.4,i);
				}else{
					glVertex2f(i,0.2);
					glVertex2f(i,-0.2);

					glVertex2f(0.2,i);
					glVertex2f(-0.2,i);

				}
			}
		}
		
	glEnd();

	glLineWidth(1.0);
}

void changeViewport(int w, int h) {
	
	float aspectratio;

	if (h==0)
		h=1;

	
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(30, (GLfloat) w/(GLfloat) h, 1.0, 200.0);
   glMatrixMode (GL_MODELVIEW);

}

float direction(int x, int y, int cx, int cy) {
	float l = sqrt(((x - cx)*(x - cx)) + ((y - cy)*(y - cy)));
	return l;
}



float decaimiento(int u, int v) {
	float r = direction(u,v,Xpos,Ypos);
	if (r == 0.0)
		return 1.0;
	return (r - r*deca + 1);
}

void init_surface() {
	int u, v;

    for (u = 0; u < 21; u++) {
		for (v = 0; v < 21; v++) {
			//Ruido
			noise[u][v][0] = ((GLfloat)u - 10.0f)*amplitudR + offsetR;
			noise[u][v][1] = ((GLfloat)v - 10.0f)*amplitudR + offsetR;
			//Posicion de los puntos de control
			ctlpoints[u][v][0] = ((GLfloat)u - 10.0f); //coord. X
			ctlpoints[u][v][1] = ((GLfloat)v - 10.0f); //coord. Z
			ctlpoints[u][v][2] = enable*amplitud*sin(direction(u,v,Xpos,Ypos)*frecuencia - t)/decaimiento(u, v) + (noiseFunction(noise[u][v]));
		
			ruido[u][v] = alturaRuido*0.005*turbulence(noise[u][v][0], noise[u][v][1],factorTurb);
		}
    }
} 

void init(){

	//GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    //GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    //GLfloat mat_shininess[] = { 100.0 };

    //glClearColor (0.0, 0.0, 0.0, 1.0);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);		

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	init_surface();

	theNurb = gluNewNurbsRenderer();
	gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 15.0); //25.0
	gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

void Keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
	case 27:             
		exit (0);
		break;
	case '1':
		pause = !pause;
		break;
	case '3':
		enable = !enable;
		break;
	case 'j':
		alturaRuido +=0.01;
		break;
	case 'm':
		alturaRuido -=0.01;
		break;
	case 't':
		factorTurb +=1;
		break;
	case 'y':
		factorTurb -=1;
		break;
	case 'g':
		amplitudR += 0.01;
		break;
	case 'b':
		amplitudR -= 0.01;
		break;
	case 'h':
		offsetR += 0.01;
		break;
	case 'n':
		offsetR -= 0.01;
		break;
	case 'q':
		Xpos += 1;
		break;
	case 'w':
		Xpos -= 1;
		break;
	case 'e':
		Ypos += 1;
		break;
	case 'r':
		Ypos -= 1;
		break;
	case 'a':
		amplitud += 0.1;
		break;
	case 'z':
		amplitud -= 0.1;
		break;
	case 's':
		frecuencia += 0.1;
		break;
	case 'x':
		frecuencia -= 0.1;
		break;
	case 'd':
		speed += 0.1;
		break;
	case 'c':
		if (speed > 0.2)
			speed -= 0.1; 
		break;
	case 'f':
		if (deca > 0.0)
			deca += 0.01;//dice que aumenta el decaimiento de la ola creo que no es sea -= 0.01;
		break;
	case 'v':
		if (deca < 1.0)
			deca -= 0.01;//dice que disminuye el decaimiento de la ola creo que no es sea += 0.01;
		break;
  }
}

GLfloat * knotVector(int k) {
	static GLfloat r[25];

	for (int i = 0; i < k; ++i) {
		r[i] = 0.0f;
	}

	for (int i = k; i < (25-k); ++i) {
		r[i] = float(i-3)/18.0;
	}

	for (int i = (25-k); i < 25; ++i) {
		r[i] = 1.0f;
	}

	return r;
}

void curva() {
	
	GLfloat *knots;

	knots = knotVector(4);

	gluBeginSurface(theNurb);

		gluNurbsSurface(theNurb, 
						25,	knots,
						25, knots,
						21 * 3,
						3,
						&ctlpoints[0][0][0],
						4, 4,
						GL_MAP2_VERTEX_3);
	/*

		No cambien los numeros de la funcion,
		solo deben de poner los nombres de las variables correspondiente.
		
	*/


		gluEndSurface(theNurb);
	
	
	/* Muestra los puntos de control */
	
	/*	int i,j;
		glPointSize(5.0);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		for (i = 0; i <21; i++) {
			for (j = 0; j < 21; j++) {
	            glVertex3f(ctlpoints[i][j][0], 	ctlpoints[i][j][1], ctlpoints[i][j][2]);
			}
		}
		glEnd();*/
		glEnable(GL_LIGHTING);
}

void render(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	GLfloat zExtent, xExtent, xLocal, zLocal;
    int loopX, loopZ;

	glLoadIdentity ();                       
	gluLookAt (25.0, 12.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	

	// Luz y material

	GLfloat mat_ambient[] = { 0.28, 0.75, 0.82, 1.0 };
	GLfloat mat_diffuse[] = { 0.7372, 0.8313, 0.9019, 0.95 };
	GLfloat mat_specular[] = { 0.803, 1.0, 1.0, 0.85 };
	GLfloat mat_shininess[] =  { 42.0 };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	

    GLfloat light_ambient[] = { 0.0, 0.0, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat light_position[] = { -10.0, 5.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);   

	// Render Grid 

	//glDisable(GL_LIGHTING);
	//glLineWidth(1.0);
	//glPushMatrix();
	//	glRotatef(angle,1.0,0.0,0.0);
	//	glColor3f( 0.0, 0.7, 0.7 );
	//	glBegin( GL_LINES );
	//		zExtent = DEF_floorGridScale * DEF_floorGridZSteps;
	//		for(loopX = -DEF_floorGridXSteps; loopX <= DEF_floorGridXSteps; loopX++ )
	//		{
	//			xLocal = DEF_floorGridScale * loopX;
	//			glVertex3f( xLocal, -zExtent, 0.0 );
	//			glVertex3f( xLocal, zExtent,  0.0 );
	//		}
	//		xExtent = DEF_floorGridScale * DEF_floorGridXSteps;
	//	    for(loopZ = -DEF_floorGridZSteps; loopZ <= DEF_floorGridZSteps; loopZ++ )
	//		{
	//			zLocal = DEF_floorGridScale * loopZ;
	//			glVertex3f( -xExtent, zLocal, 0.0 );
	//			glVertex3f(  xExtent, zLocal, 0.0 );
	//		}
	//	glEnd();
	//	ejesCoordenada();
 //   glPopMatrix();

	//glEnable(GL_LIGHTING);

	// Fin Grid
	


	//Suaviza las lineas
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_LINE_SMOOTH );	

	
	glPushMatrix();
		glRotatef(angle,-1.0,0.0,0.0);
		//glTranslated(0.0,0.0,Zpos);
		curva();
	
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_LINE_SMOOTH);

	glutSwapBuffers();
}

void animacion(int value) {
	init_surface();
	if (!pause)
		t += speed;
	glutTimerFunc(1,animacion,1);
    glutPostRedisplay();
	
}

int main (int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(960,540);

	glutInitWindowPosition (500, 100);

	glutCreateWindow("Test Opengl");

	init ();

	glutReshapeFunc(changeViewport);
	glutDisplayFunc(render);
	glutKeyboardFunc(Keyboard);
		
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}
	
	glutTimerFunc(1,animacion,1);
	glutMainLoop();
	return 0;

}
