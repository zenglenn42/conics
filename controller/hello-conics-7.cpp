
/***************************************************************************
  
  hello-conics-7

  A GLUI program demonstrating subwindows, rotation controls, translation 
  controls, and listboxes

  This version includes:

        double cone
        reset button
        rgb sliders for plane color
        slider for cone generator angle
        attempt at transparency
        pre-configured conic buttons (parabola, elipse, hyperbola)
        add Y and Z axis labels
        streamline the ui a bit more
        jettison cone transparency, not very useful
        add arrow-heads to axes
        add conic clip plane
        scale letters to more proportionate size	
  -----------------------------------------------------------------------

  The glui layout is descended from original example code written by:
  7/10/98 Paul Rademacher (rademach@cs.unc.edu) (GLUI toolkit ver 2.37)

  Copyright (☯) 2018 Glenn Streiff

****************************************************************************/

#include <iostream>
#include <cmath>
#include <string.h>
#include <GL/glui.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void control_cb(GLint control);
GLUI_Checkbox *g_clip_plane_checkbox;
GLUI_Scrollbar * g_scroll_transparency;

GLfloat g_arcball_radius = 1.35;
GLfloat g_xy_aspect;

static const GLfloat DFLT_CONE_WIDTH = 0.4;
GLfloat g_cone_width = DFLT_CONE_WIDTH;
GLfloat g_cone_height = g_cone_width;
GLfloat g_cone_angle = atan(g_cone_height/g_cone_width);

static const GLfloat DFLT_PLANE_TRANSPARENCY = 0.4; // opaque = 1.0, invisible = 0.0
GLfloat g_plane_transparency = DFLT_PLANE_TRANSPARENCY; 
GLfloat g_plane_material[] = {0.0, 0.5, 1.0, g_plane_transparency};

/** These are the live variables passed into GLUI. ***/
static const GLint DFLT_3D_CONE_SEGMENTS = 10;
static const GLint DFLT_3D_CONE_SLICES = 50;
GLint   g_live_segments = DFLT_3D_CONE_SEGMENTS;
GLint   g_live_slices = DFLT_3D_CONE_SLICES;
GLint   g_live_wireframe = 0;
GLint   g_live_light0_enabled = 1;
GLint   g_live_light1_enabled = 1;
GLfloat g_live_light0_intensity = 1.0;
GLfloat g_live_light1_intensity = 1.0;
GLint   g_main_window;
GLint   g_show_cone = 1;
GLint   g_show_plane = 1;
double  g_reset_coeff[] = {0.0, 0.0, 0.0, 0.0};
GLint   g_live_show_axes = 1;
GLint   g_live_clip_plane = 0;

static const GLfloat DFLT_MATRIX_ROTATE[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
GLfloat g_cone_rotate[16]  = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
GLfloat g_plane_rotate[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
GLfloat g_obj_rotate[16]   = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

GLfloat g_obj_pos[]   = {0.0, 0.0, 0.0};
GLfloat g_plane_pos[] = {0.0, 0.0, 0.0};

/** Rotator ball user controls. **/

GLUI_Rotation *g_obj_rotator;
GLUI_Rotation *g_cone_rotator;
GLUI_Rotation *g_plane_rotator;

/** Pointers to the windows and some of the controls we'll create **/

GLUI *g_glui_win, *g_glui_win2;
GLUI_Spinner *g_light0_spinner, *g_light1_spinner;

GLfloat g_view_scale = 1.25f;
GLfloat g_plane_scale = 0.55f;
GLfloat g_plane_color[] = {1.0, 1.0, 1.0, g_plane_transparency};

/********** User IDs for callbacks ******************/

static const GLint   LIGHT0_ENABLED_ID = 200;
static const GLint   LIGHT1_ENABLED_ID = 201;
static const GLint LIGHT0_INTENSITY_ID = 250;
static const GLint LIGHT1_INTENSITY_ID = 260;
static const GLint      PLANE_COLOR_ID = 270;
static const GLint       CONE_ANGLE_ID = 280;
static const GLint         PARABOLA_ID = 290;
static const GLint           ELIPSE_ID = 291;
static const GLint           CIRCLE_ID = 292;
static const GLint        HYPERBOLA_ID = 293;
static const GLint       CLIP_PLANE_ID = 295;

/********** Miscellaneous global variables **********/

GLfloat g_light0_ambient[]  = { 0.1f,  0.1f, 0.3f, 1.0f};
GLfloat g_light0_diffuse[]  = { 0.0f,  0.9f, 1.0f, 1.0f};
GLfloat g_light0_position[] = { 0.5f,  0.5f, 1.0f, 0.0f};

GLfloat g_light1_ambient[]  = { 0.1f,  0.1f, 0.3f, 1.0f};
GLfloat g_light1_diffuse[]  = { 0.0f,  0.9f, 1.0f, 1.0f};
GLfloat g_light1_position[] = {-1.0f, -1.0f, 1.0f, 0.0f};

GLfloat g_lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

/********** Function Definitions ********************/
struct Point {
        double x, y, z;
};

struct PlaneEqn {
        PlaneEqn(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {
		computeCoeff(p1, p2, p3);
        }   

        PlaneEqn(GLdouble x1, GLdouble y1, GLdouble z1,
              GLdouble x2, GLdouble y2, GLdouble z2,
              GLdouble x3, GLdouble y3, GLdouble z3) {

		p1.x = x1;
		p1.y = y1;
		p1.z = z1;
		p2.x = x2;
		p2.y = y2;
		p2.z = z2;
		p3.x = x3;
		p3.y = y3;
		p3.z = z3;
		computeCoeff(p1, p2, p3);
        }   

	void setPlane(Point p1, Point p2, Point p3) {
		this->p1 = p1;
		this->p2 = p2;
		this->p3 = p3;
		computeCoeff(p1, p2, p3);
	}

        void setPlane(GLdouble x1, GLdouble y1, GLdouble z1,
              GLdouble x2, GLdouble y2, GLdouble z2,
              GLdouble x3, GLdouble y3, GLdouble z3) {

		p1.x = x1;
		p1.y = y1;
		p1.z = z1;
		p2.x = x2;
		p2.y = y2;
		p2.z = z2;
		p3.x = x3;
		p3.y = y3;
		p3.z = z3;
		computeCoeff(p1, p2, p3);
	}

	void computeCoeff(Point p1, Point p2, Point p3) {
                coeff[0] = (p1.y*(p2.z - p3.z)) + (p2.y*(p3.z - p1.z)) + (p3.y*(p1.z - p2.z));
                coeff[1] = (p1.z*(p2.x - p3.x)) + (p2.z*(p3.x - p1.x)) + (p3.z*(p1.x - p2.x));
                coeff[2] = (p1.x*(p2.y - p3.y)) + (p2.x*(p3.y - p1.y)) + (p3.x*(p1.y - p2.y));
                coeff[3] = -((p1.x*((p2.y*p3.z) - (p3.y*p2.z))) + (p2.x*((p3.y*p1.z) - (p1.y*p3.z))) + (p3.x*((p1.y*p2.z) - (p2.y*p1.z))));
	}

        Point p1, p2, p3; 
        double coeff[4];
};


/********** Function Definitions ********************/
GLint reset() {
    g_live_wireframe = 0;
    g_live_segments = DFLT_3D_CONE_SEGMENTS;
    g_live_slices = DFLT_3D_CONE_SLICES;
    g_live_light0_enabled = 1;
    g_live_light1_enabled = 1;
    g_live_light0_intensity = 1.0;
    g_live_light1_intensity = 1.0;
    g_show_cone = 1;
    g_show_plane=1;
    g_live_show_axes = 1;
    g_live_clip_plane = 0;
    g_clip_plane_checkbox->set_int_val(g_live_clip_plane);
    if (!g_live_clip_plane) {
       glDisable(GL_CLIP_PLANE0);
       glClipPlane(GL_CLIP_PLANE0, g_reset_coeff);
       //control_cb(CLIP_PLANE_ID);
    }
    g_obj_pos[0] = 0.0;
    g_obj_pos[1] = 0.0;
    g_obj_pos[2] = 0.0;
    g_plane_pos[0] = 0.0;
    g_plane_pos[1] = 0.0;
    g_plane_pos[2] = 0.0;
    for (GLint i = 0; i < 16; i++) {
      g_cone_rotate[i] = g_plane_rotate[i] = g_obj_rotate[i] = DFLT_MATRIX_ROTATE[i];
    }
    g_obj_rotator->reset();
    g_obj_rotator->init_ball();
    g_obj_rotator->draw_ball(g_arcball_radius);
    g_cone_rotator->reset();
    g_cone_rotator->init_ball();
    g_cone_rotator->draw_ball(g_arcball_radius);
    g_plane_rotator->reset();
    g_plane_rotator->init_ball();
    g_plane_rotator->draw_ball(g_arcball_radius);
    g_plane_color[0] = 1.0;
    g_plane_color[1] = 1.0;
    g_plane_color[2] = 1.0;
    g_plane_transparency = DFLT_PLANE_TRANSPARENCY; // opaque = 1.0, invisible = 0.0
    g_plane_color[3] = g_plane_transparency;
    g_scroll_transparency->set_float_val(g_plane_color[3]);
    g_cone_width = DFLT_CONE_WIDTH;
    g_cone_angle = atan(g_cone_height / g_cone_width);

    g_plane_material[0] = 0.0;
    g_plane_material[1] = 0.5;
    g_plane_material[2] = 1.0;
    g_plane_material[3] = DFLT_PLANE_TRANSPARENCY;

    glutPostRedisplay();
    return 1;
}

void updatePlaneAngle(GLint conic_id) {
    g_cone_angle = abs((M_PI / 2.0) - atan(g_cone_height / g_cone_width));
    g_plane_rotate[0] = 1; g_plane_rotate[1] = 0; g_plane_rotate[2] = 0; g_plane_rotate[3] = 0;
    GLfloat f;
    switch(conic_id) {
        case PARABOLA_ID:
    		//std::cout << "elipse" << std::endl;
		g_plane_rotate[4] = 0; g_plane_rotate[5] = cos(g_cone_angle); g_plane_rotate[6] = -sin(g_cone_angle); g_plane_rotate[7] = 0;
		g_plane_rotate[8] = 0; g_plane_rotate[9] = sin(g_cone_angle); g_plane_rotate[10] = cos(g_cone_angle); g_plane_rotate[11] = 0;
		g_plane_rotate[12] = 0; g_plane_rotate[13] = g_cone_height / 2.0f; g_plane_rotate[14] = 0; g_plane_rotate[15] = 1;
    		//std::cout << "parabola" << std::endl;
		break;
        case ELIPSE_ID:
    		//std::cout << "elipse" << std::endl;
		f = 1.5;
		g_plane_rotate[4] = 0; g_plane_rotate[5] = cos(f*g_cone_angle); g_plane_rotate[6] = -sin(f*g_cone_angle); g_plane_rotate[7] = 0;
		g_plane_rotate[8] = 0; g_plane_rotate[9] = sin(f*g_cone_angle); g_plane_rotate[10] = cos(f*g_cone_angle); g_plane_rotate[11] = 0;
		g_plane_rotate[12] = 0; g_plane_rotate[13] = g_cone_height / 2.0f; g_plane_rotate[14] = 0; g_plane_rotate[15] = 1;
		break;
	case CIRCLE_ID: {
    		//std::cout << "circle" << std::endl;
		f = 2.0f;
                GLfloat angle = M_PI / 2.0; // rotate 90 deg about x-axis
		g_plane_rotate[4] = 0; g_plane_rotate[5] = cos(angle); g_plane_rotate[6] = -sin(angle); g_plane_rotate[7] = 0;
		g_plane_rotate[4] = 0; g_plane_rotate[5] = cos(angle); g_plane_rotate[6] = -sin(angle); g_plane_rotate[7] = 0;
		g_plane_rotate[8] = 0; g_plane_rotate[9] = sin(angle); g_plane_rotate[10] = cos(angle); g_plane_rotate[11] = 0;
		g_plane_rotate[12] = 0; g_plane_rotate[13] = g_cone_height / 2.0f; g_plane_rotate[14] = 0; g_plane_rotate[15] = 1;
                }
		break;
	case HYPERBOLA_ID:
    		//std::cout << "hyperbola" << std::endl;
		f = 0.0;
		g_plane_rotate[4] = 0; g_plane_rotate[5] = cos(f * g_cone_angle); g_plane_rotate[6] = -sin(f * g_cone_angle); g_plane_rotate[7] = 0;
		g_plane_rotate[8] = 0; g_plane_rotate[9] = sin(f * g_cone_angle); g_plane_rotate[10] = cos(f * g_cone_angle); g_plane_rotate[11] = 0;
		g_plane_rotate[12] = 0; g_plane_rotate[13] = 0; g_plane_rotate[14] = g_cone_width/4.0; g_plane_rotate[15] = 1;
		break;
    }
}

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb(GLint control)
{
  if (control == PLANE_COLOR_ID) {
  } else if (control == CLIP_PLANE_ID) {
    if (!g_live_clip_plane) {
       glDisable(GL_CLIP_PLANE0);
       glClipPlane(GL_CLIP_PLANE0, g_reset_coeff);
    } else {
       glEnable(GL_CLIP_PLANE0);
    }
    glutPostRedisplay();
  }
  else if (control == PARABOLA_ID || control == ELIPSE_ID || control == CIRCLE_ID || control == HYPERBOLA_ID ) {
    updatePlaneAngle(control);
    glutPostRedisplay();
  }
  else if (control == CONE_ANGLE_ID) {
    g_cone_width = (2.0f * g_cone_height) / tan(g_cone_angle);
  }
  else if (control == LIGHT0_ENABLED_ID) {
    if (g_live_light0_enabled) {
      glEnable(GL_LIGHT0);
      g_light0_spinner->enable();
    }
    else {
      glDisable(GL_LIGHT0); 
      g_light0_spinner->disable();
    }
  }
  else if (control == LIGHT1_ENABLED_ID) {
    if (g_live_light1_enabled) {
      glEnable(GL_LIGHT1);
      g_light1_spinner->enable();
    }
    else {
      glDisable(GL_LIGHT1); 
      g_light1_spinner->disable();
    }
  }
  else if (control == LIGHT0_INTENSITY_ID) {
    GLfloat v[] = { 
      g_light0_diffuse[0],  g_light0_diffuse[1],
      g_light0_diffuse[2],  g_light0_diffuse[3] };
    
    v[0] *= g_live_light0_intensity;
    v[1] *= g_live_light0_intensity;
    v[2] *= g_live_light0_intensity;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
  }
  else if (control == LIGHT1_INTENSITY_ID) {
    GLfloat v[] = { 
      g_light1_diffuse[0], g_light1_diffuse[1],
      g_light1_diffuse[2], g_light1_diffuse[3] };
    
    v[0] *= g_live_light1_intensity;
    v[1] *= g_live_light1_intensity;
    v[2] *= g_live_light1_intensity;

    glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
  }
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, GLint x, GLint y)
{
  switch(Key)
  {
  case 27: 
  case 'q':
    exit(0);
    break;
  };
  glutPostRedisplay();
}

/***************************************** myGlutMenu() ***********/

void myGlutMenu(GLint value)
{
  myGlutKeyboard(value, 0, 0);
}

/***************************************** myGlutIdle() ***********/

void myGlutIdle()
{
  /*
     According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary 
  */

  if (glutGetWindow() != g_main_window) 
    glutSetWindow(g_main_window);  

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */
  glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(GLint button, GLint button_state, GLint x, GLint y)
{
}

/***************************************** myGlutMotion() **********/

void myGlutMotion(GLint x, GLint y)
{
  glutPostRedisplay(); 
}

/**************************************** myGlutReshape() *************/

void myGlutReshape(GLint x, GLint y)
{
  GLint tx, ty, tw, th;
  GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
  glViewport(tx, ty, tw, th);

  g_xy_aspect = (GLfloat)tw / (GLfloat)th;
  glutPostRedisplay();
}

/************************************************** draw_axes() **********/
/* Disables lighting, then draws RGB axes                                */

void draw_axes(GLfloat scale)
{
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glScalef(scale, scale, scale);

  glBegin(GL_LINES);
    glColor3f(0.8, 0.0, 0.0);
    /* Letter X */
    glVertex3f(0.9f, 0.025f, 0.0); glVertex3f(1.0, 0.125f, 0.0); 
    glVertex3f(0.9f, 0.125f, 0.0); glVertex3f(1.0, 0.025f, 0.0);

    /* X axis */
    glVertex3f(0.0f, 0.00f, 0.0); glVertex3f(1.0, 0.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);
    /* Letter Y */
    glVertex3f(0.0250f, 0.9f, 0.0); glVertex3f(0.125f, 1.0f, 0.0); 
    glVertex3f(0.0250f, 1.0f, 0.0); glVertex3f(0.068, 0.94f, 0.0);

    /* Y axis */
    glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, 1.0, 0.0);    

    glColor3f(0.0, 0.0, 1.0);
    /* Letter Z */
    glVertex3f(0.025f, 0.0f, 0.9f); glVertex3f(0.125f, 0.0, 0.9f); 
    glVertex3f(0.125f, 0.0f, 0.9f); glVertex3f(0.025f, 0.0, 1.0f); 
    glVertex3f(0.025f, 0.0f, 1.0f); glVertex3f(0.125f, 0.0, 1.0f); 

    /* Z axis */
    glVertex3f(0.0, 0.0, 0.0); glVertex3f(0.0, 0.0, 1.0); 

  glEnd();

  /* X axis arrow */
  glPushMatrix();
    glColor3f(0.8, 0.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    glutSolidCone(0.02, 0.10, 10, 10);
  glPopMatrix();

  /* Y axis arrow */
  glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(0.0, 1.0, 0.0);
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(0.02, 0.10, 10, 10);
  glPopMatrix();

  /* Z axis arrow */
  glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(0.0, 0.0, 1.0);
    glutSolidCone(0.02, 0.10, 10, 10);
  glPopMatrix();

  glPopMatrix();
  glEnable(GL_LIGHTING);
}

void draw_plane(GLfloat scale, GLfloat plane_color[])
{ 
  glDisable(GL_LIGHTING);
  static PlaneEqn plnEqn(0.99, -0.99, 0.01,
                        -0.99, -0.99, 0.01,
                        -0.99,  0.99, 0.01);
  
  if (g_live_clip_plane) {
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, plnEqn.coeff);
  } else {
    //glDisable(GL_CLIP_PLANE0);
    //glClipPlane(GL_CLIP_PLANE0, g_reset_coeff);
  }
  glPushMatrix(); 
    glScalef(scale, scale, scale);
    glColor4fv(plane_color);
    glBegin(GL_QUADS);
      glVertex3f(-1.0,  1.0, 0.0); 
      glVertex3f(-1.0, -1.0, 0.0); 
      glVertex3f( 1.0, -1.0, 0.0); 
      glVertex3f( 1.0,  1.0, 0.0); 
    glEnd();
  glPopMatrix();
  glEnable(GL_LIGHTING);
}

/***************************************** myGlutDisplay() *****************/

void myGlutDisplay()
{
  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_LINE_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-g_xy_aspect * 0.04, g_xy_aspect * 0.04, -0.04, 0.04, 0.1, 15.0);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  glMultMatrixf(g_lights_rotation);
  glLightfv(GL_LIGHT0, GL_POSITION, g_light0_position);
  
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -2.6f);
  glTranslatef(g_obj_pos[0], g_obj_pos[1], -g_obj_pos[2]); 
  glMultMatrixf(g_obj_rotate);

  glScalef(g_view_scale, g_view_scale, g_view_scale);

  /** Render objects using live variables update through the GLUI window. **/

  /** Draw cone **/

  glPushMatrix();
    glMultMatrixf(g_cone_rotate);
    glPushMatrix();
      glRotatef(-90.0, 1.0, 0.0, 0.0);
      glTranslatef(0.0, 0.0, -DFLT_CONE_WIDTH);
      if (g_live_wireframe && g_show_cone)
        glutWireCone(g_cone_width, g_cone_height, g_live_slices, g_live_segments);
      else if (g_show_cone)
        glutSolidCone(g_cone_width, g_cone_height, g_live_slices, g_live_segments);
    glPopMatrix();

    glPushMatrix();
      glRotatef(90.0, 1.0, 0.0, 0.0);
      glTranslatef(0.0, 0.0, -DFLT_CONE_WIDTH);
      if (g_live_wireframe && g_show_cone)
        glutWireCone(g_cone_width, g_cone_height, g_live_slices, g_live_segments);
      else if (g_show_cone)
        glutSolidCone(g_cone_width, g_cone_height, g_live_slices, g_live_segments);
    glPopMatrix();
    if (g_live_show_axes)
      draw_axes(g_plane_scale);
  glPopMatrix();

  /** Draw plane **/

  glPushMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, g_plane_material);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glTranslatef(g_plane_pos[0], g_plane_pos[1], -g_plane_pos[2]); 
    glMultMatrixf(g_plane_rotate);
    if (g_live_wireframe && g_show_plane)
      draw_plane(g_plane_scale, g_plane_color);
    else if (g_show_plane)
      draw_plane(g_plane_scale, g_plane_color);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
  glPopMatrix();

  glPushMatrix();
    glMultMatrixf(g_cone_rotate);
    glPopMatrix();
    if (g_live_show_axes)
      draw_axes(g_plane_scale);
  glPopMatrix();

  glEnable(GL_LIGHTING);
  glutSwapBuffers(); 
}

/**************************************** main() ********************/

int main(int argc, char* argv[])
{
  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(800, 600);
 
  g_main_window = glutCreateWindow("Conic Sections");
  glutDisplayFunc(myGlutDisplay);
  GLUI_Master.set_glutReshapeFunc(myGlutReshape);  
  GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
  GLUI_Master.set_glutSpecialFunc(NULL);
  GLUI_Master.set_glutMouseFunc(myGlutMouse);
  glutMotionFunc(myGlutMotion);

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/

  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, g_light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, g_light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, g_light0_position);

  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, g_light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, g_light1_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, g_light1_position);

  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);

  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  printf("GLUI version: %3.2f\n", GLUI_Master.get_version());

  /*** Create the side subwindow ***/
  g_glui_win = GLUI_Master.create_glui_subwindow(g_main_window, 
					    GLUI_SUBWINDOW_RIGHT);

  /******** Add some controls for pre-set conics ********/

  GLUI_Rollout *roll_conics = new GLUI_Rollout(g_glui_win, "Conics", true);
  new GLUI_Button(roll_conics, "parabola", PARABOLA_ID, control_cb);
  new GLUI_Button(roll_conics, "elipse", ELIPSE_ID, control_cb);
  new GLUI_Button(roll_conics, "circle", CIRCLE_ID, control_cb);
  new GLUI_Button(roll_conics, "hyperbola", HYPERBOLA_ID, control_cb);

  /*** Add another rollout ***/
  GLUI_Rollout *options = new GLUI_Rollout(g_glui_win, "Options", false);
  new GLUI_Checkbox(options, "Draw axes", &g_live_show_axes);
  g_clip_plane_checkbox = new GLUI_Checkbox(options, "Clip plane", &g_live_clip_plane, CLIP_PLANE_ID, control_cb);

  //new GLUI_Button(g_glui_win, "Quit", 0,(GLUI_Update_CB)exit);

  /******** Add some controls for cone ********/

  GLUI_Rollout *roll_cone = new GLUI_Rollout(options, "Cone", false);
  new GLUI_Checkbox(roll_cone, "Draw cone", &g_show_cone);

  GLUI_Panel *angle_panel = new GLUI_Panel(roll_cone, "Angle (0 - 45)");
  GLUI_Scrollbar *asb;
  asb = new GLUI_Scrollbar(angle_panel, "Angle",GLUI_SCROLL_HORIZONTAL,
                           &g_cone_angle,CONE_ANGLE_ID,control_cb);
  GLfloat min_cone_angle = atan(g_cone_height / (DFLT_CONE_WIDTH / 2.0));
  asb->set_float_limits(M_PI/2.0, min_cone_angle);

  new GLUI_Checkbox(roll_cone, "Wireframe", &g_live_wireframe, 1, control_cb);
  GLUI_Spinner *spinner2 = 
    new GLUI_Spinner(roll_cone, "Slices:", &g_live_slices);
  spinner2->set_int_limits(3, 60);
  spinner2->set_alignment(GLUI_ALIGN_RIGHT);

  GLUI_Spinner *spinner = 
    new GLUI_Spinner(roll_cone, "Segments:", &g_live_segments);
  spinner->set_int_limits(3, 60);
  spinner->set_alignment(GLUI_ALIGN_RIGHT);

  /******** Add some controls for plane ********/

  GLUI_Rollout *roll_plane = new GLUI_Rollout(options, "Plane", false);
  new GLUI_Checkbox(roll_plane, "Draw plane", &g_show_plane);

  GLUI_Panel *color_panel = new GLUI_Panel(roll_plane, "Color (RGB)");
  GLUI_Scrollbar *psb;
  psb = new GLUI_Scrollbar(color_panel, "Red",GLUI_SCROLL_HORIZONTAL,
                           &g_plane_color[0],PLANE_COLOR_ID,control_cb);
  psb->set_float_limits(0,1);
  psb = new GLUI_Scrollbar(color_panel, "Green",GLUI_SCROLL_HORIZONTAL,
                           &g_plane_color[1],PLANE_COLOR_ID,control_cb);
  psb->set_float_limits(0,1);
  psb = new GLUI_Scrollbar(color_panel, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &g_plane_color[2],PLANE_COLOR_ID,control_cb);
  psb->set_float_limits(0,1);

  /******** Add some controls for lights ********/

  GLUI_Rollout *roll_lights = new GLUI_Rollout(options, "Lights", false);

  GLUI_Panel *light0 = new GLUI_Panel(roll_lights, "Light 1");
  GLUI_Panel *light1 = new GLUI_Panel(roll_lights, "Light 2");

  new GLUI_Checkbox(light0, "Enabled", &g_live_light0_enabled,
                     LIGHT0_ENABLED_ID, control_cb);
  g_light0_spinner =
    new GLUI_Spinner(light0, "Intensity:",
                      &g_live_light0_intensity, LIGHT0_INTENSITY_ID,
                      control_cb);
  g_light0_spinner->set_float_limits(0.0, 1.0);
  GLUI_Scrollbar *sb;
  sb = new GLUI_Scrollbar(light0, "Red",GLUI_SCROLL_HORIZONTAL,
                           &g_light0_diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light0, "Green",GLUI_SCROLL_HORIZONTAL,
                           &g_light0_diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light0, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &g_light0_diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  new GLUI_Checkbox(light1, "Enabled", &g_live_light1_enabled,
                     LIGHT1_ENABLED_ID, control_cb);
  g_light1_spinner =
    new GLUI_Spinner(light1, "Intensity:",
                      &g_live_light1_intensity, LIGHT1_INTENSITY_ID,
                      control_cb);
  g_light1_spinner->set_float_limits(0.0, 1.0);
  sb = new GLUI_Scrollbar(light1, "Red",GLUI_SCROLL_HORIZONTAL,
                           &g_light1_diffuse[0],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light1, "Green",GLUI_SCROLL_HORIZONTAL,
                           &g_light1_diffuse[1],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light1, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &g_light1_diffuse[2],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);

  /**** Link windows to GLUI, and register idle callback ******/
  
  g_glui_win->set_main_gfx_window(g_main_window);

  /*** Create the bottom subwindow ***/

  g_glui_win2 = GLUI_Master.create_glui_subwindow(g_main_window, 
                                             GLUI_SUBWINDOW_BOTTOM);
  g_glui_win2->set_main_gfx_window(g_main_window);

  g_obj_rotator = new GLUI_Rotation(g_glui_win2, "Rotate", g_obj_rotate);
  g_obj_rotator->set_spin(1.0);
  new GLUI_Column(g_glui_win2, false);
  GLUI_Translation *trans_z = 
    new GLUI_Translation(g_glui_win2, "Zoom", GLUI_TRANSLATION_Z, &g_obj_pos[2]);
  trans_z->set_speed(.005);

  new GLUI_Column(g_glui_win2, false);
  g_plane_rotator = new GLUI_Rotation(g_glui_win2, "Plane", g_plane_rotate);
  g_plane_rotator->set_spin(.98);

  new GLUI_Column(g_glui_win2, false);
  GLUI_Translation *ptrans_y = 
    new GLUI_Translation(g_glui_win2, "Plane Y", GLUI_TRANSLATION_Y, &g_plane_pos[1]);
  ptrans_y->set_speed(.005);
  new GLUI_Column(g_glui_win2, false);
  GLUI_Translation *ptrans_z = 
    new GLUI_Translation(g_glui_win2, "Plane Z", GLUI_TRANSLATION_Z, &g_plane_pos[2]);
  ptrans_z->set_speed(.005);
  new GLUI_Column(g_glui_win2, false);
  GLUI_Panel *color_panel_2 = new GLUI_Panel(g_glui_win2, "Transparency");
  g_scroll_transparency = new GLUI_Scrollbar(color_panel_2, "Alpha",GLUI_SCROLL_HORIZONTAL,
                           &g_plane_color[3],PLANE_COLOR_ID,control_cb);
  g_scroll_transparency->set_alignment(GLUI_ALIGN_LEFT);
  g_scroll_transparency->set_float_limits(0,1);
  GLUI_Button *gb = new GLUI_Button(g_glui_win2, "Reset", 0,(GLUI_Update_CB)reset);

  new GLUI_Column(g_glui_win2, false);
  g_cone_rotator = new GLUI_Rotation(g_glui_win2, "Cone", g_cone_rotate);
  g_cone_rotator->set_spin(.98);

#if 0
  /**** We register the idle callback with GLUI, *not* with GLUT ****/
  GLUI_Master.set_glutIdleFunc(myGlutIdle);
#endif

  /**** Regular GLUT main loop ****/

#if 0
  const GLdouble plane_coeff[] = {1.0, 0.0, 1.0, 1.0};
  //glEnable(GL_CLIP_PLANE);
  glClipPlane(GL_CLIP_PLANE0, plane_coeff);
#endif

  glutMainLoop();

  return EXIT_SUCCESS;
}
