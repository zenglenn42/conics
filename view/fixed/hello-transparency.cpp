// OpenGL Programming Guide, Seventh Edition
// Example: 6-3 alpha3D.c

#include <stdlib.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#define MAXZ 8.0
#define MINZ -8.0
#define ZINC 0.4
static float solidZ = MAXZ;
static float transparentZ = MINZ;
static GLuint sphereList, coneList, planeList;

static void init(void) {
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.15 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat position[] = { 0.5, 0.5, 1.0, 0.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    sphereList = glGenLists(1);
    glNewList(sphereList, GL_COMPILE);
        glutSolidSphere(0.4, 20, 20);
    glEndList();

    planeList = glGenLists(1);
    glNewList(planeList, GL_COMPILE);
        glPushMatrix();
            glTranslatef(0.0, 0.3, 0.0);
	    glRotatef(-45.0, 1.0, 0.0, 0.0);
            glBegin(GL_QUADS);
                glVertex3f(-0.6, 0.0, -0.6);
                glVertex3f(0.6, 0.0, -0.6);
                glVertex3f(0.6, 0.0, 0.6);
                glVertex3f(-0.6, 0.0, 0.6);
            glEnd();            
        glPopMatrix();
    glEndList();

    coneList = glGenLists(1);
    glNewList(coneList, GL_COMPILE);
        glPushMatrix();
	    glRotatef(90.0, 1.0, 0.0, 0.0);
	    glTranslatef(0.0, 0.0, -0.5);
            glutSolidCone(0.5, 0.5, 16, 16);
        glPopMatrix();
        glPushMatrix();
	    glRotatef(-90.0, 1.0, 0.0, 0.0);
	    glTranslatef(0.0, 0.0, -0.5);
            glutSolidCone(0.5, 0.5, 16, 16);
        glPopMatrix();
    glEndList();
}

void display(void) {
    GLfloat mat_solid[] = { 0.75, 0.0, 0.0, 1.0 };
    GLfloat mat_zero[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat mat_transparent_cone[] = { 0.0, 0.8, 0.8, 0.6 };
    GLfloat mat_emission_cone[] = { 0.0, 0.3, 0.3, 0.6 };

    GLfloat mat_transparent_plane[] = { 0.0, 0.8, 0.8, 0.6 };
    GLfloat mat_emission_plane[] = { 0.5, 0.5, 0.5, 0.6 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    glPushMatrix();
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);
        glCallList(sphereList);
    glPopMatrix();

    glPushMatrix();
        glRotatef(15.0, 1.0, 1.0, 0.0);
        glRotatef(30.0, 0.0, 1.0, 0.0);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission_cone);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent_cone);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glCallList(coneList);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    glPopMatrix();

    glPushMatrix();
        glRotatef(15.0, 1.0, 1.0, 0.0);
        glRotatef(30.0, 0.0, 1.0, 0.0);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission_plane);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent_plane);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glCallList(planeList);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w,
            1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-1.5 * (GLfloat)w / (GLfloat)h, 1.5 * (GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glLoadIdentity();
}

void animate(void) {
    if (solidZ <= MINZ || transparentZ >= MAXZ)
        glutIdleFunc(NULL);
    else {
        solidZ -= ZINC;
        transparentZ += ZINC;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'a':
        case 'A':
            solidZ = MAXZ;
            transparentZ = MINZ;
            glutIdleFunc(animate);
            break;
        case 'r':
        case 'R':
            solidZ = MAXZ;
            transparentZ = MINZ;
            glutPostRedisplay();
            break;
        case 27:
            exit(0);
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow(argv[0]);
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
