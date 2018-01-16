//-----------------------------------------------------------------------------
// Render stuff in two adjacent sub-windows.
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
//
//    Fix ChangeSize
//-----------------------------------------------------------------------------
#include "shared/gltools.h"
#include "shared/glframe.h"
#include <math.h>

GLuint mainWin, sub1Win, sub2Win;
GLuint subWidth = 300;
GLuint subHeight = 300;
GLuint subGap = 20;
const GLuint msecDelay = 10;    // Delay for timer-based animation.
GLFrame frameCamera;

void ResetViewport();

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

    // frameCamera.ApplyCameraTransform();

    glutSetWindow(sub1Win);
    glutPositionWindow(subGap, subGap);

    glutSetWindow(sub2Win);
    glutPositionWindow(subGap + subWidth + subGap, subGap);

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
//
// TODO: Fix me :-)
//-----------------------------------------------------------------------------
void ChangeSize(int w, int h) {   
    GLfloat fAspect;
    
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    h = (h == 0) ? 1 : h;

    glViewport(0, 0, w, h); 
    
    fAspect = (GLfloat)w / (GLfloat)h;

    // Reset the coordinate system before modifying.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set the clipping volume
    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
