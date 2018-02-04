/****************************************************************************
  
  hello-conics-5

  A GLUI program demonstrating subwindows, rotation controls, translation 
  controls, and listboxes

  This version includes:

        double cone
        reset button
        rgb sliders for plane color
        slider for cone generator angle
        attempt at transparency
        pre-configured conic buttons (parabola, elipse, hyperbola)
        Y and Z axis labels
        add transparency for cone
	
  -----------------------------------------------------------------------

  This is an altered version of code written by:

  7/10/98 Paul Rademacher (rademach@cs.unc.edu)

  in the GLUI user interface toolkit (ver 2.37).

****************************************************************************/

#include <iostream>
#include <string.h>
#include <GL/glui.h>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int reset();
float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;
float ball_radius = 1.35;
float cone_width = 0.4;
float cone_height = 0.4;
float max_cone_width = cone_height;
GLfloat cone_angle = atan(cone_height/cone_width);

float cone_transparency = 1.0;
float plane_transparency = 0.7; // opaque = 1.0, invisible = 0.0
GLfloat mat_transparent_plane[] = { 0.0, 0.5, 1.0, plane_transparency };
GLfloat mat_emission_plane[] = { 0.7, 0.7, 0.7, 0.6 };


/** These are the live variables passed into GLUI ***/
int   wireframe = 0;
int   obj_type = 1;
int   segments = 8;
int   segments2 = 8;
int   light0_enabled = 1;
int   light1_enabled = 1;
float light0_intensity = 1.0;
float light1_intensity = 1.0;
int   main_window;
float scale = 1.25;
int   show_cone = 1;
int   show_plane = 1;
int   show_axes = 1;
int   show_text = 0;
float rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float cone_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float plane_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//float plane_rotate[16] = { 1,0,0,0, 0,cos(3.14159/4.0),-sin(3.14159/4.0),0, 0,sin(3.14159/4.0),cos(3.14159/4.0),0, 0,cone_height/2.0,0,1 };
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
float plane_pos[] = { 0.0, 0.0, 0.0 };
const char *string_list[] = { "Hello World!", "Foo", "Testing...", "Bounding box: on" };
int   curr_string = 0;
GLUI_Rotation *view_rot;
GLUI_Rotation *cone_rot;
GLUI_Rotation *pln_rot;
GLUI_Rotation *lights_rot;

/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui, *glui2;
GLUI_Spinner    *light0_spinner, *light1_spinner;
GLUI_RadioGroup *radio;
GLUI_Panel      *obj_panel;

/********** User IDs for callbacks ********/
#define LIGHT0_ENABLED_ID    200
#define LIGHT1_ENABLED_ID    201
#define LIGHT0_INTENSITY_ID  250
#define LIGHT1_INTENSITY_ID  260
#define PLANE_COLOR_ID       270
#define CONE_ANGLE_ID        280
#define CONE_TRANSPARENCY_ID 281
#define ENABLE_ID            300
#define DISABLE_ID           301
#define SHOW_ID              302
#define HIDE_ID              303
#define PARABOLA_ID          290
#define ELIPSE_ID            291
#define CIRCLE_ID            292
#define HYPERBOLA_ID         293

/********** Miscellaneous global variables **********/

GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {0.0f, 0.9f, 1.0f, 1.0f};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

GLfloat light1_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
//GLfloat light1_diffuse[] =  {.9f, .6f, 0.0f, 1.0f};
GLfloat light1_diffuse[] =  {0.0f, 0.9f, 1.0f, 1.0f};
GLfloat light1_position[] = {-1.0f, -1.0f, 1.0f, 0.0f};

GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };

GLfloat plane_color[] = {1.0, 1.0, 1.0, plane_transparency};

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb(int control)
{
  if (control == PLANE_COLOR_ID) {
    /*
    */
  }
  else if (control == CONE_TRANSPARENCY_ID) {
  }
  else if (control == PARABOLA_ID) {
    reset();
    plane_rotate[0] = 1; plane_rotate[1] = 0; plane_rotate[2] = 0; plane_rotate[3] = 0;
    plane_rotate[4] = 0; plane_rotate[5] = cos(cone_angle); plane_rotate[6] = -sin(cone_angle); plane_rotate[7] = 0;
    plane_rotate[8] = 0; plane_rotate[9] = sin(cone_angle); plane_rotate[10] = cos(cone_angle); plane_rotate[11] = 0;
    plane_rotate[12] = 0; plane_rotate[13] = cone_height/2.0; plane_rotate[14] = 0; plane_rotate[15] = 1;
    glutPostRedisplay();
  }
  else if (control == ELIPSE_ID) {
    reset();
    float f = 1.5;
    plane_rotate[0] = 1; plane_rotate[1] = 0; plane_rotate[2] = 0; plane_rotate[3] = 0;
    plane_rotate[4] = 0; plane_rotate[5] = cos(f*cone_angle); plane_rotate[6] = -sin(f*cone_angle); plane_rotate[7] = 0;
    plane_rotate[8] = 0; plane_rotate[9] = sin(f*cone_angle); plane_rotate[10] = cos(f*cone_angle); plane_rotate[11] = 0;
    plane_rotate[12] = 0; plane_rotate[13] = cone_height/2.0; plane_rotate[14] = 0; plane_rotate[15] = 1;
    glutPostRedisplay();
  }
  else if (control == CIRCLE_ID) {
    reset();
    float f = 2.0;
    plane_rotate[0] = 1; plane_rotate[1] = 0; plane_rotate[2] = 0; plane_rotate[3] = 0;
    plane_rotate[4] = 0; plane_rotate[5] = cos(f*cone_angle); plane_rotate[6] = -sin(f*cone_angle); plane_rotate[7] = 0;
    plane_rotate[8] = 0; plane_rotate[9] = sin(f*cone_angle); plane_rotate[10] = cos(f*cone_angle); plane_rotate[11] = 0;
    plane_rotate[12] = 0; plane_rotate[13] = cone_height/2.0; plane_rotate[14] = 0; plane_rotate[15] = 1;
    glutPostRedisplay();
  }
  else if (control == HYPERBOLA_ID) {
    reset();
    float f = 0.0;
    plane_rotate[0] = 1; plane_rotate[1] = 0; plane_rotate[2] = 0; plane_rotate[3] = 0;
    plane_rotate[4] = 0; plane_rotate[5] = cos(f*cone_angle); plane_rotate[6] = -sin(f*cone_angle); plane_rotate[7] = 0;
    plane_rotate[8] = 0; plane_rotate[9] = sin(f*cone_angle); plane_rotate[10] = cos(f*cone_angle); plane_rotate[11] = 0;
    plane_rotate[12] = 0; plane_rotate[13] = 0; plane_rotate[14] = cone_width/4.0; plane_rotate[15] = 1;
    glutPostRedisplay();
  }
  else if (control == CONE_ANGLE_ID) {
    cone_width = (2.0 * cone_height) / tan(cone_angle);
  }
  else if (control == LIGHT0_ENABLED_ID) {
    if (light0_enabled) {
      glEnable(GL_LIGHT0);
      light0_spinner->enable();
    }
    else {
      glDisable(GL_LIGHT0); 
      light0_spinner->disable();
    }
  }
  else if (control == LIGHT1_ENABLED_ID) {
    if (light1_enabled) {
      glEnable(GL_LIGHT1);
      light1_spinner->enable();
    }
    else {
      glDisable(GL_LIGHT1); 
      light1_spinner->disable();
    }
  }
  else if (control == LIGHT0_INTENSITY_ID) {
    float v[] = { 
      light0_diffuse[0],  light0_diffuse[1],
      light0_diffuse[2],  light0_diffuse[3] };
    
    v[0] *= light0_intensity;
    v[1] *= light0_intensity;
    v[2] *= light0_intensity;

    glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
  }
  else if (control == LIGHT1_INTENSITY_ID) {
    float v[] = { 
      light1_diffuse[0],  light1_diffuse[1],
      light1_diffuse[2],  light1_diffuse[3] };
    
    v[0] *= light1_intensity;
    v[1] *= light1_intensity;
    v[2] *= light1_intensity;

    glLightfv(GL_LIGHT1, GL_DIFFUSE, v);
  }
  else if (control == ENABLE_ID)
  {
    glui2->enable();
  }
  else if (control == DISABLE_ID)
  {
    glui2->disable();
  }
  else if (control == SHOW_ID)
  {
    glui2->show();
  }
  else if (control == HIDE_ID)
  {
    glui2->hide();
  }
}

/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
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

void myGlutMenu(int value)
{
  myGlutKeyboard(value, 0, 0);
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle()
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if (glutGetWindow() != main_window) 
    glutSetWindow(main_window);  

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */

  glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y)
{
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y)
{
  glutPostRedisplay(); 
}

/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
  glViewport(tx, ty, tw, th);

  xy_aspect = (float)tw / (float)th;

  glutPostRedisplay();
}


/************************************************** draw_axes() **********/
/* Disables lighting, then draws RGB axes                                */

void draw_axes(float scale)
{
  glDisable(GL_LIGHTING);

  glPushMatrix();
  glScalef(scale, scale, scale);

  glBegin(GL_LINES);
 
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(.8f, 0.05f, 0.0);  glVertex3f(1.0, 0.25f, 0.0); /* Letter X */
  glVertex3f(0.8f, .25f, 0.0);  glVertex3f(1.0, 0.05f, 0.0);

  glVertex3f(0.0, 0.0, 0.0);  glVertex3f(1.0, 0.0, 0.0); /* X axis      */

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(.05f, .8f, 0.0); glVertex3f(0.25f, 1.0, 0.0); /* Letter Y */
  glVertex3f(0.137f, 0.9f, 0.0); glVertex3f(0.05, 1.0f, 0.0);

  glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 1.0, 0.0); /* Y axis      */

  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(.05f, 0.0, .8f); glVertex3f(.25f, 0.0, .8f); /* Letter Z */
  glVertex3f(.25f, 0.0, .8f); glVertex3f(0.05f, 0.0, 1.0f); 
  glVertex3f(.05f, 0.0, 1.0f); glVertex3f(.25f, 0.0, 1.0f); 
  glVertex3f(0.137f, 0.9f, 0.0); glVertex3f(0.05, 1.0f, 0.0);

  glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 0.0, 1.0); /* Z axis    */
  glEnd();

  glPopMatrix();

  glEnable(GL_LIGHTING);
}

void draw_plane(float scale, GLfloat plane_color[])
{ 
  glDisable(GL_LIGHTING);
  
  glPushMatrix(); 
  glScalef(scale, scale, scale);
  
  glColor4fv(plane_color);
  glBegin(GL_QUADS);
    glVertex3f(-1.0, 1.0, 0.0); 
    glVertex3f(-1.0, -1.0, 0.0); 
    glVertex3f(1.0, -1.0, 0.0); 
    glVertex3f(1.0, 1.0, 0.0); 
  glEnd();
  
  glPopMatrix();
  
  glEnable(GL_LIGHTING);
}



/***************************************** myGlutDisplay() *****************/

void myGlutDisplay()
{
  glClearColor(.5f, .5f, .5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  glMultMatrixf(lights_rotation);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -2.6f);
  glTranslatef(obj_pos[0], obj_pos[1], -obj_pos[2]); 
  glMultMatrixf(view_rotate);

  glScalef(scale, scale, scale);

  /*** Now we render object, using the variables 'obj_type', 'segments', and
    'wireframe'.  These are _live_ variables, which are transparently 
    updated by GLUI ***/

  // Draw cone.
  glPushMatrix();
    //glTranslatef(-.5, 0.0, 0.0);
    glMultMatrixf(cone_rotate);
    glPushMatrix();
      glRotatef(-90.0, 1.0, 0.0, 0.0);
      glTranslatef(0.0, 0.0, -0.4);
      if (wireframe && show_cone)
        glutWireCone(cone_width, cone_height, 16, segments);
      else if (show_cone)
        glutSolidCone(cone_width, cone_height, 16, segments);
    glPopMatrix();
    glPushMatrix();
      glRotatef(90.0, 1.0, 0.0, 0.0);
      glTranslatef(0.0, 0.0, -0.4);
      if (wireframe && show_cone)
        glutWireCone(cone_width, cone_height, 16, segments);
      else if (show_cone)
        glutSolidCone(cone_width, cone_height, 16, segments);
    glPopMatrix();
    if (show_axes)
      draw_axes(.52f);
  glPopMatrix();

  // Draw plane.
  glPushMatrix();

    //glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission_plane);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent_plane);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glTranslatef(plane_pos[0], plane_pos[1], -plane_pos[2]); 
    //glTranslatef(.5, 0.0, 0.0);
    glMultMatrixf(plane_rotate);
    if (wireframe && show_plane)
      draw_plane(.52f, plane_color);
    else if (show_plane)
      draw_plane(.52f, plane_color);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
  glPopMatrix();

  if (show_text) 
  {
    glDisable(GL_LIGHTING);  /* Disable lighting while we render text */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 100.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3ub(0, 0, 0);
    glRasterPos2i(10, 10);

    /*  printf("text: %s\n", text);              */
  }

  glEnable(GL_LIGHTING);


  glutSwapBuffers(); 
}

int reset() {
    wireframe = 0;
    obj_type = 1;
    segments = 8;
    segments2 = 8;
    light0_enabled = 1;
    light1_enabled = 1;
    light0_intensity = 1.0;
    light1_intensity = 1.0;
    scale = 1.25;
    show_cone = 1;
    show_plane=1;
    show_axes = 1;
    show_text = 0;
    obj_pos[0] = 0.0;
    obj_pos[1] = 0.0;
    obj_pos[2] = 0.0;
    plane_pos[0] = 0.0;
    plane_pos[1] = 0.0;
    plane_pos[2] = 0.0;
    for (int i = 0; i < 16; i++) {
      cone_rotate[i] = plane_rotate[i] = view_rotate[i] = rotate[i];
    }
    view_rot->reset();
    view_rot->init_ball();
    view_rot->draw_ball(ball_radius);
    cone_rot->reset();
    cone_rot->init_ball();
    cone_rot->draw_ball(ball_radius);
    pln_rot->reset();
    pln_rot->init_ball();
    pln_rot->draw_ball(ball_radius);
    //lights_rot->reset();
    //lights_rot->init_ball();
    //lights_rot->draw_ball(ball_radius);
    plane_color[0] = 1.0;
    plane_color[1] = 1.0;
    plane_color[2] = 1.0;
    plane_transparency = 0.7; // opaque = 1.0, invisible = 0.0
    plane_color[3] = plane_transparency;

    cone_transparency = 1.0;
    cone_width = 0.4;
    cone_angle = atan(cone_height / cone_width);

    mat_transparent_plane[0] = 0.0;
    mat_transparent_plane[1] = 0.5;
    mat_transparent_plane[2] = 1.0;
    mat_transparent_plane[3] = plane_transparency;
    //mat_emission_plane[] = { 0.7, 0.7, 0.7, 0.6 };

    glutPostRedisplay();
    return 1;
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
 
  main_window = glutCreateWindow("Conic Sections");
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
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);

  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  printf("GLUI version: %3.2f\n", GLUI_Master.get_version());

  /*** Create the side subwindow ***/
  glui = GLUI_Master.create_glui_subwindow(main_window, 
					    GLUI_SUBWINDOW_RIGHT);

  /******** Add some controls for pre-set conics ********/

  GLUI_Rollout *roll_conics = new GLUI_Rollout(glui, "Conics", true);
  new GLUI_Button(roll_conics, "parabola", PARABOLA_ID, control_cb);
  new GLUI_Button(roll_conics, "elipse", ELIPSE_ID, control_cb);
  new GLUI_Button(roll_conics, "circle", CIRCLE_ID, control_cb);
  new GLUI_Button(roll_conics, "hyperbola", HYPERBOLA_ID, control_cb);

  /******** Add some controls for cone ********/

  GLUI_Rollout *roll_cone = new GLUI_Rollout(glui, "Cone", false);
  new GLUI_Checkbox(roll_cone, "Draw cone", &show_cone);

  GLUI_Panel *angle_panel = new GLUI_Panel(roll_cone, "Angle (0 - 45)");
  GLUI_Scrollbar *asb;
  asb = new GLUI_Scrollbar(angle_panel, "Angle",GLUI_SCROLL_HORIZONTAL,
                           &cone_angle,CONE_ANGLE_ID,control_cb);
  float min_cone_angle = atan(cone_height / (max_cone_width / 2.0));
  asb->set_float_limits(3.14159/2.0, min_cone_angle);

  new GLUI_Checkbox(roll_cone, "Wireframe", &wireframe, 1, control_cb);
  GLUI_Spinner *spinner = 
    new GLUI_Spinner(roll_cone, "Segments:", &segments);
  spinner->set_int_limits(3, 60);
  spinner->set_alignment(GLUI_ALIGN_RIGHT);
  GLUI_Panel *cone_glui_panel = new GLUI_Panel(roll_cone, "Transparency");
  GLUI_Scrollbar *psb3;
  psb3 = new GLUI_Scrollbar(cone_glui_panel, "Alpha",GLUI_SCROLL_HORIZONTAL,
                           &cone_transparency,CONE_TRANSPARENCY_ID,control_cb);
  psb3->set_float_limits(0,1);

  /******** Add some controls for plane ********/

  GLUI_Rollout *roll_plane = new GLUI_Rollout(glui, "Plane", false);
  new GLUI_Checkbox(roll_plane, "Draw plane", &show_plane);

  GLUI_Panel *color_panel = new GLUI_Panel(roll_plane, "Color (RGB)");
  GLUI_Scrollbar *psb;
  psb = new GLUI_Scrollbar(color_panel, "Red",GLUI_SCROLL_HORIZONTAL,
                           &plane_color[0],PLANE_COLOR_ID,control_cb);
  psb->set_float_limits(0,1);
  psb = new GLUI_Scrollbar(color_panel, "Green",GLUI_SCROLL_HORIZONTAL,
                           &plane_color[1],PLANE_COLOR_ID,control_cb);
  psb->set_float_limits(0,1);
  psb = new GLUI_Scrollbar(color_panel, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &plane_color[2],PLANE_COLOR_ID,control_cb);
  psb->set_float_limits(0,1);
  GLUI_Panel *color_panel_2 = new GLUI_Panel(roll_plane, "Transparency");
  GLUI_Scrollbar *psb2;
  psb2 = new GLUI_Scrollbar(color_panel_2, "Alpha",GLUI_SCROLL_HORIZONTAL,
                           &plane_color[3],PLANE_COLOR_ID,control_cb);
  psb->set_float_limits(0,1);

#if 0
  obj_panel = new GLUI_Rollout(glui, "Properties", false);

  /***** Control for object params *****/
  GLUI_Spinner *scale_spinner = 
    new GLUI_Spinner(obj_panel, "Scale:", &scale);
  scale_spinner->set_float_limits(.2f, 4.0);
  scale_spinner->set_alignment(GLUI_ALIGN_RIGHT);
#endif

  /*** Add another rollout ***/
  GLUI_Rollout *options = new GLUI_Rollout(glui, "Options", false);
  new GLUI_Checkbox(options, "Draw axes", &show_axes);

  new GLUI_Button(glui, "Quit", 0,(GLUI_Update_CB)exit);


  /******** Add some controls for lights ********/

  GLUI_Rollout *roll_lights = new GLUI_Rollout(options, "Lights", false);

  GLUI_Panel *light0 = new GLUI_Panel(options, "Light 1");
  GLUI_Panel *light1 = new GLUI_Panel(options, "Light 2");

  new GLUI_Checkbox(light0, "Enabled", &light0_enabled,
                     LIGHT0_ENABLED_ID, control_cb);
  light0_spinner =
    new GLUI_Spinner(light0, "Intensity:",
                      &light0_intensity, LIGHT0_INTENSITY_ID,
                      control_cb);
  light0_spinner->set_float_limits(0.0, 1.0);
  GLUI_Scrollbar *sb;
  sb = new GLUI_Scrollbar(light0, "Red",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[0],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light0, "Green",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[1],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light0, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &light0_diffuse[2],LIGHT0_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  new GLUI_Checkbox(light1, "Enabled", &light1_enabled,
                     LIGHT1_ENABLED_ID, control_cb);
  light1_spinner =
    new GLUI_Spinner(light1, "Intensity:",
                      &light1_intensity, LIGHT1_INTENSITY_ID,
                      control_cb);
  light1_spinner->set_float_limits(0.0, 1.0);
  sb = new GLUI_Scrollbar(light1, "Red",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[0],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light1, "Green",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[1],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);
  sb = new GLUI_Scrollbar(light1, "Blue",GLUI_SCROLL_HORIZONTAL,
                           &light1_diffuse[2],LIGHT1_INTENSITY_ID,control_cb);
  sb->set_float_limits(0,1);

  /*new GLUI_Checkbox(options, "Draw text", &show_text);*/


  /**** Link windows to GLUI, and register idle callback ******/
  
  glui->set_main_gfx_window(main_window);


  /*** Create the bottom subwindow ***/
  glui2 = GLUI_Master.create_glui_subwindow(main_window, 
                                             GLUI_SUBWINDOW_BOTTOM);
  glui2->set_main_gfx_window(main_window);

  view_rot = new GLUI_Rotation(glui2, "Rotate", view_rotate);
  view_rot->set_spin(1.0);
  new GLUI_Column(glui2, false);
  GLUI_Translation *trans_z = 
    new GLUI_Translation(glui2, "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2]);
  trans_z->set_speed(.005);

  new GLUI_Column(glui2, false);
  pln_rot = new GLUI_Rotation(glui2, "Plane", plane_rotate);
  pln_rot->set_spin(.98);

  new GLUI_Column(glui2, false);
  GLUI_Translation *ptrans_y = 
    new GLUI_Translation(glui2, "Plane Y", GLUI_TRANSLATION_Y, &plane_pos[1]);
  ptrans_y->set_speed(.005);
  new GLUI_Column(glui2, false);
  GLUI_Translation *ptrans_z = 
    new GLUI_Translation(glui2, "Plane Z", GLUI_TRANSLATION_Z, &plane_pos[2]);
  ptrans_z->set_speed(.005);
  new GLUI_Column(glui2, false);

  new GLUI_Column(glui2, false);
  cone_rot = new GLUI_Rotation(glui2, "Cone", cone_rotate);
  cone_rot->set_spin(.98);

  GLUI_Column *gc = new GLUI_Column(glui2, false);
  GLUI_Button *gb = new GLUI_Button(glui2, "", 0,(GLUI_Update_CB)reset);
  int dummy;
  int col_h = ptrans_z->w;
  gb->set_alignment(GLUI_ALIGN_LEFT);
  gb->set_w(col_h);
  gb->set_h(col_h);
  
  GLUI_String gstr = "  Reset  "; // TODO: find better way to center text under button
  GLUI_StaticText *gst = new GLUI_StaticText(glui2, gstr);

  //lights_rot = new GLUI_Rotation(glui2, "Blue Light", lights_rotation);
  //lights_rot->set_spin(.82);
  //new GLUI_Column(glui2, false);
  //GLUI_Translation *trans_xy = 
  //  new GLUI_Translation(glui2, "Both XY", GLUI_TRANSLATION_XY, obj_pos);
  //trans_xy->set_speed(.005);
  //new GLUI_Column(glui2, false);
  //GLUI_Translation *trans_x = 
  //  new GLUI_Translation(glui2, "Both X", GLUI_TRANSLATION_X, obj_pos);
  //trans_x->set_speed(.005);
  //new GLUI_Column(glui2, false);
  //GLUI_Translation *trans_y = 
  //  new GLUI_Translation(glui2, "Both Y", GLUI_TRANSLATION_Y, &obj_pos[1]);
  //trans_y->set_speed(.005);

#if 0
  /**** We register the idle callback with GLUI, *not* with GLUT ****/
  GLUI_Master.set_glutIdleFunc(myGlutIdle);
#endif

  /**** Regular GLUT main loop ****/
  
  glutMainLoop();

  return EXIT_SUCCESS;
}

