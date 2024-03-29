// sdl2-gl.c (filename)
// SDL2 with OpenGL for 3D (OpenGL with "windowing" requires SDL2 (or SDL1) or e.g. freeGLUT (GLUT implementation) )

/* gcc sdl2-gl.c `sdl2-config --cflags --libs` -lopengl32 -lglu32 && ./a */

/*
https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html ||
SDL_Delay( 30 ); // added  ||

ported from SDL1 to SDL2
https://wiki.libsdl.org/MigrationGuide
OpenGL: If you were already using OpenGL directly, your migration is pretty simple. Change your SDL_SetVideoMode() call to SDL_CreateWindow() followed by SDL_GL_CreateContext(), and your SDL_GL_SwapBuffers() call to SDL_GL_SwapWindow(window). All the actual calls into the GL are exactly the same.

https://stackoverflow.com/questions/49585857/error-with-sdl-window

compiled with MSYS2/MinGW64 (provides pacman) ||
pacman -Fy sdl2-config ||
pacman -S --needed mingw-w64-x86_64-SDL2 # SDL2 : sdl2-config . not : (SDL1/SDL : sdl-config)
*/

/*
 * SDL OpenGL Tutorial.
 * (c) Michael Vance, 2000
 * briareos@lokigames.com
 *
 * Distributed under terms of the LGPL.
 */

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>

static GLboolean should_rotate = GL_TRUE;
static int scene = 0;

SDL_Window* window;


static void quit_tutorial( int code )
{
    /*
     * Quit SDL so we can release the fullscreen
     * mode and restore the previous video settings,
     * etc.
     */
    SDL_Quit( );

    /* Exit program. */
    exit( code );
}

static void handle_key_down( SDL_Keysym* keysym )
{
  static bool wireframe = false;

    /*
     * We're only interested if 'Esc' has
     * been presssed.
     *
     * EXERCISE:
     * Handle the arrow keys and have that change the
     * viewing position/angle.
     */
    switch( keysym->sym ) {
    case SDLK_ESCAPE:
        quit_tutorial( 0 );
        break;
    case SDLK_SPACE:
        should_rotate = !should_rotate;
        break;

    case SDLK_RETURN:
      scene++;
      scene%=3;
      break;

    case SDLK_w: // wire-frame toggle
      wireframe = !wireframe;
      glPolygonMode( GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL );
      break;

    default:
        break;
    }

}

static void process_events( void )
{
    /* Our SDL event placeholder. */
    SDL_Event event;

    /* Grab all the events off the queue. */
    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
        case SDL_KEYDOWN:
            /* Handle key presses. */
            handle_key_down( &event.key.keysym );
            break;
        case SDL_QUIT:
            /* Handle quit requests (like Ctrl-c). */
            quit_tutorial( 0 );
            break;
        }

    }

}

//-----------------------------------------

typedef struct {
  float array[3];
} vector;
typedef struct {
  vector* array;
  int count;
} vector_array;

typedef struct {
  int array[6];
} triangle;
typedef struct {
  triangle* array;
  int count;
} triangle_array;

typedef struct {
vector_array vertex_positions;
vector_array vertex_normals;
triangle_array mesh;
} model;

#include "parse.h" // parse model to load

void draw_triangle(triangle t, model m){

  vector vertex_normal, vertex_position;

  vertex_normal=m.vertex_normals.array[t.array[1]-1];
  glNormal3f(vertex_normal.array[0],vertex_normal.array[1],vertex_normal.array[2]);
  vertex_position=m.vertex_positions.array[t.array[0]-1];
  glVertex3f(vertex_position.array[0],vertex_position.array[1],vertex_position.array[2]);

  vertex_normal=m.vertex_normals.array[t.array[3]-1];
  glNormal3f(vertex_normal.array[0],vertex_normal.array[1],vertex_normal.array[2]);
  vertex_position=m.vertex_positions.array[t.array[2]-1];
  glVertex3f(vertex_position.array[0],vertex_position.array[1],vertex_position.array[2]);

  vertex_normal=m.vertex_normals.array[t.array[5]-1];
  glNormal3f(vertex_normal.array[0],vertex_normal.array[1],vertex_normal.array[2]);
  vertex_position=m.vertex_positions.array[t.array[4]-1];
  glVertex3f(vertex_position.array[0],vertex_position.array[1],vertex_position.array[2]);
}

void draw_model(model m){
  glBegin(GL_TRIANGLES);
    for(int i=0; i<m.mesh.count; i++)
    draw_triangle(m.mesh.array[i],m);
  glEnd();
}

model cube;

void draw_box(float scale){
  glPushMatrix();
  glScalef(scale,scale,scale);
  draw_model(cube);
  glPopMatrix();
}

// (cleaner) code import from gltest.cpp (part of https://fox-toolkit.org/)

// Draws a simple box using the given corners
void drawBox(GLfloat xmin, GLfloat ymin, GLfloat zmin, GLfloat xmax, GLfloat ymax, GLfloat zmax) {

  draw_box(1);

  return;

  if(1){
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(1.,0.,0.);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
  glEnd();
  }


  if(1){
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(-1.,0.,0.);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
  glEnd();
  }

  if(1){
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,1.,0.);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymax, zmax);
  glEnd();
  }

  if(0){
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,-1.,0.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymin, zmin);
  glEnd();
  }

  if(1){
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,1.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
  glEnd();
  }

  if(1){
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,-1.);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
  glEnd();
  }

}


// Draw the GL scene
void drawScene(model model1){
  //glDisable(GL_CULL_FACE);

  int w,h;
  SDL_GetWindowSize(window,&w,&h);

      /* Our angle of rotation. */
    static float angle = 0.0f;

    if( should_rotate ) {

        if( ++angle > 360.0f ) {
            angle = 0.0f;
        }

    }

  /// https://community.khronos.org/t/shininess/18327/10
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_SPECULAR);

  const GLfloat lightPosition[]={15.,10.,5.,1.};
  const GLfloat lightAmbient[]={.1f,.1f,.1f,1.};
  const GLfloat lightDiffuse[]={.9f,.9f,.9f,1.};
  const GLfloat redMaterial[]={1.,0.,0.,1.};
  const GLfloat blueMaterial[]={0.,0.,1.,1.};

  GLdouble width = w;
  GLdouble height = h;
  GLdouble aspect = height>0 ? width/height : 1.0;

  glViewport(0,0,width,height);

  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glEnable(GL_DEPTH_TEST);

  glDisable(GL_DITHER);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.,aspect,1.,100.);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(5.,10.,15.,0.,0.,0.,0.,1.,0.);

  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

  /// https://community.khronos.org/t/shininess/18327/10
  GLfloat white[] = { 0.5, 0.5, 0.5, 1 };
  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_SHININESS, white);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /// https://community.khronos.org/t/shininess/18327/10
  GLfloat specularColor [3] = {1,1,1};
  GLfloat shininess [1] = {50};
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularColor);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  glMaterialfv(GL_FRONT, GL_AMBIENT, blueMaterial);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blueMaterial);

  glPushMatrix();
  glRotated(angle, 0., 1., 0.);

  if(scene==0) draw_model(model1); // WIP
  if(scene==1 || scene==2) draw_box(1);
  //drawBox(-1, -1, -1, 1, 1, 1);

if(scene==1){

  /// https://community.khronos.org/t/shininess/18327/10
  GLfloat black[] = { 0,0,0, 1 };
  glLightfv(GL_LIGHT0, GL_SPECULAR, black);
  glLightfv(GL_LIGHT0, GL_SHININESS, black);

  glMaterialfv(GL_FRONT, GL_AMBIENT, redMaterial);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, redMaterial);

  glPushMatrix();
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  //drawBox(-.5,-.5,-.5,.5,.5,.5);
  draw_box(0.5);
  glPopMatrix();

  glPushMatrix();
  glTranslated(0.,-1.75,0.);
  glRotated(angle, 0., 1., 0.);
  //drawBox(-.5,-.5,-.5,.5,.5,.5);
  draw_box(0.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 1., 0., 0.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  //drawBox(-.5,-.5,-.5,.5,.5,.5);
  draw_box(0.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., -1., 0., 0.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  //drawBox(-.5,-.5,-.5,.5,.5,.5);
  draw_box(0.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 0., 0., 1.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  //drawBox(-.5,-.5,-.5,.5,.5,.5);
  draw_box(0.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 0., 0., -1.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  //drawBox(-.5,-.5,-.5,.5,.5,.5);
  draw_box(0.5);
  glPopMatrix();

}
  glPopMatrix();

  SDL_GL_SwapWindow(window);

}

static void setup_opengl( int width, int height )
{
    float ratio = (float) width / (float) height;

    /* Our shading model--Gouraud (smooth). */
    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );

    /* Setup our viewport. */
    glViewport( 0, 0, width, height );

    /*
     * Change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    /*
     * EXERCISE:
     * Replace this with a call to glFrustum.
     */
    gluPerspective( 60.0, ratio, 1.0, 1024.0 );
}

int main( int argc, char* argv[] )
{
    /* Information about the current video settings. */
    ///const SDL_VideoInfo* info = NULL; // SDL1
    /* Dimensions of our window. */
    int width = 0;
    int height = 0;
    /* Color depth in bits of our window. */
    ///int bpp = 0; // SDL1
    /* Flags we will pass into SDL_SetVideoMode. */
    ///int flags = 0; // SDL1

    /* First, initialize SDL's video subsystem. */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        quit_tutorial( 1 );
    }

    /* Let's get some video information. */
    //info = SDL_GetVideoInfo( );
    /*
    if( !info ) {
        // This should probably never happen.
        fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
        quit_tutorial( 1 );
    }*/

    /*
     * Set our width/height to 640/480 (you would
     * of course let the user decide this in a normal
     * app). We get the bpp we will request from
     * the display. On X11, VidMode can't change
     * resolution, so this is probably being overly
     * safe. Under Win32, ChangeDisplaySettings
     * can change the bpp.
     */
    width = 640;
    height = 480;
    ///bpp = info->vfmt->BitsPerPixel; // SDL1

    /*
     * Now, we want to setup our requested
     * window attributes for our OpenGL window.
     * We want *at least* 5 bits of red, green
     * and blue. We also want at least a 16-bit
     * depth buffer.
     *
     * The last thing we do is request a double
     * buffered window. '1' turns on double
     * buffering, '0' turns it off.
     *
     * Note that we do not use SDL_DOUBLEBUF in
     * the flags to SDL_SetVideoMode. That does
     * not affect the GL attribute state, only
     * the standard 2D blitting setup.
     */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );


    /*
    ported from SDL1 to SDL2
https://wiki.libsdl.org/MigrationGuide
OpenGL: If you were already using OpenGL directly, your migration is pretty simple. Change your SDL_SetVideoMode() call to SDL_CreateWindow() followed by SDL_GL_CreateContext(), and your SDL_GL_SwapBuffers() call to SDL_GL_SwapWindow(window). All the actual calls into the GL are exactly the same.
    */
window = SDL_CreateWindow("Game Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
SDL_GL_CreateContext(window);

    /*
     * We want to request that SDL provide us
     * with an OpenGL window, in a fullscreen
     * video mode.
     *
     * EXERCISE:
     * Make starting windowed an option, and
     * handle the resize events properly with
     * glViewport.
     */
    //flags = SDL_OPENGL | SDL_FULLSCREEN; // SDL1

    /*
     * Set the video mode
     *
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) {
         *
         * This could happen for a variety of reasons,
         * including DISPLAY not being set, the specified
         * resolution not being available, etc.
         *
        fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
        quit_tutorial( 1 );
    }*/

    /*
     * At this point, we should have a properly setup
     * double-buffered window for use with OpenGL.
     */
    setup_opengl( width, height );

    model model1=load_model_obj("assets/head.obj");
    cube=load_model_obj("assets/cube.obj");

    /*
     * Now we want to begin our normal app process--
     * an event loop with a lot of redrawing.
     */
    while( 1 ) {
        /* Process incoming events. */
        process_events( );
        /* Draw the screen. */
        drawScene( model1 );


        ///////--------------------- MY ADD
        SDL_Delay( 30 );
    }

    /*
     * EXERCISE:
     * Record timings using SDL_GetTicks() and
     * and print out frames per second at program
     * end.
     */

    /* Never reached. */
    return 0;
}
