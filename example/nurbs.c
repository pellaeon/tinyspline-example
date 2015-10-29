#include "tinyspline.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h> 

tsBSpline spline;
GLUnurbsObj *theNurb;
float u = 0.f;

unsigned int n_ctrlp;
FILE* file_p;

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
	printf("asdasd");
	file_p = fopen("input.txt", "r");
	fscanf(file_p, "n_ctrlp: %u\n", &n_ctrlp);
	printf("====: %u\n", n_ctrlp);

    ts_bspline_new(3, 3, n_ctrlp, TS_CLAMPED ,&spline);
    const float w = (float)(sqrt(2.f) / 2.f);
    
	float x,y,z;
	int i=0;
	fpos_t orig;
	while( !fgetpos(file_p, &orig) && fscanf(file_p, "ctrlp: %f %f %f\n", &x, &y, &z) == 3 ) {
		spline.ctrlp[i] = x;
		i++;
		spline.ctrlp[i] = y;
		i++;
		spline.ctrlp[i] = z;
		i++;
	}
	fsetpos(file_p, &orig);

	if ( i != n_ctrlp*3 ) {
		printf("Some control points might be missing\n");
	}

	int j=0;
	float knot_val;
	while( fscanf(file_p, "knot: %f\n", &knot_val) == 1 ) {
		spline.knots[j] = knot_val;
		j++;
	}
}

void tear_down()
{
    ts_bspline_free(&spline);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // draw spline
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);
    gluBeginCurve(theNurb);
        gluNurbsCurve(
            theNurb, 
            spline.n_knots, 
            spline.knots, 
            spline.dim, 
            spline.ctrlp, 
            spline.order, 
            GL_MAP1_VERTEX_3
        );
    gluEndCurve(theNurb);

    // draw control points
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
	float j=0;
    size_t i;
    glBegin(GL_POINTS);
      for (i = 0; i < spline.n_ctrlp; i++) {
		  j+=1.0/n_ctrlp;
		  glColor3f(0.0, j, 0.0);
         glVertex3fv(&spline.ctrlp[i * spline.dim]);
	  }
    glEnd();
    
    // draw evaluation
    glColor3f(0.0, 0.0, 1.0);
    glPointSize(5.0);
    tsDeBoorNet net;
    ts_bspline_evaluate(&spline, u, &net);
    glBegin(GL_POINTS);
        glVertex3fv(net.result);
    glEnd();
    ts_deboornet_free(&net);
    
    u += 0.001;
    if (u > 4.f) {
        u = 0.f;
    }
    
    glutSwapBuffers();
    glutPostRedisplay();
}




/********************************************************
*                                                       *
* Framework                                             *
*                                                       *
********************************************************/
void nurbsError(GLenum errorCode)
{
   const GLubyte *estring;

   estring = gluErrorString(errorCode);
   fprintf (stderr, "Nurbs Error: %s\n", estring);
   exit (0);
}
   
void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty (theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
    gluNurbsCallback(theNurb, GLU_ERROR, (GLvoid (*)()) nurbsError);
    setup();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 100.0, 200.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Translate Matrix for 30x50mm-spline.dxf
   glTranslatef (-110.0, 135.0, -110.0);
}

int main(int argc, char** argv)
{
	printf("asdasd");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMainLoop();
    tear_down();
    return 0; 
}
