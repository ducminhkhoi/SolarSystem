//
// Created by Khoi Duc Minh Nguyen on 9/29/16.
//

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#define _USE_MATH_DEFINES

#include <math.h>
using namespace std;


#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include <GL/glew.h>
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#else

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#include "main_glslprogram.h"
#include "main_obj.h"


//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// title of these windows:

const char *WINDOWTITLE = {"OpenGL - Homework 3 -- Khoi Nguyen"};
const char *GLUITITLE = {"User Interface Window"};


// what the glui package defines as true and false:

const int GLUITRUE = {true};
const int GLUIFALSE = {false};


// the escape key:

#define ESCAPE        0x1b


// initial window size:

const int INIT_WINDOW_SIZE = {700};


// size of the box:

const float BOXSIZE = {0.5f};



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = {1.};
const float SCLFACT = {0.005f};


// minimum allowable scale factor:

const float MINSCALE = {0.05f};


// active mouse buttons (or them together):

const int LEFT = {4};
const int MIDDLE = {2};
const int RIGHT = {1};


// which projection:

enum Projections {
    ORTHO,
    PERSP
};

// which button:

enum ButtonVals {
    RESET,
    QUIT
};

// which view:

enum View {
    OUTSIDE,
    INSIDE
};

// Which filter:
enum Filter {
    NEAREST,
    LINEAR
};

// window background color (rgba):

const GLfloat BACKCOLOR[] = {0., 0., 0., 1.};


// line width for the axes:

const GLfloat AXES_WIDTH = {3.};


// the color numbers:
// this order must match the radio button order

enum Colors {
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK
};

char *ColorNames[] =
        {
                "Red",
                "Yellow",
                "Green",
                "Cyan",
                "Blue",
                "Magenta",
                "White",
                "Black"
        };


// the color definitions:
// this order must match the menu order

const GLfloat Colors[][3] =
        {
                {1., 0., 0.},        // red
                {1., 1., 0.},        // yellow
                {0., 1., 0.},        // green
                {0., 1., 1.},        // cyan
                {0., 0., 1.},        // blue
                {1., 0., 1.},        // magenta
                {1., 1., 1.},        // white
                {0., 0., 0.},        // black
        };


// fog parameters:

const GLfloat FOGCOLOR[4] = {.0, .0, .0, 1.};
const GLenum FOGMODE = {GL_LINEAR};
const GLfloat FOGDENSITY = {0.30f};
const GLfloat FOGSTART = {1.5};
const GLfloat FOGEND = {4.};

const int MS_IN_THE_ANIMATION_CYCLE = 300000;
float White[] = {1., 1., 1., 1.};
const float D2R = M_PI/180.f;


// non-constant global variables:

int ActiveButton;            // current button that is down
GLuint AxesList;                // list to hold the axes
int AxesOn;                    // != 0 means to draw the axes
int DebugOn;                // != 0 means to print debugging info
int DepthCueOn;                // != 0 means to use intensity depth cueing
int Orbit_On;              // != 0 means to use texture mapping
int ScaleOrbitOn;               // object will rotate or not
int MainWindow;                // window id for main graphics window
float Scale;                    // scaling factor
int WhichColor;                // index into Colors[ ]
int WhichProjection;        // ORTHO or PERSP
int WhichView;              // OUTSIDE or INSIDE
int ScaleDiameterOn;            // NEAREST or LINEAR
int Xmouse, Ymouse;            // mouse values
float Xrot, Yrot;                // rotation angles in degrees
float Time;                   // Global variable
int PlaySound;
int isPlayingSound;
int ShowInstructions;

int show_planet_names;
bool Freeze;
bool Light0On, Light1On, Light2On;
GLuint mercury, earth_day, earth_night, jupiter, saturn_ring, uranus_ring, pluto,
        mars, moon, neptune, saturn, sun, uranus, venus, stars; //
GLuint sun_list, mercury_list, venus_list, earth_list, moon_list, mars_list,
        jupiter_list, saturn_list, uranus_list, neptune_list, pluto_list;

float cam_x, cam_y, cam_z;
float target_x, target_y, target_z;
GLSLProgram	*Pattern;
Model_OBJ obj;

float earth_diameter = 1.;
float moon_diameter = 0.27;
float mercury_diameter = 0.38;
float venus_diameter = 0.95;
float mars_diameter = 0.53;
float jupiter_diameter = log(1120.)/log(10.); //speical values
float saturn_diameter = log(945.)/log(10.);
float uranus_diameter = log(400.)/log(10.);
float neptune_diameter = log(388.)/log(10.);
float pluto_diameter = 0.17;
float comet_diameter = 0.1;

float earth_orbital_radius = 1.f*3; // AU * double
float moon_orbital_radius = 0.5; // Special value for moon
float mercury_orbital_radius = 0.39f * 3;
float venus_orbital_radius = 0.72f * 3;
float mars_orbital_radius = 1.52f * 3;
float jupiter_orbital_radius = log(5.2f)/log(1.3); // special values
float saturn_orbital_radius = log(9.58f)/log(1.3);
float uranus_orbital_radius = log(19.23f)/log(1.3);
float neptune_orbital_radius = log(30.1f)/log(1.3);
float pluto_orbital_radius = log(39.52f)/log(1.3);
float self_rotate_speed = 100;
float orbital_speed_constant = 100;
float orbit_radius_const = 2.f;
float comet_orbital_radius = 1.29f*3;


// function prototypes:

void Animate();

void Display();

void DoAxesMenu(int);

void DoColorMenu(int);

void DoDepthMenu(int);

void DoDebugMenu(int);

void DoMainMenu(int);

void DoProjectMenu(int);

void ShowInstructionsMenu(int);

void DoTextureMenu(int);

void DoFilterMenu(int);

void DoRotateMenu(int);

void ShowEarthMenu(int);

void DoRasterString(float, float, float, char *);

void DoRasterString2(float, float, float, char *);

void DoStrokeString(float, float, float, float, char *);

float ElapsedSeconds();

void InitGraphics();

void InitLists();

void InitMenus();

void Keyboard(unsigned char, int, int);

void SpecialKey(int, int, int);

void MouseButton(int, int, int, int);

void MouseMotion(int, int);

void Reset();

void Resize(int, int);

void Visibility(int);

float *Array3(float, float, float);

float *MulArray3(float, float array0[3]);

void SetMaterial(float, float, float, float);

void SetPointLight(int ilight, float x, float y, float z, float r, float g, float b);

void SetSpotLight(int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b);

void Axes(float);

void HsvRgb(float[3], float [3]);

void DoViewMenu(int id);

// Supported Functions
void Cross(float [3], float [3], float [3]);

float Unit(float [3], float [3]);

void DrawObjects();

float abs2(float a);

void load_texture(string texture_name, GLuint &texture);

float orbital_speed(float orbital_radius);

void set_texture(GLuint texture1, GLuint texture2);

void DrawOrbit(float r);

void ScaleView();

void WriteInstruction();

#include "sphere.cpp"
#include "SOIL.h"
#include "bmptotexture.cpp"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <time.h>


time_t PlayingTime;

using namespace std;
// main program:

int
main(int argc, char *argv[]) {
    // turn on the glut package:
    // (do this before checking argc and argv since it might
    // pull some command line arguments out)

    glutInit(&argc, argv);


    // setup all the graphics stuff:

    InitGraphics();


    // create the display structures that will not change:

    InitLists();


    // init all the global variables used by Display( ):
    // this will also post a redisplay

    Reset();


    // setup all the user interface stuff:

    InitMenus();


    // draw the scene once and wait for some interaction:
    // (this will never return)

    if (PlaySound){
        system("afplay audio.mp3 &");
        isPlayingSound = 1;
        PlayingTime = time(NULL);
    }

    glutFullScreen();
    glutSetWindow(MainWindow);
    glutMainLoop();


    // this is here to make the compiler happy:

    return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate() {
    // put animation stuff in here -- change some global variables
    // for Display( ) to find:

    // force a call to Display( ) next time it is convenient:

    int ms = glutGet(GLUT_ELAPSED_TIME);    // milliseconds
    ms %= MS_IN_THE_ANIMATION_CYCLE;
    Time = (float) ms / (float) MS_IN_THE_ANIMATION_CYCLE;        // [ 0., 1. )
    if (DebugOn) {
        printf("Time = %f\n", Time);
    }

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

// draw the complete scene:

void
Display() {
    if (DebugOn != 0) {
        fprintf(stderr, "Display\n");
    }

    // set which window we want to do the graphics into:

    glutSetWindow(MainWindow);


    // erase the background:

    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable( GL_DEPTH_TEST );


    // specify shading to be flat:

    glShadeModel(GL_SMOOTH);


    // set the viewport to a square centered in the window:

    GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
    GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
//    GLsizei v = vx < vy ? vx : vy;            // minimum dimension
//    GLint xl = (vx - v) / 2;
//    GLint yb = (vy - v) / 2;
//    glViewport(xl, yb, v, v);
    glViewport(0, 0, vx, vy);

    // set the viewing volume:
    // remember that the Z clipping  values are actually
    // given as DISTANCES IN FRONT OF THE EYE
    // USE gluOrtho2D( ) IF YOU ARE DOING 2D !

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (WhichProjection == ORTHO)
        glOrtho(-3., 3., -3., 3., 0.1, 1000.);
    else {
        gluPerspective(90., vx * 1.f / vy, 0.1, 1000.);
//        gluPerspective(90., 1., 0.1, 1000.);
    }


    // place the objects into the scene:

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // set the eye position, look-at position, and up-vector:
    if (WhichView == 9) {
        if (ScaleOrbitOn) {
            gluLookAt(-5., 25., 25., 0., 0., 0., 0, 1., 0.);
        } else {
            gluLookAt(-10., 60., 60., 0., 0., 0., 0, 1., 0.);
        }

    }
    else if (WhichView == 8)
        gluLookAt(0., 1., 6., 0., 0., 0., 0, 1., 0.);
    else
        gluLookAt(cam_x, cam_y, cam_z, target_x, target_y, target_z, 0, 1., 0.);

    // rotate the scene:

    glRotatef((GLfloat) Yrot, 0., 1., 0.);
    glRotatef((GLfloat) Xrot, 1., 0., 0.);


    // uniformly scale the scene:

    if (Scale < MINSCALE)
        Scale = MINSCALE;
    glScalef((GLfloat) Scale, (GLfloat) Scale, (GLfloat) Scale);


    // set the fog parameters:

    if (DepthCueOn != 0) {
        glFogi(GL_FOG_MODE, FOGMODE);
        glFogfv(GL_FOG_COLOR, FOGCOLOR);
        glFogf(GL_FOG_DENSITY, FOGDENSITY);
        glFogf(GL_FOG_START, FOGSTART);
        glFogf(GL_FOG_END, FOGEND);
        glEnable(GL_FOG);
    } else {
        glDisable(GL_FOG);
    }


    // possibly draw the axes:

    if (AxesOn != 0) {
        glDisable(GL_LIGHTING);
        glColor3fv(&Colors[WhichColor][0]);
        glCallList(AxesList);
    }

    // End set lighting

    // since we are using glScalef( ), be sure normals get unitized:
    glEnable(GL_NORMALIZE);

    // Main Drawing going here
    // draw the current object:


    if (DebugOn) {
        fprintf(stderr, "orbtial_speed_constant=%f\tself_rotate_speed=%f\n", orbital_speed_constant, self_rotate_speed);
    }

    if (PlaySound == 0 && isPlayingSound == 1){
        system("killall afplay");
        isPlayingSound = 0;
    }

    time_t current_time = time(NULL);
    if ((PlaySound == 1 && isPlayingSound == 0) || current_time - PlayingTime >= 182){
        system("afplay audio.mp3 &");
        isPlayingSound = 1;
        PlayingTime = current_time;
    }


    ScaleView();

    DrawObjects();

    WriteInstruction();


    // swap the double-buffered framebuffers:

    glutSwapBuffers();


    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !

    glFlush();


}

void WriteInstruction(){
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D( 0., 100.,     0., 100. );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glColor3f( 1., 1., 1. );

    if (ShowInstructions) {
        string texts[27] = {"------INSTRUCTIONS------",
                            "Left Click for adjust View",
                            "Right Click for more options",
                            "Middle Click for zoom in/out",
                            "KEY_LEFT for decrease self rotate speed",
                            "KEY_RIGHT for increase self rotate speed",
                            "KEY_UP for decrease orbital speed",
                            "KEY_DOWN for increase orbital speed",
                            "Press 'h' for hide this instructions",
                            "Press 'q' for quit",
                            "Press 'o' for toggle Scale Orbit or not",
                            "Press 'd' for toggle Scale Diameter or not",
                            "Press 'b' for toggle Show Planets' orbits",
                            "Press 'n' for toggle Show Planets' names",
                            "Press 'f' for freeze animation",
                            "Press 'r' for reset View",
                            "Press 's' for toggle Playing sound",
                            "Press '0' for view from Sun",
                            "Press '1' for view from Earth",
                            "Press '2' for view from Moon",
                            "Press '3' for view from Jupiter",
                            "Press '4' for view from Saturn",
                            "Press '5' for view from Neptune",
                            "Press '6' for view from Pluto",
                            "Press '7' for view from Comet",
                            "Press '8' for view from Closer View",
                            "Press '9' for view from Overall View",
        };
        for (int i = 0; i < 27; i++) {
            DoRasterString2(1., 1.55 * (27 - i), 0., (char *) texts[i].c_str());
        }
    } else {
        DoRasterString2(1., 5, 0., "Click Screen and Press 'h' for instructions");
    }
}


void ScaleView(){
    if (ScaleDiameterOn == 1) {
        jupiter_diameter = log(1120.) / log(10.); //speical values
        saturn_diameter = log(945.) / log(10.);
        uranus_diameter = log(400.) / log(10.);
        neptune_diameter = log(388.) / log(10.);
    } else {
        jupiter_diameter = 11.2; //speical values
        saturn_diameter = 9.45;
        uranus_diameter = 4.;
        neptune_diameter = 3.88;
        comet_diameter = 0.05;
    }

    if (ScaleOrbitOn == 1){
        earth_orbital_radius = 1.f * 3; // AU * double
        moon_orbital_radius = 0.5; // Special value for moon
        mercury_orbital_radius = 0.39f * 3;
        venus_orbital_radius = 0.72f * 3;
        mars_orbital_radius = 1.52f * 3;
        comet_orbital_radius = 1.29f * 3;
        jupiter_orbital_radius = log(5.2f) / log(1.3); // special values
        saturn_orbital_radius = log(9.58f) / log(1.3);
        uranus_orbital_radius = log(19.23f) / log(1.3);
        neptune_orbital_radius = log(30.1f) / log(1.3);
        pluto_orbital_radius = log(39.52f) / log(1.3);
    } else {
        earth_orbital_radius = 1.f; // AU * double
        moon_orbital_radius = 0.5; // Special value for moon
        mercury_orbital_radius = 0.39f;
        venus_orbital_radius = 0.72f;
        mars_orbital_radius = 1.52f;
        comet_orbital_radius = 1.29f;
        jupiter_orbital_radius = 5.9; // special values
        saturn_orbital_radius = 9.58;
        uranus_orbital_radius = 19.23;
        neptune_orbital_radius = 30.1;
        pluto_orbital_radius = 39.52;
    }
}

float orbital_speed(float orbital_radius){
    float result = Time * 360.f * orbital_speed_constant / sqrt(pow(orbital_radius,3));

    return result;
}

void set_texture(GLuint texture1, GLuint texture2){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glActiveTexture( GL_TEXTURE1);
    glBindTexture( GL_TEXTURE_2D, texture2 );
}

void write_text(char * text){
    glPushMatrix();
    if (show_planet_names) {
        Pattern->Use(0);
        glColor3f(0., 1., 1.);
        if (strcmp(text, "Sun") == 0){
            DoRasterString(0., 1.3, 0., text);
        } else if (strcmp(text, "Jupiter") || strcmp(text, "Saturn")){
            if (ScaleDiameterOn){
                DoRasterString(0., 0.5, 0., text);
            } else {
                DoRasterString(0., 1., 0., text);
            }
        }
        else {
            DoRasterString(0., 0.5, 0., text);
        }
        glColor3f(1., 1., 1.);
        Pattern->Use();
    }
    glPopMatrix();
}

void Transform(string planet, glm::mat4 &modelMatrix){

    if (planet == "comet"){
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1., 1., 0.5));
        modelMatrix = glm::rotate(modelMatrix, D2R * 20.f, glm::vec3(0., 0., -1.));


        glScalef(1., 1., 0.5);
        glRotatef(20, 0, 0, -1.);

        if (ScaleOrbitOn == 1){
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-5, 0., 0.));
            glTranslatef(-5, 0., 0.);
        } else {
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 0., 0.));
            glTranslatef(-1, 0., 0.);
        }


    } else {

        modelMatrix = glm::scale(modelMatrix, glm::vec3(1., 1., 0.8));
        glScalef(1., 1., 0.8);

        if (planet == "pluto") {
            modelMatrix = glm::rotate(modelMatrix, D2R * 10.f, glm::vec3(0., 0., 1.));
            glRotatef(10, 0, 0, 1.);
        }

        if (ScaleOrbitOn == 1){
            if (planet.length() % 2 == 1) {
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5, 0., 0.));
                glTranslatef(0.5, 0., 0.);
            } else {
                modelMatrix = glm::translate(modelMatrix, glm::vec3(1.5, 0., 0.));
                glTranslatef(1.5, 0., 0.);
            }
        } else {
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.25, 0., 0.));
            glTranslatef(0.25, 0., 0.);
        }
    }
}


void DrawOrbit(float r, string planet){

    if (Orbit_On) {
        glPushMatrix();
        glm::mat4 a = glm::mat4(1.f);
        Transform(planet, a);
        int num_segments = 1000;
        float theta = 2 * 3.1415926 / float(num_segments);
        float c = cosf(theta);//precalculate the sine and cosine
        float s = sinf(theta);
        float t;

        float x = r * orbit_radius_const;//we start at angle = 0
        float y = 0;

        glBegin(GL_LINE_LOOP);
        for (int ii = 0; ii < num_segments; ii++) {

            glColor3f(.3, .3, .3);

            glVertex3f(x, 0., y);//output vertex

            //apply the rotation matrix
            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }
        glEnd();

        glPopMatrix();
    }
}


void DrawImage(void) {

    glDisable(GL_DEPTH_TEST);
    set_texture(stars, stars);
    GLsizei vx = 100;
    GLsizei vy = 100;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity( );
    glOrtho( -vx, vx, -vy, vy, -1, 1 );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity( );
    glColor3f( 1., 1., 1. );
    // Draw a textured quad
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex3f(-vx, -vy, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-vx, vy, 0);
        glTexCoord2f(1, 1);
        glVertex3f(vx, vy, 0);
        glTexCoord2f(1, 0);
        glVertex3f(vx, -vy, 0);
    }
    glEnd();
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}


void DrawRing(GLuint texture, float radius1, float radius2){
    glPushMatrix();
    set_texture(texture, texture);
    int num_segments = 100;
    if (texture == uranus_ring){
        glRotatef(90, 1., 0., 0.);
    } else {
        glRotatef(-20, 1., 0., 0.);
    }

    for (int i = 0; i < num_segments; i++) {
        float theta = 2 * M_PI * float(i) / float(num_segments);//get the current angle
        float theta2 = 2 * M_PI * float(i+1) / float(num_segments);//get the current angle

        float x1 = radius1 * cosf(theta);//calculate the x component
        float y1 = radius1 * sinf(theta);//calculate the y component

        float x2 = radius1 * cosf(theta2);//calculate the x component
        float y2 = radius1 * sinf(theta2);//calculate the y component

        float x3 = radius2 * cosf(theta);//calculate the x component
        float y3 = radius2 * sinf(theta);//calculate the y component

        float x4 = radius2 * cosf(theta2);//calculate the x component
        float y4 = radius2 * sinf(theta2);//calculate the y component

        glBegin(GL_QUAD_STRIP);
        {
            glTexCoord2f(0, 0);
            glVertex3f(x1, 0, y1);
            glTexCoord2f(0, 1);
            glVertex3f(x2, 0, y2);
            glTexCoord2f(1, 1);
            glVertex3f(x3, 0, y3);
            glTexCoord2f(1, 0);
            glVertex3f(x4, 0, y4);
        }
        glEnd();
    }

    glPopMatrix();
}

void PutCamera(glm::mat4 modelMatrix, string planet){
//    printf("%s\n", glm::to_string(modelMatrix).c_str() ) ;
    glDisable(GL_DEPTH_TEST);

    // case for jupiter, saturn, neptune, pluto
    glm::vec4 cam(-.2, 0., -.2, 1.);
    glm::vec4 target(0, 0, 0, 0.);

    if (planet == "moon"){
        cam = glm::vec4(-.2, 0., 0., 1.);
        target = glm::vec4(-.4, 0, 0, 1.);
    } else if (planet == "earth" || planet == "sun"){
        cam = glm::vec4(.2, 0., 0., 1.);
        target = glm::vec4(.4, 0, 0, 1.);
    } else if (planet == "comet"){
        cam = glm::vec4(5., 5., 5., 1.);
        target = glm::vec4(.0, 0, 0, 1.);
    }

    cam = modelMatrix * cam;

    cam_x = cam.x; cam_y = cam.y; cam_z = cam.z;

    target = modelMatrix * target;

    target_x = target.x; target_y = target.y; target_z = target.z;

//    DoRasterString(cam_x, cam_y, cam_z, "X");
//    DoRasterString(target_x, target_y, target_z, "O");

    if (DebugOn){
        fprintf(stderr, "cam(x,y,z) = (%f,%f,%f)\ttarget(x,y,z)=(%f,%f,%f)\n",
                cam_x, cam_y, cam_z, target_x, target_y, target_z);
    }
    glEnable(GL_DEPTH_TEST);

}


void DrawObjects() {

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMatrixMode(GL_MODELVIEW);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, MulArray3(.3f, White));
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    //Draw Background

    Pattern->Use();

    DrawImage();
    glm::mat4 modelMatrix = glm::mat4(1.f);// Store the model matrix
    glm::mat4 mattemp = glm::mat4(1.f);

    // Draw Earth + Moon
    glPushMatrix();
    {

        glm::mat4 modelMatrix1 = modelMatrix;
        Transform("earth", modelMatrix1);

        modelMatrix1 = glm::rotate(modelMatrix1, D2R*orbital_speed(earth_orbital_radius), glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(orbit_radius_const*earth_orbital_radius, 0., 0.));

        glRotatef(orbital_speed(earth_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*earth_orbital_radius, 0., 0.);

        // Draw Earth

        glPushMatrix();
        {
            glm::mat4 modelMatrix2 = modelMatrix1;
            modelMatrix2 = glm::rotate(modelMatrix2, D2R*Time * 360 * self_rotate_speed, glm::vec3(0.,1.,0.));

            glRotatef(Time * 360 * self_rotate_speed, 0., 1., 0.);

            if (WhichView == 1){
                PutCamera(modelMatrix2, "earth");
            }

            Pattern->SetUniformVariable("uTime", Time);
            Pattern->SetUniformVariable("earth_day_tex", 0);
            Pattern->SetUniformVariable("earth_night_tex", 1);

            set_texture(earth_day, earth_night);

            MjbSphere(0.2, 100, 50);
        }
        glPopMatrix();

        write_text("Earth");

        // Draw Moon
        glPushMatrix();
        {
            glm::mat4 modelMatrix2 = modelMatrix1;
            modelMatrix2 = glm::rotate(modelMatrix2, D2R*orbital_speed(moon_orbital_radius), glm::vec3(0.,1.,0.));
            modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(moon_orbital_radius, 0., 0.));
            modelMatrix2 = glm::scale(modelMatrix2, glm::vec3(moon_diameter));

            glRotatef(orbital_speed(moon_orbital_radius), 0., 1., 0.);
            glTranslatef(moon_orbital_radius, 0., 0.);
            write_text("Moon");
            glScalef(moon_diameter, moon_diameter, moon_diameter);

            if (WhichView == 2){
                PutCamera(modelMatrix2, "moon");
            }

            set_texture(moon, moon);
            glCallList(moon_list);
        }
        glPopMatrix();
    }
    glPopMatrix();

    glActiveTexture(GL_TEXTURE0);// very important!!!!

    // Draw Sun
    glPushMatrix();
    {
        glm::mat4 modelMatrix1 = modelMatrix;

        modelMatrix1 = glm::rotate(modelMatrix1, -D2R*Time * 360 * self_rotate_speed/5.f, glm::vec3(0,-1,0));


        glDisable(GL_LIGHTING);

        glRotatef(Time * 360 * self_rotate_speed/5., 0., 1., 0.);
        write_text("Sun");

        if (ScaleOrbitOn == 1) {
            modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(5.));
            glScalef(5., 5., 5.);
        } else {
            modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(2.));
            glScalef(2., 2., 2.);
        }

        if (WhichView == 0) {
            PutCamera(modelMatrix1, "sun");
        }

        set_texture(sun, sun);

        glCallList(sun_list);
        glEnable(GL_LIGHTING);
        SetPointLight(GL_LIGHT0, 0., 0., 0., 1., 1., 1.);

    }
    glPopMatrix();

    //Draw Mercury
    glPushMatrix();
    {
        Transform("mercury", mattemp);
        glRotatef(orbital_speed(mercury_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*mercury_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Mercury");
        glScalef(mercury_diameter, mercury_diameter, mercury_diameter);
        glRotatef(Time * 360 * self_rotate_speed/10., 0., 1., 0.);
        set_texture(mercury, mercury);
        glCallList(mercury_list);
    }
    glPopMatrix();

    //Draw Venus
    glPushMatrix();
    {
        Transform("venus", mattemp);
        glRotatef(orbital_speed(venus_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*venus_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Venus");
        glScalef(venus_diameter, venus_diameter, venus_diameter);
        glRotatef(Time * 360 * self_rotate_speed/5.0, 0., 1., 0.);
        set_texture(venus, venus);
        glCallList(venus_list);
    }
    glPopMatrix();

    //Draw Mars
    glPushMatrix();
    {
        Transform("mars", mattemp);
        glRotatef(orbital_speed(mars_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*mars_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Mars");
        glScalef(mars_diameter, mars_diameter, mars_diameter);
        glRotatef(Time * 360 * self_rotate_speed, 0., 1., 0.);
        set_texture(mars, mars);
        glCallList(mars_list);
    }
    glPopMatrix();

    //Draw Jupiter
    glPushMatrix();
    {
        glm::mat4 modelMatrix1 = modelMatrix;
        Transform("jupiter", modelMatrix1);

        modelMatrix1 = glm::rotate(modelMatrix1, D2R*orbital_speed(jupiter_orbital_radius), glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(orbit_radius_const*jupiter_orbital_radius, 0., 0.));
        modelMatrix1 = glm::rotate(modelMatrix1, D2R*Time * 360, glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(jupiter_diameter));

        glRotatef(orbital_speed(jupiter_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*jupiter_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Jupiter");
        glScalef(jupiter_diameter, jupiter_diameter, jupiter_diameter);
        glRotatef(Time * 360 * self_rotate_speed, 0., 1., 0.);

        if (WhichView == 3){
            PutCamera(modelMatrix1, "jupiter");
        }

        set_texture(jupiter, jupiter);
        glCallList(jupiter_list);

    }
    glPopMatrix();

    //Draw Saturn
    glPushMatrix();
    {
        glm::mat4 modelMatrix1 = modelMatrix;
        Transform("saturn", modelMatrix1);

        modelMatrix1 = glm::rotate(modelMatrix1, D2R*orbital_speed(saturn_orbital_radius), glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(orbit_radius_const*saturn_orbital_radius, 0., 0.));
        modelMatrix1 = glm::rotate(modelMatrix1, D2R*Time * 360, glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(saturn_diameter));

        glRotatef(orbital_speed(saturn_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*saturn_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);

        write_text("Saturn");
        glScalef(saturn_diameter, saturn_diameter, saturn_diameter);
        //add saturn ring here
        DrawRing(saturn_ring, 0.25, 0.4);
        glRotatef(Time * 360 * self_rotate_speed, 0., 1., 0.);

        if (WhichView == 4){
            PutCamera(modelMatrix1, "saturn");
        }

        set_texture(saturn, saturn);
        glCallList(saturn_list);
    }
    glPopMatrix();

    //Draw Uranus
    glPushMatrix();
    {
        Transform("uranus", mattemp);

        glRotatef(orbital_speed(uranus_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*uranus_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Uranus");
        glScalef(uranus_diameter, uranus_diameter, uranus_diameter);
        DrawRing(uranus_ring, 0.25, 0.3);
        glRotatef(Time * 360 * self_rotate_speed, 0., 1., 0.);

        set_texture(uranus, uranus);
        glCallList(uranus_list);
    }
    glPopMatrix();

    //Draw Neptune
    glPushMatrix();
    {
        glm::mat4 modelMatrix1 = modelMatrix;
        Transform("neptune", modelMatrix1);

        modelMatrix1 = glm::rotate(modelMatrix1, D2R*orbital_speed(neptune_orbital_radius), glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(orbit_radius_const*neptune_orbital_radius, 0., 0.));
        modelMatrix1 = glm::rotate(modelMatrix1, D2R*Time * 360, glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(neptune_diameter));

        glRotatef(orbital_speed(neptune_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*neptune_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Neptune");
        glScalef(neptune_diameter, neptune_diameter, neptune_diameter);
        glRotatef(Time * 360 * self_rotate_speed, 0., 1., 0.);

        if (WhichView == 5){
            PutCamera(modelMatrix1, "neptune");
        }

        set_texture(neptune, neptune);
        glCallList(neptune_list);
    }
    glPopMatrix();

    //Draw Pluto
    glPushMatrix();
    {
        glm::mat4 modelMatrix1 = modelMatrix;
        Transform("pluto", modelMatrix1);

        modelMatrix1 = glm::rotate(modelMatrix1, D2R*orbital_speed(pluto_orbital_radius), glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(orbit_radius_const*pluto_orbital_radius, 0., 0.));
        modelMatrix1 = glm::rotate(modelMatrix1, D2R*Time * 360, glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(pluto_diameter));

        glRotatef(orbital_speed(pluto_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*pluto_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Pluto");
        glScalef(pluto_diameter, pluto_diameter, pluto_diameter);
        glRotatef(Time * 360 * self_rotate_speed, 0., 1., 0.);

        if (WhichView == 6){
            PutCamera(modelMatrix1, "pluto");
        }

        set_texture(pluto, pluto);
        glCallList(pluto_list);
    }
    glPopMatrix();


    //Draw Comet
    glPushMatrix();
    {
        glm::mat4 modelMatrix1 = modelMatrix;
        Transform("comet", modelMatrix1);

        modelMatrix1 = glm::rotate(modelMatrix1, D2R*orbital_speed(comet_orbital_radius), glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(orbit_radius_const*comet_orbital_radius, 0., 0.));
        modelMatrix1 = glm::rotate(modelMatrix1, D2R*Time * 360, glm::vec3(0., 1., 0.));
        modelMatrix1 = glm::scale(modelMatrix1, glm::vec3(comet_diameter));

        glRotatef(orbital_speed(comet_orbital_radius), 0., 1., 0.);
        glTranslatef(orbit_radius_const*comet_orbital_radius, 0., 0.);
        glRotatef(Time * 360, 0., 1., 0.);
        write_text("Comet 67P");
        glScalef(comet_diameter, comet_diameter, comet_diameter);
        glRotatef(Time * 360 * self_rotate_speed/30., 0., 1., 0.);

        if (WhichView == 7){
            PutCamera(modelMatrix1, "comet");
        }

        set_texture(moon, moon);
        obj.Draw();
    }
    glPopMatrix();


    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    Pattern->Use(0);

    DrawOrbit(mercury_orbital_radius, "mercury");
    DrawOrbit(venus_orbital_radius, "venus");
    DrawOrbit(earth_orbital_radius, "earth");
    DrawOrbit(mars_orbital_radius, "mars");
    DrawOrbit(jupiter_orbital_radius, "jupiter");
    DrawOrbit(saturn_orbital_radius, "saturn");
    DrawOrbit(uranus_orbital_radius, "uranus");
    DrawOrbit(neptune_orbital_radius, "neptune");
    DrawOrbit(pluto_orbital_radius, "pluto");
    DrawOrbit(comet_orbital_radius, "comet");
}

void DoTextureMenu(int id) {
    Orbit_On = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void DoFilterMenu(int id) {
    ScaleDiameterOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
DoAxesMenu(int id) {
    AxesOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
DoColorMenu(int id) {
    WhichColor = id - RED;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
DoDebugMenu(int id) {
    DebugOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
DoDepthMenu(int id) {
    DepthCueOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void DoViewMenu(int id) {

    WhichView = id;
    if (WhichView == 2){
        orbital_speed_constant = 10;
    } else {
        orbital_speed_constant = 100;
    }

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void DoRotateMenu(int id) {

    ScaleOrbitOn = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

// utility to create an array from 3 separate values:

float *Array3(float a, float b, float c) {
    static float array[4];

    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;

}

// utility to create an array from a multiplier and an array:

float *MulArray3(float factor, float array0[3]) {
    static float array[4];

    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;

}

void SetMaterial(float r, float g, float b, float shininess) {
    glMaterialfv(GL_BACK, GL_EMISSION, Array3(0., 0., 0.));
    glMaterialfv(GL_BACK, GL_AMBIENT, MulArray3(.4f, White));
    glMaterialfv(GL_BACK, GL_DIFFUSE, MulArray3(1., White));
    glMaterialfv(GL_BACK, GL_SPECULAR, Array3(0., 0., 0.));
    glMaterialf(GL_BACK, GL_SHININESS, 2.f);

    glMaterialfv(GL_FRONT, GL_EMISSION, Array3(0., 0., 0.));
    glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r, g, b));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
    glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

}

void SetPointLight(int ilight, float x, float y, float z, float r, float g, float b) {
    glLightfv(ilight, GL_POSITION, Array3(x, y, z));
    glLightfv(ilight, GL_AMBIENT, Array3(0.1, 0.1, 0.1));
    glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
    glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
    glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
    glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
    glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);

}

// main menu callback:

void
DoMainMenu(int id) {
    switch (id) {
        case RESET:
            Reset();
            break;

        case QUIT:
            // gracefully close out the graphics:
            // gracefully close the graphics window:
            // gracefully exit the program:
            obj.Release();
            system("killall afplay");
            glutSetWindow(MainWindow);
            glFinish();
            glutDestroyWindow(MainWindow);
            exit(0);
            break;

        default:
            if (DebugOn)
                fprintf(stderr, "Don't know what to do with Main Menu ID %d\n", id);
    }

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void ShowEarthMenu(int id) {
    show_planet_names = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void
DoProjectMenu(int id) {
    PlaySound = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

void
ShowInstructionsMenu(int id) {
    ShowInstructions = id;

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// use glut to display a string of characters using a raster font:

void
DoRasterString(float x, float y, float z, char *s) {
    glRasterPos3f((GLfloat) x, (GLfloat) y, (GLfloat) z);

    char c;            // one character to print
    for (; (c = *s) != '\0'; s++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, c);
    }
}

void
DoRasterString2(float x, float y, float z, char *s) {
    glRasterPos3f((GLfloat) x, (GLfloat) y, (GLfloat) z);

    char c;            // one character to print
    for (; (c = *s) != '\0'; s++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString(float x, float y, float z, float ht, char *s) {
    glPushMatrix();
    glTranslatef((GLfloat) x, (GLfloat) y, (GLfloat) z);
    float sf = ht / (119.05f + 33.33f);
    glScalef((GLfloat) sf, (GLfloat) sf, (GLfloat) sf);
    char c;            // one character to print
    for (; (c = *s) != '\0'; s++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    }
    glPopMatrix();
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds() {
    // get # of milliseconds since the start of the program:

    int ms = glutGet(GLUT_ELAPSED_TIME);

    // convert it to seconds:

    return (float) ms / 1000.f;
}


// initialize the glui window:

void
InitMenus() {
    glutSetWindow(MainWindow);

    int debugmenu = glutCreateMenu(DoDebugMenu);
    glutAddMenuEntry("Off", 0);
    glutAddMenuEntry("On", 1);

    int viewmenu = glutCreateMenu(DoViewMenu);
    glutAddMenuEntry("Sun", 0);
    glutAddMenuEntry("Earth", 1);
    glutAddMenuEntry("Moon", 2);
    glutAddMenuEntry("Jupiter", 3);
    glutAddMenuEntry("Saturn", 4);
    glutAddMenuEntry("Neptune", 5);
    glutAddMenuEntry("Pluto", 6);
    glutAddMenuEntry("Comet", 7);
    glutAddMenuEntry("Near View", 8);
    glutAddMenuEntry("Overall View", 9);

    int texturemenu = glutCreateMenu(DoTextureMenu);
    glutAddMenuEntry("ON", 1);
    glutAddMenuEntry("OFF", 0);

    int filtermenu = glutCreateMenu(DoFilterMenu);
    glutAddMenuEntry("ON", 1);
    glutAddMenuEntry("OFF", 0);

    int rotatemenu = glutCreateMenu(DoRotateMenu);
    glutAddMenuEntry("ON", 1);
    glutAddMenuEntry("OFF", 0);

    int showearthmenu = glutCreateMenu(ShowEarthMenu);
    glutAddMenuEntry("ON", 1);
    glutAddMenuEntry("OFF", 0);

    int playsoundhmenu = glutCreateMenu(DoProjectMenu);
    glutAddMenuEntry("Play", 1);
    glutAddMenuEntry("Stop", 0);

    int showintructionmenu = glutCreateMenu(ShowInstructionsMenu);
    glutAddMenuEntry("Play", 1);
    glutAddMenuEntry("Stop", 0);

    int mainmenu = glutCreateMenu(DoMainMenu);
    glutAddSubMenu("Show Orbits", texturemenu);
    glutAddSubMenu("Scale Diameter", filtermenu);
    glutAddSubMenu("Scale Orbit", rotatemenu);
    glutAddSubMenu("Play Sound", playsoundhmenu);
    glutAddSubMenu("Show Planets' names", showearthmenu);
    glutAddSubMenu("Show Instructions", showintructionmenu);
    glutAddSubMenu("View From", viewmenu);
    glutAddMenuEntry("Reset", RESET);
    glutAddSubMenu("Debug", debugmenu);
    glutAddMenuEntry("Quit", QUIT);

// attach the pop-up menu to the right mouse button:

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void load_texture(string texture_name, GLuint &texture){
    // Set texture mapping in here
    int width, height;
    int level = 0, ncomps = 4, border = 0;
    unsigned  char* texture_tex;
    string file_name = string("textures/")+texture_name+string(".jpg");

    texture_tex = SOIL_load_image(file_name.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, level, ncomps, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, texture_tex);
}


void load_Earth_texture(){
    // Set texture mapping in here
    int width, height;
    int level = 0, ncomps = 4, border = 0;
    unsigned  char* earth_day_tex;
    unsigned  char* earth_night_tex;

    earth_day_tex = BmpToTexture("textures/earth_day.bmp", &width, &height );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &earth_day);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earth_day);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, level, ncomps, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, earth_day_tex);

    earth_night_tex = BmpToTexture("textures/earth_night.bmp", &width, &height);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &earth_night);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, earth_night);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, level, ncomps, width, height, border, GL_RGB, GL_UNSIGNED_BYTE, earth_night_tex);
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics() {
    // request the display modes:
    // ask for red-green-blue-alpha color, double-buffering, and z-buffering:

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // set the initial window configuration:

    glutInitWindowPosition(0, 0);
    glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);

    // open the window and set its title:

    MainWindow = glutCreateWindow(WINDOWTITLE);
    glutSetWindowTitle(WINDOWTITLE);

    // set the framebuffer clear values:

    glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);

    // setup the callback functions:
    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc	-- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // SpaceballMotionFunc -- handle spaceball translation
    // SpaceballRotateFunc -- handle spaceball rotation
    // SpaceballButtonFunc -- handle spaceball button hits
    // ButtonBoxFunc -- handle button box hits
    // DialsFunc -- handle dial rotations
    // TabletMotionFunc -- handle digitizing tablet motion
    // TabletButtonFunc -- handle digitizing tablet button hits
    // MenuStateFunc -- declare when a pop-up menu is in use
    // TimerFunc -- trigger something to happen a certain time from now
    // IdleFunc -- what to do when nothing else is going on

    glutSetWindow(MainWindow);
    glutDisplayFunc(Display);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(Keyboard);
    glutMouseFunc(MouseButton);
    glutMotionFunc(MouseMotion);
    glutPassiveMotionFunc(NULL);
    glutVisibilityFunc(Visibility);
    glutEntryFunc(NULL);
    glutSpecialFunc(SpecialKey);
    glutSpaceballMotionFunc(NULL);
    glutSpaceballRotateFunc(NULL);
    glutSpaceballButtonFunc(NULL);
    glutButtonBoxFunc(NULL);
    glutDialsFunc(NULL);
    glutTabletMotionFunc(NULL);
    glutTabletButtonFunc(NULL);
    glutMenuStateFunc(NULL);
    glutTimerFunc(-1, NULL, 0);
    glutIdleFunc(Animate);

    // init glew (a window must be open to do this):

#ifdef WIN32
    GLenum err = glewInit( );
    if( err != GLEW_OK )
    {
        fprintf( stderr, "glewInit Error\n" );
    }
    else
        fprintf( stderr, "GLEW initialized OK\n" );
    fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

    Pattern = new GLSLProgram( );
    bool valid = Pattern->Create( "pattern.vert",  "pattern.frag" );
    if( ! valid )
    {
        fprintf( stderr, "Shader cannot be created!\n" );
        DoMainMenu( QUIT );
    }
    else
    {
        fprintf( stderr, "Shader created.\n" );
    }
    Pattern->SetVerbose( false );

}

void InitObjects(GLuint &list, GLuint &texture, string texture_name){
    load_texture(texture_name, texture);
    list = glGenLists(1);

    glNewList(list, GL_COMPILE);

    MjbSphere(0.2, 100, 50);

    glEndList();
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists() {

    load_Earth_texture();

    load_texture("stars", stars);
    load_texture("saturn_ring", saturn_ring);
    load_texture("uranus_ring", uranus_ring);

    InitObjects(sun_list, sun, "sun");
    InitObjects(moon_list, moon, "moon");
    InitObjects(mercury_list, mercury, "mercury");
    InitObjects(venus_list, venus, "venus");
    InitObjects(mars_list, mars, "mars");
    InitObjects(jupiter_list, jupiter, "jupiter");
    InitObjects(saturn_list, saturn, "saturn");
    InitObjects(uranus_list, uranus, "uranus");
    InitObjects(neptune_list, neptune, "neptune");
    InitObjects(pluto_list, pluto, "pluto");

    obj.Load("comet.obj");
    // create the axes:

    AxesList = glGenLists(1);
    glNewList(AxesList, GL_COMPILE);
    glLineWidth(AXES_WIDTH);
    Axes(2.);
    glLineWidth(1.);
    glEndList();
}

float
Dot(float v1[3], float v2[3]) {
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void
Cross(float v1[3], float v2[3], float vout[3]) {
    float tmp[3];
    tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
    tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
    tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
    vout[0] = tmp[0];
    vout[1] = tmp[1];
    vout[2] = tmp[2];
}

float
Unit(float vin[3], float vout[3]) {
    float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
    if (dist > 0.0) {
        dist = sqrt(dist);
        vout[0] = vin[0] / dist;
        vout[1] = vin[1] / dist;
        vout[2] = vin[2] / dist;
    } else {
        vout[0] = vin[0];
        vout[1] = vin[1];
        vout[2] = vin[2];
    }
    return dist;
}


// the keyboard callback:

void
Keyboard(unsigned char c, int x, int y) {
    if (DebugOn != 0)
        fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

    switch (c) {
        case 'q':
        case 'Q':
        case ESCAPE:
            DoMainMenu(QUIT);    // will not return here
            break;                // happy compiler

        case 'o':
        case 'O':
            ScaleOrbitOn = !ScaleOrbitOn;
            break;

        case 'h':
        case 'H':
            ShowInstructions = !ShowInstructions;
            break;

        case 'n':
        case 'N':
            show_planet_names = !show_planet_names;
            break;

        case 's':
        case 'S':
            PlaySound = !PlaySound;
            break;

        case 'd':
        case 'D':
            ScaleDiameterOn = !ScaleDiameterOn;
            break;

        case 'r':
        case 'R':
            Reset();
            break;

        case 'b':
        case 'B':
            Orbit_On = !Orbit_On;
            break;

        case 'f':
        case 'F':
            Freeze = !Freeze;
            if (Freeze)
                glutIdleFunc(NULL);
            else
                glutIdleFunc(Animate);
            break;

        case '0':
            WhichView = 0;
            break;

        case '1':
            WhichView = 1;
            break;

        case '2':
            WhichView = 2;
            break;
        case '3':
            WhichView = 3;
            break;
        case '4':
            WhichView = 4;
            break;
        case '5':
            WhichView = 5;
            break;
        case '6':
            WhichView = 6;
            break;
        case '7':
            WhichView = 7;
            break;
        case '8':
            WhichView = 8;
            break;
        case '9':
            WhichView = 9;
            break;


        default:
            fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
    }

    if (WhichView == 2){
        orbital_speed_constant = 10;
    } else {
        orbital_speed_constant = 100;
    }

    // force a call to Display( ):

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void
SpecialKey( int c, int x, int y) {
    if (DebugOn != 0)
        fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

    switch (c) {
        case GLUT_KEY_UP:
            orbital_speed_constant += 5;
            if (orbital_speed_constant > 200){
                orbital_speed_constant = 200;
            }
            break;
        case GLUT_KEY_DOWN:
            orbital_speed_constant -= 5;
            if (orbital_speed_constant < 0){
                orbital_speed_constant = 0;
            }
            break;
        case GLUT_KEY_LEFT:
            self_rotate_speed -= 5;
            if (self_rotate_speed < 0){
                self_rotate_speed = 0;
            }
            break;
        case GLUT_KEY_RIGHT:
            self_rotate_speed += 5;
            if (self_rotate_speed > 200){
                self_rotate_speed = 200;
            }
            break;

        default:
            fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
    }

    // force a call to Display( ):

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}

// called when the mouse button transitions down or up:

void
MouseButton(int button, int state, int x, int y) {
    int b = 0;            // LEFT, MIDDLE, or RIGHT

    if (DebugOn != 0)
        fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);


    // get the proper button bit mask:

    switch (button) {
        case GLUT_LEFT_BUTTON:
            b = LEFT;
            break;

        case GLUT_MIDDLE_BUTTON:
            b = MIDDLE;
            break;

        case GLUT_RIGHT_BUTTON:
            b = RIGHT;
            break;

        default:
            b = 0;
            fprintf(stderr, "Unknown mouse button: %d\n", button);
    }


    // button down sets the bit, up clears the bit:

    if (state == GLUT_DOWN) {
        Xmouse = x;
        Ymouse = y;
        ActiveButton |= b;        // set the proper bit
    } else {
        ActiveButton &= ~b;        // clear the proper bit
    }
}


// called when the mouse moves while a button is down:

void
MouseMotion(int x, int y) {
    if (DebugOn != 0)
        fprintf(stderr, "MouseMotion: %d, %d\n", x, y);

    int dx = x - Xmouse;        // change in mouse coords
    int dy = y - Ymouse;

    if ((ActiveButton & LEFT) != 0) {
        Xrot += (ANGFACT * dy);
        Yrot += (ANGFACT * dx);
    }

    if ((ActiveButton & MIDDLE) != 0) {
        Scale += SCLFACT * (float) (dx - dy);

        // keep object from turning inside-out or disappearing:

        if (Scale < MINSCALE)
            Scale = MINSCALE;
    }

    Xmouse = x;            // new current position
    Ymouse = y;


    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset() {
    ActiveButton = 0;
    show_planet_names = 1;
    PlaySound = 1;
    Orbit_On = 1;
    AxesOn = 0;
    DebugOn = 0;
    DepthCueOn = 0;
    ScaleOrbitOn = 1;
    Scale = 1.0;
    Distort = 0;
    WhichColor = WHITE;
    WhichProjection = PERSP;
    WhichView = 7;
    ScaleDiameterOn = 1;
    Xrot = Yrot = 0.;

    if (WhichView < 8) { // Not near view and overall view
        self_rotate_speed = 10;
    } else {
        self_rotate_speed = 100;
    }

    if (WhichView == 2) { // Moon
        orbital_speed_constant = 10;
    } else {
        orbital_speed_constant = 100;
    }
}


// called when user resizes the window:

void
Resize(int width, int height) {
    if (DebugOn != 0)
        fprintf(stderr, "ReSize: %d, %d\n", width, height);

    // don't really need to do anything since window size is
    // checked each time in Display( ):

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


// handle a change to the window's visibility:

void
Visibility(int state) {
    if (DebugOn != 0)
        fprintf(stderr, "Visibility: %d\n", state);

    if (state == GLUT_VISIBLE) {
        glutSetWindow(MainWindow);
        glutPostRedisplay();
    } else {
        // could optimize by keeping track of the fact
        // that the window is not visible and avoid
        // animating or redrawing it ...
    }
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[] = {
        0.f, 1.f, 0.f, 1.f
};

static float xy[] = {
        -.5f, .5f, .5f, -.5f
};

static int xorder[] = {
        1, 2, -3, 4
};

static float yx[] = {
        0.f, 0.f, -.5f, .5f
};

static float yy[] = {
        0.f, .6f, 1.f, 1.f
};

static int yorder[] = {
        1, 2, 3, -2, 4
};

static float zx[] = {
        1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[] = {
        .5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[] = {
        1, 2, 3, 4, -5, 6
};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes(float length) {
    glBegin(GL_LINE_STRIP);
    glVertex3f(length, 0., 0.);
    glVertex3f(0., 0., 0.);
    glVertex3f(0., length, 0.);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0., 0., 0.);
    glVertex3f(0., 0., length);
    glEnd();

    float fact = LENFRAC * length;
    float base = BASEFRAC * length;

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 4; i++) {
        int j = xorder[i];
        if (j < 0) {

            glEnd();
            glBegin(GL_LINE_STRIP);
            j = -j;
        }
        j--;
        glVertex3f(base + fact * xx[j], fact * xy[j], 0.0);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 5; i++) {
        int j = yorder[i];
        if (j < 0) {

            glEnd();
            glBegin(GL_LINE_STRIP);
            j = -j;
        }
        j--;
        glVertex3f(fact * yx[j], base + fact * yy[j], 0.0);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 6; i++) {
        int j = zorder[i];
        if (j < 0) {

            glEnd();
            glBegin(GL_LINE_STRIP);
            j = -j;
        }
        j--;
        glVertex3f(0.0, fact * zy[j], base + fact * zx[j]);
    }
    glEnd();

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb(float hsv[3], float rgb[3]) {
    // guarantee valid input:

    float h = hsv[0] / 60.f;
    while (h >= 6.) h -= 6.;
    while (h < 0.) h += 6.;

    float s = hsv[1];
    if (s < 0.)
        s = 0.;
    if (s > 1.)
        s = 1.;

    float v = hsv[2];
    if (v < 0.)
        v = 0.;
    if (v > 1.)
        v = 1.;

    // if sat==0, then is a gray:

    if (s == 0.0) {
        rgb[0] = rgb[1] = rgb[2] = v;
        return;
    }

    // get an rgb from the hue itself:

    float i = floor(h);
    float f = h - i;
    float p = v * (1.f - s);
    float q = v * (1.f - s * f);
    float t = v * (1.f - (s * (1.f - f)));

    float r, g, b;            // red, green, blue
    switch ((int) i) {
        case 0:
            r = v;
            g = t;
            b = p;
            break;

        case 1:
            r = q;
            g = v;
            b = p;
            break;

        case 2:
            r = p;
            g = v;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = v;
            break;

        case 4:
            r = t;
            g = p;
            b = v;
            break;

        case 5:
            r = v;
            g = p;
            b = q;
            break;
    }


    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}