//-----------------------------------------------------------------------------
// Render some bitmap font text.
//
// This is a bit of a thing if you're not used to the graphics realm.
// You have to disable many rendering attributes to get simple 2D text
// rendered upon your grahics window.
//
// The more generalized approach would employ vector fonts that render
// as graphical objects and can scale in nice ways.
//-----------------------------------------------------------------------------
#include <string>
#include "shared/gltools.h"
#include "shared/glframe.h"
#include <math.h>

GLvoid *fontStyle = GLUT_BITMAP_HELVETICA_18;
GLuint width  = 200;
GLuint height = width * 0.681;

void DrawStr(GLuint x, GLuint y, std::string str, bool center);

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
// Draw bitmap font string
//
// Most of the configuration fu for rendering simple 2D text comes from
// in a graphics context comes from:
//
// https://stackoverflow.com/questions/9430852/glutbitmapcharacter-positions-text-wrong
//-----------------------------------------------------------------------------
void DrawStr(GLuint x, GLuint y, std::string str, bool center = true) {

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

//-----------------------------------------------------------------------------
// Called to draw scene.
//-----------------------------------------------------------------------------
void RenderScene(void) {

    // Clear the window with current clearing color.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    DrawStr(0, 0, "hello, text");

    glutSwapBuffers();
} // RenderScene

//-----------------------------------------------------------------------------
// Respond to arrow keys by moving the camera frame of reference.
//-----------------------------------------------------------------------------
void SpecialKeys(int key, int x, int y) {
#if 0
    if (key == GLUT_KEY_UP)

    if (key == GLUT_KEY_DOWN)

    if (key == GLUT_KEY_LEFT)

    if (key == GLUT_KEY_RIGHT)
#endif
    glutPostRedisplay();
}

//-----------------------------------------------------------------------------
// Called by GLUT library when user changes windows size.
//
// TODO: Fix me :-)
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
    glutInitWindowSize(width, height);

    glutCreateWindow("hello, text");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);

    SetupRC();
    glutMainLoop();

    return 0;
}
