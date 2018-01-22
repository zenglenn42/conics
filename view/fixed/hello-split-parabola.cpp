//-----------------------------------------------------------------------------
// Render a 2D and 3D parabola in adjacent sub-windows with some lable text, 
// maintaining a desired overall aspect ratio in the parent window.
//
// TODO: Get top cone to illuminate.
//       Get transparency working for intersection.
//
// Copyright © Glenn Streiff 2017
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include <math.h>
#include "shared/gltools.h"
#include "shared/glframe.h"

GLvoid *fontStyle = GLUT_BITMAP_HELVETICA_18;

const GLfloat TWO_2_ONE_RATIO = 2.0;
GLfloat gAspectRatio = TWO_2_ONE_RATIO;

GLfloat gBezierControlPoints[4][3] = {
    {-2.0, 2.121, 0.0}, {-0.667, -1.650, 0.0}, {0.667, -1.650, 0.0}, {2.0, 2.121, 0.0}
};

GLfloat gFocus[1][3] = {
    {0.0, -0.354, 0.0}
};

GLuint mainWin, sub1Win, sub2Win;
GLfloat gSubGapPercent = 0.04;
const GLuint msecDelay = 10;    // Delay for timer-based animation.
GLFrame frameCamera;
GLuint gSubWidth = 300;
GLfloat gSubGap = gSubWidth * gSubGapPercent; // TODO: Add aspect ratio factor.
GLuint gWinPixelWidth = gSubWidth * 2.0 + gSubGap * 3.0;
GLuint gWinPixelHeight = gSubWidth / gAspectRatio + gSubGap * 2.0;
GLuint gSubHeight = gWinPixelHeight - 2 * gSubGap;

void DrawStr(GLfloat x, GLfloat y, std::string str, bool center);
void DrawParabola();
void DrawCones(GLboolean drawBasePlane);
void ResetViewport();

//-----------------------------------------------------------------------------
// Draw bitmap font string
//
// Most of the configuration fu for rendering simple 2D text comes from
// in a graphics context comes from:
//
// https://stackoverflow.com/questions/9430852/glutbitmapcharacter-positions-text-wrong
//-----------------------------------------------------------------------------
void DrawStr(GLfloat x, GLfloat y, std::string str, bool center = false) {

    glMatrixMode( GL_PROJECTION ) ;
    glPushMatrix() ; // save
    glLoadIdentity();// and clear
    glMatrixMode( GL_MODELVIEW ) ;
    glPushMatrix() ;
    glLoadIdentity() ;
    glDisable(GL_LIGHTING | GL_DEPTH_TEST);

    GLfloat strPixelWidth = 0.0;

    if (center) {
        for (int i = 0; i < str.length(); i++) {
            strPixelWidth += glutBitmapWidth(fontStyle, str[i]);
        }
        GLfloat offset = (strPixelWidth / (1.0 * gWinPixelWidth)) / 2.0;
        glRasterPos2f(x - offset, y);
    } else {
        glRasterPos2f(x, y);	// NB: center is at (0.0, 0.0)
    }

    // Set text color.
    glColor4f(1.0f, 1.0, 1.0f, 1.0f);

    for(int i = 0; i < str.length(); i++) {
        glutBitmapCharacter(fontStyle, str[i]);
    }
    glEnable(GL_LIGHTING | GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

//-----------------------------------------------------------------------------
// This function resets the viewport.
//-----------------------------------------------------------------------------
void ResetViewport()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0, 2.0, -2.0, 2.0, 0.5, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//-----------------------------------------------------------------------------
// This function initializes the rendering context.
//-----------------------------------------------------------------------------
void SetupRC() {

    // Bluish background
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

    //glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &gBezierControlPoints[0][0]);
    //glEnable(GL_MAP1_VERTEX_3);

    // Avoid jaggy lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
}

//-----------------------------------------------------------------------------
// Draw teapot
//-----------------------------------------------------------------------------
void DrawScene() {
    glPushMatrix();
        // Gray base-plane.
        glBegin(GL_QUADS);
            glColor3f(0.7, 0.7, 0.7);
            glVertex3f(2.0, 0.0, 2.0);
            glVertex3f(2.0, 0.0, -2.0);
            glVertex3f(-2.0, 0.0, -2.0);
            glVertex3f(-2.0, 0.0, 2.0);
        glEnd();

        // White base-plane axes.
        glBegin(GL_LINES);
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(0.0, 0.0, 2.0);
            glVertex3f(0.0, 0.0, -2.0);
            glVertex3f(-2.0, 0.0, 0.0);
            glVertex3f(2.0, 0.0, 0.0);
        glEnd();
    glPopMatrix();

    // Intersecting plane.
    glPushMatrix();
        glColor3f(1.0, 0.0, 0.0);
        glTranslatef(0.0, 1.5, 0.0);
        glRotatef(45.0, 0.0, 0.0, 1.0);
        glBegin(GL_QUADS);
            glColor3f(1.0, 0.0, 0.0);
            glVertex3f(1.5, 0.0, 1.5);
            glVertex3f(1.5, 0.0, -1.5);
            glVertex3f(-1.5, 0.0, -1.5);
            glVertex3f(-1.5, 0.0, 1.5);
        glEnd();
    glPopMatrix();

    glPushMatrix();
        // Bottom cone.
        glColor3f(1.0, 0.0, 1.0);
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        glutSolidCone(1.0, 1.0, 20, 20);
    glPopMatrix();
    glPushMatrix();
        // Top cone.
        glTranslatef(0.0, 2.0, 0.0);
        glColor3f(0.0, 1.0, 1.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        glutSolidCone(1.0, 1.0, 20, 20);
    glPopMatrix();

}

//-----------------------------------------------------------------------------
// Called to draw scene.
//-----------------------------------------------------------------------------
void RenderScene(void) {
    // Clear the window with current clearing color.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

#if 0
    GLfloat strPixelHeight = glutBitmapWidth(fontStyle, 'X');
    GLfloat vNudge = 1.5 * (gSubGap - strPixelHeight) / gWinPixelHeight;
    std::cout << "gSubGap = " << gSubGap << std::endl;
    std::cout << "vNudge = " << vNudge << std::endl;
#endif
    GLfloat lNudge = (gSubGap * 3.0 / gWinPixelWidth) * .7 ;
    GLfloat rNudge = lNudge * .7;

    DrawStr(-1.0 + lNudge, 0.9, "2D Projection");
    DrawStr( 0.0 + rNudge, 0.9, "3D Perspective");
    glutSwapBuffers();
} // RenderScene

//-----------------------------------------------------------------------------
// Called to draw sub-scene.
//-----------------------------------------------------------------------------
void RenderSubScene1(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    DrawParabola();
    glutSwapBuffers();
}

//-----------------------------------------------------------------------------
void DrawParabola() {
   int i;

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &gBezierControlPoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    // Coordinate axes.

    glColor4f(0.3, 0.3, 0.3, 1.0f);       // gray text
    glBegin(GL_LINES);
        // vertical axis
        glVertex2f(0.0,  1.0);
        glVertex2f(0.0, -1.0);

        // horizontal axis
        glVertex2f(-1.0,  0.0);
        glVertex2f( 1.0,  0.0);
    glEnd();

   // Render Bezier curve.

   glColor4f(1.0, 1.0, 1.0, 1.0f);       // white text
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

   // Render parabola focus.

   glPointSize(7.0);
   glEnable(GL_POINT_SMOOTH);	// round points
   glBegin(GL_POINTS);
         glVertex3fv(&gFocus[0][0]);
   glEnd();
}

void DrawCones(GLboolean drawBasePlane)
{
    GLfloat refSize = 200.0f; // Dimension of visible universe.
    GLfloat maxCoord = refSize / 2.0;
    GLfloat minCoord = -maxCoord;

    GLfloat yOffset = 0.0f;
    if (drawBasePlane)
    {
        // Draw plane that the objects rest on
        glColor3f(0.0f, 0.0f, 0.90f); // Blue
        glNormal3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_QUADS);
            glVertex3f(minCoord, yOffset, minCoord);
            glVertex3f(minCoord, yOffset, maxCoord);
            glVertex3f(maxCoord, yOffset, maxCoord);
            glVertex3f(maxCoord, yOffset, minCoord);
        glEnd();
    }

    // Draw double cone

    GLfloat coneRefSize = refSize / 4.0f;

    GLfloat coneRadius = coneRefSize;
    GLfloat coneHeight = coneRefSize;
    GLint   coneSlices = coneRefSize;
    GLint   coneStacks = coneRefSize;

    glColor3f(1.0f, 1.0f, 0.0f);
    glPushMatrix();
        // bottom cone
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.0f);
        glutSolidCone(coneRadius, coneHeight, coneSlices, coneStacks);
    glPopMatrix();

    glPushMatrix();
        // top cone
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -100.0f);
        glutSolidCone(coneRadius, coneHeight, coneSlices, coneStacks);
    glPopMatrix();
}

//-----------------------------------------------------------------------------
// Called to draw sub-scene.
//-----------------------------------------------------------------------------
void RenderSubScene2(void) {
    //ResetViewport();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glEnable(GL_LIGHTING);
    GLfloat lightPos[] = {5.0f,5.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);

    // Allow surfaces to reflect color gradations
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        //DrawCones(GL_FALSE);
        gluPerspective(30, 1.0, 3.0, 50.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            //gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            gluLookAt(0.0, 3.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            DrawScene();
        glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();
    glutSwapBuffers();
}

//-----------------------------------------------------------------------------
// Respond to arrow keys by moving the camera frame of reference.
//-----------------------------------------------------------------------------
void SpecialKeys(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_UP:
        case GLUT_KEY_DOWN:
        case GLUT_KEY_LEFT:
        case GLUT_KEY_RIGHT:
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Called by GLUT library when idle (window not being resized or moved).
//-----------------------------------------------------------------------------
void TimerFunction(int timerParam) {
    // Redraw the scene with new coordinates
    glutPostRedisplay();

    // Reset one-shot timer
    GLint unusedTimerArg = 1;
    glutTimerFunc(msecDelay, TimerFunction, unusedTimerArg);
}

//-----------------------------------------------------------------------------
// Called by GLUT library when user changes windows size.
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
    
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Adjust the dimensions of sub-windows to fit within the AR-constrained
    // parent window.

    gSubGap = gWinPixelWidth * gSubGapPercent;
    gSubWidth = (gWinPixelWidth - gSubGap * 3) / 2.0;
    gSubHeight = gWinPixelHeight - gSubGap * 2;

    glutSetWindow(sub1Win);
    glutPositionWindow(gSubGap, gSubGap);
    glutReshapeWindow(gSubWidth, gSubHeight);

    glutSetWindow(sub2Win);
    glutPositionWindow(gSubGap + gSubWidth + gSubGap, gSubGap);
    glutReshapeWindow(gSubWidth, gSubHeight);

    glutSetWindow(mainWin);
}

//-----------------------------------------------------------------------------
// Main program entry point.
//-----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(gSubWidth * 2 + gSubGap * 3, gSubHeight * 1 + gSubGap * 2);

    mainWin = glutCreateWindow("Hello, parabolas");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

    sub1Win = glutCreateSubWindow(mainWin, gSubGap, gSubGap, gSubWidth, gSubHeight);
    glutDisplayFunc(RenderSubScene1);

    sub2Win = glutCreateSubWindow(mainWin, gSubGap + gSubWidth + gSubGap, gSubGap, gSubWidth, gSubHeight);
    glutDisplayFunc(RenderSubScene2);

    glutSpecialFunc(SpecialKeys);
    glutTimerFunc(msecDelay, TimerFunction, 1);

    SetupRC();
    glutMainLoop();

    return 0;
}
