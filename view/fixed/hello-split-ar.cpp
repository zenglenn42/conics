//-----------------------------------------------------------------------------
// Render stuff in two adjacent sub-windows with some lable text, maintaining
// a desired overall aspect ratio in the parent window.
//
// Original code courtesy JH Kim:
// http://study.marearts.com/2012/01/sample-source-to-make-subwindow-in.html
//
// Copyright © Glenn Streiff 2017 (derivative work)
//
// TODO:
//    Get rid of all these dang globals. :-/
//    Add some nice sub-window callback fu outlined in:
//    http://www.lighthouse3d.com/tutorials/glut-tutorial/subwindows/
//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
#include "shared/gltools.h"
#include "shared/glframe.h"
#include <math.h>

GLvoid *fontStyle = GLUT_BITMAP_HELVETICA_18;

const GLfloat GOLDEN_RATIO = 1.618;
const GLfloat HDTV_RATIO = 16.0 / 9.0;
const GLfloat TV_RATIO = 4.0 / 3.0;
const GLfloat TWO_2_ONE_RATIO = 2.0;
const GLfloat ONE_2_ONE_RATIO = 1.0;
GLfloat gAspectRatio = ONE_2_ONE_RATIO;

GLuint mainWin, sub1Win, sub2Win;
GLfloat gSubGapPercent = 0.05;
const GLuint msecDelay = 10;    // Delay for timer-based animation.
GLFrame frameCamera;
GLuint gSubWidth = 300;
GLfloat gSubGap = gSubWidth * gSubGapPercent; // TODO: Add aspect ratio factor.
GLuint gWinPixelWidth = gSubWidth * 2.0 + gSubGap * 3.0;
GLuint gWinPixelHeight = gSubWidth / gAspectRatio + gSubGap * 2.0;
GLuint gSubHeight = gWinPixelHeight - 2 * gSubGap;

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

#if 0
    GLfloat strPixelHeight = glutBitmapWidth(fontStyle, 'X');
    GLfloat vNudge = 1.5 * (gSubGap - strPixelHeight) / gWinPixelHeight;
    std::cout << "gSubGap = " << gSubGap << std::endl;
    std::cout << "vNudge = " << vNudge << std::endl;
#endif
    GLfloat lNudge = (gSubGap * 3.0 / gWinPixelWidth) * .7 ;
    GLfloat rNudge = lNudge * .7;

    DrawStr(-1.0 + lNudge, 0.9, "Side");
    DrawStr( 0.0 + rNudge, 0.9, "Perspective");
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
            // std::stringstream ss;
            // ss << "aspect ratio = " << gAspectRatio;
            // gWinText = ss.str();
            // gWinPixelHeight = gWinPixelWidth / gAspectRatio;
            // glutReshapeWindow(gWinPixelWidth, gWinPixelHeight);
            }
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

    mainWin = glutCreateWindow("Hello, subWindows");
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
