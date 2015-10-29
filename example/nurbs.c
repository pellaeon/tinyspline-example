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

/********************************************************
*                                                       *
* Modify these lines for experimenting.                 *
*                                                       *
********************************************************/
void setup()
{
    ts_bspline_new(3, 3, 13, TS_CLAMPED ,&spline);
    const float w = (float)(sqrt(2.f) / 2.f);
    
    spline.ctrlp[0] = 125.672848f;
    spline.ctrlp[1] = -138.790728f;
    spline.ctrlp[2] = 0.000000f;
    spline.ctrlp[3] = 125.672848f;
    spline.ctrlp[4] = -152.597846f;
    spline.ctrlp[5] = 0.000000f;
    spline.ctrlp[6] = 118.957119f;
    spline.ctrlp[7] = -163.790728f;
    spline.ctrlp[8] = 0.000000f;
    spline.ctrlp[9] = 110.672848f;
    spline.ctrlp[10] = -163.790728f;
    spline.ctrlp[11] = 0.000000f;
    spline.ctrlp[12] = 102.388577f;
    spline.ctrlp[13] = -163.790728f;
    spline.ctrlp[14] = 0.000000f;
    spline.ctrlp[15] = 95.672848f;
    spline.ctrlp[16] = -152.597846f;
    spline.ctrlp[17] = 0.000000f;
    spline.ctrlp[18] = 95.672848f;
    spline.ctrlp[19] = -138.790728f;
    spline.ctrlp[20] = 0.000000f;
    spline.ctrlp[21] = 95.672848f;
    spline.ctrlp[22] = -124.983609f;
    spline.ctrlp[23] = 0.000000f;
    spline.ctrlp[24] = 102.388577f;
    spline.ctrlp[25] = -113.790728f;
    spline.ctrlp[26] = 0.000000f;
    spline.ctrlp[27] = 110.672848f;
    spline.ctrlp[28] = -113.790728f;
    spline.ctrlp[29] = 0.000000f;
    spline.ctrlp[30] = 118.957119f;
    spline.ctrlp[31] = -113.790728f;
    spline.ctrlp[32] = 0.000000f;
    spline.ctrlp[33] = 125.672848f;
    spline.ctrlp[34] = -124.983609f;
    spline.ctrlp[35] = 0.000000f;
    spline.ctrlp[36] = 125.672848f;
    spline.ctrlp[37] = -138.790728f;
    spline.ctrlp[38] = 0.f;
    spline.knots[0] = 0.000000f;
    spline.knots[1] = 0.000000f;
    spline.knots[2] = 0.000000f;
    spline.knots[3] = 0.000000f;
    spline.knots[4] = 1.000000f;
    spline.knots[5] = 1.000000f;
    spline.knots[6] = 1.000000f;
    spline.knots[7] = 2.000000f;
    spline.knots[8] = 2.000000f;
    spline.knots[9] = 2.000000f;
    spline.knots[10] = 3.000000f;
    spline.knots[11] = 3.000000f;
    spline.knots[12] = 3.000000f;
    spline.knots[13] = 4.000000f;
    spline.knots[14] = 4.000000f;
    spline.knots[15] = 4.000000f;
    spline.knots[16] = 4.000000f;

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
    size_t i;
    glBegin(GL_POINTS);
      for (i = 0; i < spline.n_ctrlp; i++) 
         glVertex3fv(&spline.ctrlp[i * spline.dim]);
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
