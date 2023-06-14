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

static GLboolean should_rotate = GL_TRUE;

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

typedef struct {
  float array[3];
} vector;

typedef struct {
  vector* array;
  int count;
} vector_array;

//vector_array vertex_positions;
//vector_array vertex_normals;

/*
vector* vertex_position_array;
int vertex_position_count;

vector* vertex_normal_array;
int vertex_normal_count;
*/

typedef int vector_index;

typedef struct {
  vector_index vertex_position;
  vector_index vertex_normal;
} vertex;

typedef struct {
  vertex array[3];
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

model load_model(){
  model model1;
  model1.mesh.count=0; // WIP (currently skips looping)
  // CASE "f" (face) triangle face (when triangulated)
  triangle triangle1;
  
  // indices (integers)
  triangle1.array[0].vertex_position=0;
  triangle1.array[0].vertex_normal=0;
  
  triangle1.array[1].vertex_position=0;
  triangle1.array[1].vertex_normal=0;

  triangle1.array[2].vertex_position=0;
  triangle1.array[2].vertex_normal=0;

  // ... use ... (grow array) model1.mesh.array
  
  // CASE "v" (vertex) position vector

  vector position1;
  position1.array[0]=0; // 3 floats
  position1.array[1]=0;
  position1.array[2]=0;
  model1.vertex_positions.count=0;
  //model1.vertex_positions.array[]=position1;

  // CASE "vn" (vertex normal) normal vector

  vector normal1;

  return model1; // WIP needs deallocation after non-automated allocation
}

void draw_vertex(vertex v, model m){
  vector vertex_normal=m.vertex_normals.array[v.vertex_normal];
  glNormal3f(vertex_normal.array[0], vertex_normal.array[1], vertex_normal.array[2]);

  vector vertex_position=m.vertex_positions.array[v.vertex_position];
  glVertex3f(vertex_position.array[0],vertex_position.array[1],vertex_position.array[2]);
}

void draw_triangle(triangle t, model m){
  draw_vertex(t.array[0],m);
  draw_vertex(t.array[1],m);
  draw_vertex(t.array[2],m);
}

void draw_model(model m){
  glBegin(GL_TRIANGLES);
    for(int i=0; i<m.mesh.count; i++)
    draw_triangle(m.mesh.array[i],m);
  glEnd();
    
  ///-- glNormal3f, glVertex3f
  //for _,triangle in ipairs(model) do

    ///glNormal3f(triangle[i].normal.x, triangle[i].normal.y, triangle[i].normal.z);
    ///glVertex3f(triangle[i].x, triangle[i].y, triangle[i].z);
    
    /*
    glNormal3f(triangle[1].normal.x, triangle[1].normal.y, triangle[1].normal.z)
    glVertex3f(triangle[1].x, triangle[1].y, triangle[1].z)

    glNormal3f(triangle[2].normal.x, triangle[2].normal.y, triangle[2].normal.z)
    glVertex3f(triangle[2].x, triangle[2].y, triangle[2].z)

    glNormal3f(triangle[3].normal.x, triangle[3].normal.y, triangle[3].normal.z)
    glVertex3f(triangle[3].x, triangle[3].y, triangle[3].z)
    */
  //end

}

// (cleaner) code import from gltest.cpp (part of https://fox-toolkit.org/)

// Draws a simple box using the given corners
void drawBox(GLfloat xmin, GLfloat ymin, GLfloat zmin, GLfloat xmax, GLfloat ymax, GLfloat zmax) {
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,-1.);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(1.,0.,0.);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,1.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(-1.,0.,0.);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,1.,0.);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,-1.,0.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymin, zmin);
  glEnd();
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

  //glDisable(GL_DITHER);

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
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  glMaterialfv(GL_FRONT, GL_AMBIENT, blueMaterial);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blueMaterial);

  glPushMatrix();
  glRotated(angle, 0., 1., 0.);
  
  draw_model(model1);
  drawBox(-1, -1, -1, 1, 1, 1);

if(0){
  glMaterialfv(GL_FRONT, GL_AMBIENT, redMaterial);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, redMaterial);

  glPushMatrix();
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glTranslated(0.,-1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 1., 0., 0.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., -1., 0., 0.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 0., 0., 1.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 0., 0., -1.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
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
    model model1=load_model();

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

//==========================================

/// https://github.com/arkenidar/sdl-cb/blob/master/main-sdl.c

// . sdl-cross-compile.sh
// wine sdl-app.exe

#if defined(_WIN64)

#include <windows.h>
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow){
 return main(__argc, __argv); }

#endif

#if defined(_WIN64)

#include <windows.h>

int WINAPI WinMain_DE_ACTIVATED( HINSTANCE hInstance,    // HANDLE TO AN INSTANCE.  This is the "handle" to YOUR PROGRAM ITSELF.
                    HINSTANCE hPrevInstance,// USELESS on modern windows (totally ignore hPrevInstance)
                    LPSTR szCmdLine,        // Command line arguments.  similar to argv in standard C programs
                    int iCmdShow )          // Start window maximized, minimized, etc.
{
  //char* argv[]={"main-sdl","--window"};
  //return main(sizeof(argv)/sizeof(argv[0]),argv);

  // https://stackoverflow.com/questions/27363851/pass-winmain-or-wwinmain-arguments-to-normal-main

	int returned = 0;

    char* my_argv[]={"main-sdl","--window"}; // example, not strict
    //return main(sizeof(argv)/sizeof(argv[0]),argv);

    int my_argc = sizeof(my_argv)/sizeof(my_argv[0]);
	returned = main(my_argc, my_argv);

	// C++ code follows:
	/*
  	int w_argc = 0;
	LPWSTR* w_argv = CommandLineToArgvW(GetCommandLineW(), &w_argc);
	if (w_argv)
	{
		char** my_argv = new char*[w_argc]; // C++ code
		int my_argc = 0;

		for (int i = 0; i < w_argc; ++i)
		{
			int w_len = lstrlenW(w_argv[i]);
			int len = WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, NULL, 0, NULL, NULL);
			my_argv[my_argc] = new char[len+1];
			WideCharToMultiByte(CP_ACP, 0, w_argv[i], w_len, my_argv[my_argc], len+1, NULL, NULL);
			++my_argc;
		}

		returned = SDL_main(my_argc, my_argv);

		for (int i = 0; i < my_argc; ++i)
			delete[] my_argv[i];
		delete[] my_argv;

		LocalFree(w_argv);
	}
	*/
	return returned;
}

#endif
