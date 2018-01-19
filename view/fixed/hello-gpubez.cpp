//-----------------------------------------------------------------------------
// Code for gpu-accelerated rendering of a 3rd order Bezier curve by specifying
// four control points.
//
// Leverages GPU maps and evaluators.  
//
// Maps hold the control points and express what values will be output by the 
// downstream evaluator.  They also express the extent of the curve to be
// rendered.
//
// Evaluators define curves based upon Bernstein polynomials and return points 
// along that curve for a given input parameter.
//
// See: https://msdn.microsoft.com/en-us/library/windows/desktop/ee872051(v=vs.85).aspx
//
// Courtesy: http://glprogramming.com/red/chapter12.html
// 
// Interestingly, this is not considered a modern approach according to:
// https://stackoverflow.com/questions/7953189/curves-representable-in-opengl#
// but it at least leverages the GPU.  I'm just trying to mock-up
// a demo so I'm content with incremental progress of getting (mostly) off the 
// CPU.
//-----------------------------------------------------------------------------
#include "shared/gltools.h"

GLfloat ctrlpoints[4][3] = {
        { -4.0, -4.0, 0.0}, { -2.0, 4.0, 0.0}, 
        {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);

   //          +-- Triggers generation of glVertex3 commands by evaluator.
   //          |
   //          |              +-- min parametric value, i think ;-)
   //          |              |
   //          |              |    +-- max parametric value
   //          |              |    |
   //          |              |    |   +-- stride, # of values between ctrlpoints.
   //          |              |    |   |   So a 3d control point will have x, y, z
   //          |              |    |   |   values between control point boundaries,
   //          |              |    |   |   or a stried of 3
   //          |              |    |   |
   //          |              |    |   |  +-- number of control points
   //          |              |    |   |  |
   //          |              |    |   |  |   +-- ptr to array of control points
   //          |              |    |   |  |   |
   glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
   glEnable(GL_MAP1_VERTEX_3);
}

void display(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);

   // Render Bezier curve.

   glBegin(GL_LINE_STRIP);
      //
      // Subdivide the curve into steps between the min and max
      // parametric values, evaluating the curve as we go and
      // issuing corresponding glVertex3 calls.
      //
      // Within a GL_LINE_STRIP context, effect is to join points
      // along the curve by straight lines, approximating the
      // overall curve.
      //
      for (i = 0; i <= 30; i++) 
         glEvalCoord1f((GLfloat) i/30.0);
   glEnd();

   // Render control points.

   glPointSize(5.0);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&ctrlpoints[i][0]);
   glEnd();
   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w, 
               5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
   else
      glOrtho(-5.0*(GLfloat)w/(GLfloat)h, 
               5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMainLoop();
   return 0;
}
