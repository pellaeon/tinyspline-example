#include "tinyspline.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

tsBSpline spline;
GLUnurbsObj *theNurb;
int drawBeziers = 0; // 0 - bspline, 1 - beziers

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
    ts_bspline_new(
        3,      // degree of spline
        3,      // dimension of each point
        7,      // number of control points
        TS_CLAMPED,// used to hit first and last control point
        &spline // the spline to setup
    );
    
    // Setup control points.
    spline.ctrlp[0] = -1.75;
    spline.ctrlp[1] = -1.0;
    spline.ctrlp[2] = 0.0;
    spline.ctrlp[3] = -1.5;
    spline.ctrlp[4] = -0.5;
    spline.ctrlp[5] = 0.0;
    spline.ctrlp[6] = -1.5;
    spline.ctrlp[7] = 0.0;
    spline.ctrlp[8] = 0.0;
    spline.ctrlp[9] = -1.25;
    spline.ctrlp[10] = 0.5;
    spline.ctrlp[11] = 0.0;
    spline.ctrlp[12] = -0.75;
    spline.ctrlp[13] = 0.75;
    spline.ctrlp[14] = 0.0;
    spline.ctrlp[15] = 0.0;
    spline.ctrlp[16] = 0.5;
    spline.ctrlp[17] = 0.0;
    spline.ctrlp[18] = 0.5;
    spline.ctrlp[19] = 0.0;
    spline.ctrlp[20] = 0.0;
}

void tear_down()
{
    ts_bspline_free(&spline);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tsBSpline draw;
    if (drawBeziers)
        ts_bspline_to_beziers(&spline, &draw);
    else
        ts_bspline_copy(&spline, &draw);

    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(3);
    gluBeginCurve(theNurb);
        gluNurbsCurve(
            theNurb,
            draw.n_knots,
            draw.knots,
            draw.dim,
            draw.ctrlp,
            draw.order,
            GL_MAP1_VERTEX_3
        );
    gluEndCurve(theNurb);
    ts_bspline_free(&draw);

    // draw control points
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    size_t i;
    glBegin(GL_POINTS);
      for (i = 0; i < spline.n_ctrlp; i++) 
         glVertex3fv(&spline.ctrlp[i * spline.dim]);
    glEnd();
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void keyPressed (unsigned char key, int x, int y) {
    if (key == ' ') {
        if (drawBeziers) {
            printf("draw bspline\n");
            drawBeziers = 0;
        } else {
            printf("draw beziers\n");
            drawBeziers = 1;
        }
    }
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
   gluPerspective (45.0, (GLdouble)w/(GLdouble)h, 3.0, 8.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef (0.0, 0.0, -5.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
    tear_down();
    return 0; 
}
