//-----------------------------------------------------------------------------
// Maintain the overall aspect ratio of a GLUT window.
// Cycle through various well-known aspect ratios by pressing an arrow key.
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
GLfloat gAspectRatio = GOLDEN_RATIO;

std::string gWinText;
const std::string gWinTitle = "hello, aspect ratio";

GLuint gWinPixelWidth  = 400;
GLuint gWinPixelHeight = (GLfloat) gWinPixelWidth / gAspectRatio;

void DrawModel();
void DrawStr(GLuint x, GLuint y, std::string str, bool center);

//-----------------------------------------------------------------------------
// This function initializes the rendering context.
//-----------------------------------------------------------------------------
void SetupRC() {
    // Bluish background.
    glClearColor(0.0f, 0.3f, 0.80f, 1.0f);

    // Initialize aspect ratio string.
    std::stringstream ss;
    ss << "aspect ratio = " << gAspectRatio;
    gWinText = ss.str();
}

//-----------------------------------------------------------------------------
// Draw bitmap font string
//
// Most of the configuration fu for rendering simple 2D text comes from
// in a graphics context comes from:
//
// https://stackoverflow.com/questions/9430852/glutbitmapcharacter-positions-text-wrong
//-----------------------------------------------------------------------------
void DrawStr(GLuint x, GLuint y, std::string str, bool center = true) {

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
    // Coordinate axes.
    glBegin(GL_LINES);
        glVertex2f(0.0,  1.0);
        glVertex2f(0.0, -1.0);

        glVertex2f(-1.0,  0.0);
        glVertex2f( 1.0,  0.0);
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
        DrawStr(0, 0, gWinText);
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
               gAspectRatio = GOLDEN_RATIO;
            }
            std::stringstream ss;
            ss << "aspect ratio = " << gAspectRatio;
            gWinText = ss.str();
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
// Called by GLUT library when user changes windows size.
//-----------------------------------------------------------------------------
void ChangeSize(int w, int h) {   
    gWinPixelWidth  = w;
    gWinPixelHeight = w / gAspectRatio;
    
    // Set viewport to window dimensions
    glViewport(0, 0, gWinPixelWidth, gWinPixelHeight);

    // Reset the coordinate system before modifying.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enforce final window dimensions to maintain desired aspect ratio
    // despite resize.
    glutReshapeWindow(gWinPixelWidth, gWinPixelHeight);

    DrawStr(0, 0, gWinText);
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
