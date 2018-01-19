//-----------------------------------------------------------------------------
// Render a fast parabola (using OpenGL primitives that leverage the GPU).
//
// Maintain the overall aspect ratio of the enclosing GLUT window when 
// resizing.
//-----------------------------------------------------------------------------

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include "shared/gltools.h"
#include "shared/glframe.h"

GLvoid *fontStyle = GLUT_BITMAP_HELVETICA_18;

const GLfloat GOLDEN_RATIO = 1.618;
const GLfloat HDTV_RATIO = 16.0 / 9.0;
const GLfloat TV_RATIO = 4.0 / 3.0;
const GLfloat TWO_2_ONE_RATIO = 2.0;
const GLfloat ONE_2_ONE_RATIO = 1.0;
GLfloat gAspectRatio = ONE_2_ONE_RATIO;

std::string gWinText = "parabola(t) = (1/sqrt(2)) * (t^2 - 1)";
const std::string gWinTitle = "hello, parabola";

GLuint gWinPixelWidth  = 400;
GLuint gWinPixelHeight = (GLfloat) gWinPixelWidth / gAspectRatio;

GLfloat gBezierControlPoints[4][3] = {
    {-2.0, 2.121, 0.0}, {-0.667, -1.650, 0.0}, {0.667, -1.650, 0.0}, {2.0, 2.121, 0.0}
};

GLfloat gFocus[1][3] = {
    {0.0, -0.354, 0.0}
};

void DrawModel();
void DrawBitmapStr(GLuint x, GLuint y, std::string str, bool center);
void DrawParabola();

//-----------------------------------------------------------------------------
// This function initializes the rendering context.
//-----------------------------------------------------------------------------
void SetupRC() {
    // Bluish background.
    glClearColor(0.0f, 0.3f, 0.80f, 1.0f);

    // Configure a map to support downstream evaluation of
    // a 3rd order Bezier curve.

    //          +-- Triggers generation of glVertex3 commands by evaluator.
    //          |
    //          |              +-- min parametric value, i think ;-)
    //          |              |
    //          |              |    +-- max parametric value
    //          |              |    |
    //          |              |    |   +-- stride, # of values between gBezierControlPoints.
    //          |              |    |   |   So a 3d control point will have x, y, z
    //          |              |    |   |   values between control point boundaries,
    //          |              |    |   |   or a stried of 3
    //          |              |    |   |
    //          |              |    |   |  +-- number of control points
    //          |              |    |   |  |
    //          |              |    |   |  |   +-- ptr to array of control points
    //          |              |    |   |  |   |

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &gBezierControlPoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
}

//-----------------------------------------------------------------------------
// Draw bitmap font string
//
// Most of the configuration fu for rendering simple 2D text comes from
// in a graphics context comes from:
//
// https://stackoverflow.com/questions/9430852/glutbitmapcharacter-positions-text-wrong
//-----------------------------------------------------------------------------
void DrawBitmapStr(GLuint x, GLuint y, std::string str, bool center = true) {

    glMatrixMode(GL_PROJECTION) ;
    glPushMatrix() ; // save
    glLoadIdentity();// and clear
    glMatrixMode(GL_MODELVIEW) ;
    glPushMatrix() ;
    glLoadIdentity() ;
    glDisable(GL_LIGHTING | GL_DEPTH_TEST);

    if (center) {
        GLfloat strPixelWidth = 0.0;
        for (int i = 0; i < str.length(); i++) {
            strPixelWidth += glutBitmapWidth(fontStyle, str[i]);
        }
        GLfloat leftPixelMargin = (gWinPixelWidth - strPixelWidth) / 2.0;
        GLfloat ndcLPM = leftPixelMargin / gWinPixelWidth;
        GLfloat ndcOffset = 2.0 * ndcLPM; // Multiple by 2.0 b/c we range from -1.0 to 1.0.
        glRasterPos2f(-1.0 + ndcOffset, y);
    } else {
        // Center at (x, y) = (0, 0).
        glRasterPos2f(x, y);
    }

    glColor4f(1.0f, 1.0, 1.0f, 1.0f);       // white text
    for(int i = 0; i < str.length(); i++) {        
        glutBitmapCharacter(fontStyle, str[i]);
    }
    glEnable(GL_LIGHTING | GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void DrawModel() {
    DrawBitmapStr(0, 0, gWinText);

    // Coordinate axes.
    glColor4f(0.7, 0.7, 0.7, 1.0f);       // gray text
    glBegin(GL_LINES);
	// vertical axis
        glVertex2f(0.0,  1.0);
        glVertex2f(0.0, -1.0);

	// horizontal axis
        glVertex2f(-1.0,  0.0);
        glVertex2f( 1.0,  0.0);
    glEnd();

    DrawParabola();
}

void DrawParabola() {
   int i;

   //glClear(GL_COLOR_BUFFER_BIT);
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

   glPointSize(5.0);

   // Render control points.

   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++) 
         glVertex3fv(&gBezierControlPoints[i][0]);
   glEnd();

   // Render parabola focus.
   glBegin(GL_POINTS);
         glVertex3fv(&gFocus[0][0]);
   glEnd();
}

//-----------------------------------------------------------------------------
// Called to draw scene.
//-----------------------------------------------------------------------------
void RenderScene(void) {

    // Clear the window with current clearing color.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glPushMatrix();
        DrawModel();
    glPopMatrix();

    glutSwapBuffers();
} // RenderScene

//-----------------------------------------------------------------------------
// Respond to arrow keys by moving the camera frame of reference.
//-----------------------------------------------------------------------------
void SpecialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT: {
            if (gAspectRatio == GOLDEN_RATIO) {
               gAspectRatio = HDTV_RATIO;
            } else if (gAspectRatio == HDTV_RATIO) {
               gAspectRatio = TV_RATIO;
            } else if (gAspectRatio == TV_RATIO) {
               gAspectRatio = TWO_2_ONE_RATIO;
            } else if (gAspectRatio == TWO_2_ONE_RATIO) {
               gAspectRatio = ONE_2_ONE_RATIO;
            } else if (gAspectRatio == ONE_2_ONE_RATIO) {
               gAspectRatio = GOLDEN_RATIO;
            }
            gWinPixelHeight = gWinPixelWidth / gAspectRatio;
            glutReshapeWindow(gWinPixelWidth, gWinPixelHeight);
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Called by GLUT library when user changes window size.
//-----------------------------------------------------------------------------
void ChangeSize(int w, int h) {   
    GLint screenPixelHeight = glutGet(GLUT_SCREEN_HEIGHT);

    gWinPixelWidth  = w;
    gWinPixelHeight = w / gAspectRatio;

    if (gWinPixelHeight > screenPixelHeight) {
        // Clamp the window height to something viewable on this display.
        gWinPixelHeight = h;
        gWinPixelWidth  = h * gAspectRatio;
    }

    // Set (external) window dimensions to maintain desired aspect ratio.
    glutReshapeWindow(gWinPixelWidth, gWinPixelHeight);

    // Set (internal) viewport to window dimensions.
    glViewport(0, 0, gWinPixelWidth, gWinPixelHeight);

    // Reset the coordinate system before modifying.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    DrawModel();
}

//-----------------------------------------------------------------------------
// Main program entry point.
//-----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(gWinPixelWidth, gWinPixelHeight);

    glutCreateWindow(gWinTitle.c_str());
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);

    SetupRC();
    glutMainLoop();

    return 0;
}
