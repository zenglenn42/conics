//-----------------------------------------------------------------------------
// Render stuff in two adjacent sub-windows with some lable text.
//
// Original code courtesy JH Kim:
// http://study.marearts.com/2012/01/sample-source-to-make-subwindow-in.html
//
// Copyright © Glenn Streiff 2017 (derivative work)
//
// TODO:
//
//    Add some nice sub-window callback fu outlined in:
//    http://www.lighthouse3d.com/tutorials/glut-tutorial/subwindows/
//-----------------------------------------------------------------------------
#include <string>
#include "shared/gltools.h"
#include "shared/glframe.h"
#include <math.h>

GLuint mainWin, sub1Win, sub2Win;
GLuint subGap = 30;
const GLuint msecDelay = 10;    // Delay for timer-based animation.
GLFrame frameCamera;
GLuint subWidth = 300;
GLuint width = subWidth * 2.0 + subGap * 3.0;
GLuint height = width / 2.0;
GLuint subHeight = height - 2 * subGap;
GLvoid *fontStyle = GLUT_BITMAP_HELVETICA_18;

void DrawStr(GLfloat x, GLfloat y, std::string str, bool center);
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
        GLfloat offset = (strPixelWidth / (1.0 * width)) / 2.0;
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

    // Avoid jaggy lines
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
}

//-----------------------------------------------------------------------------
// Draw teapot
//-----------------------------------------------------------------------------
void DrawScene() {
    glColor3f(0.7, 0.7, 0.7);
    glPushMatrix();

    glBegin(GL_QUADS);
    glVertex3f(2.0, 0.0, 2.0);
    glVertex3f(2.0, 0.0, -2.0);
    glVertex3f(-2.0, 0.0, -2.0);
    glVertex3f(-2.0, 0.0, 2.0);
    glEnd();

    glPopMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glTranslatef(0.0, 0.0, -0.5);
    glutWireTeapot(1.0);
    glPopMatrix();
}

//-----------------------------------------------------------------------------
// Called to draw scene.
//-----------------------------------------------------------------------------
void RenderScene(void) {
    // Clear the window with current clearing color.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    GLfloat strPixelHeight = glutBitmapWidth(fontStyle, 'X');
    GLfloat vNudge = 2.75 * strPixelHeight / height;

    GLfloat lNudge = (subGap * 3.0 / width) * .7 ;
    GLfloat rNudge = lNudge * .7;
    DrawStr(-1.0 + lNudge, 1.0 - vNudge, "Side");
    DrawStr( 0.0 + rNudge, 1.0 - vNudge, "Perspective");
    glutSwapBuffers();
} // RenderScene

//-----------------------------------------------------------------------------
// Called to draw sub-scene.
//-----------------------------------------------------------------------------
void RenderSubScene1(void) {
    ResetViewport();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glPushMatrix();
    gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    DrawScene();
    glPopMatrix();
    glutSwapBuffers();
}

//-----------------------------------------------------------------------------
// Called to draw sub-scene.
//-----------------------------------------------------------------------------
void RenderSubScene2(void) {
    ResetViewport();

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluPerspective(30, 1.0, 3.0, 50.0);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
            gluLookAt(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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
    if (key == GLUT_KEY_UP)
        frameCamera.MoveForward(0.1f);

    if (key == GLUT_KEY_DOWN)
        frameCamera.MoveForward(-0.1f);

    if (key == GLUT_KEY_LEFT)
        frameCamera.RotateLocalY(0.1f);

    if (key == GLUT_KEY_RIGHT)
        frameCamera.RotateLocalY(-0.1f);

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
    GLfloat fAspect;
    width = w;
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    h = (h == 0) ? 1 : h;

    glViewport(0, 0, w, h); 
    
    fAspect = (GLfloat)w / (GLfloat)h;

    // Reset the coordinate system before modifying.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    GLfloat subWidth  = (w - subGap * 3) / 2.0;
    GLfloat subHeight = h - subGap * 2;

    glutSetWindow(sub1Win);
    glutPositionWindow(subGap, subGap);
    glutReshapeWindow(subWidth, subHeight);

    glutSetWindow(sub2Win);
    glutPositionWindow(subGap + subWidth + subGap, subGap);
    glutReshapeWindow(subWidth, subHeight);

    glutSetWindow(mainWin);
}

//-----------------------------------------------------------------------------
// Main program entry point.
//-----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(subWidth * 2 + subGap * 3, subHeight * 1 + subGap * 2);

    mainWin = glutCreateWindow("Hello, subWindows");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);

    sub1Win = glutCreateSubWindow(mainWin, subGap, subGap, subWidth, subHeight);
    glutDisplayFunc(RenderSubScene1);

    sub2Win = glutCreateSubWindow(mainWin, subGap + subWidth + subGap, subGap, subWidth, subHeight);
    glutDisplayFunc(RenderSubScene2);

    glutSpecialFunc(SpecialKeys);
    glutTimerFunc(msecDelay, TimerFunction, 1);

    SetupRC();
    glutMainLoop();

    return 0;
}
