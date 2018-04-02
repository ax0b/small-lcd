#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#include "bcm_host.h"

#include "GLES/gl.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#define PATH "./"

#define IMAGE_SIZE 128

#ifndef M_PI
   #define M_PI 3.141592654
#endif
	
static const GLbyte quadx[6*4*3] = {
   /* FRONT */
   -10, -10,  10,
   10, -10,  10,
   -10,  10,  10,
   10,  10,  10,

   /* BACK */
   -10, -10, -10,
   -10,  10, -10,
   10, -10, -10,
   10,  10, -10,

   /* LEFT */
   -10, -10,  10,
   -10,  10,  10,
   -10, -10, -10,
   -10,  10, -10,

   /* RIGHT */
   10, -10, -10,
   10,  10, -10,
   10, -10,  10,
   10,  10,  10,

   /* TOP */
   -10,  10,  10,
   10,  10,  10,
   -10,  10, -10,
   10,  10, -10,

   /* BOTTOM */
   -10, -10,  10,
   -10, -10, -10,
   10, -10,  10,
   10, -10, -10,
};

/** Texture coordinates for the quad. */
static const GLfloat texCoords[6 * 4 * 2] = {
   0.f,  0.f,
   0.f,  1.f,
   1.f,  0.f,
   1.f,  1.f,

   0.f,  0.f,
   0.f,  1.f,
   1.f,  0.f,
   1.f,  1.f,

   0.f,  0.f,
   0.f,  1.f,
   1.f,  0.f,
   1.f,  1.f,

   0.f,  0.f,
   0.f,  1.f,
   1.f,  0.f,
   1.f,  1.f,

   0.f,  0.f,
   0.f,  1.f,
   1.f,  0.f,
   1.f,  1.f,

   0.f,  0.f,
   0.f,  1.f,
   1.f,  0.f,
   1.f,  1.f
};


typedef struct
{
   uint32_t screen_width;
   uint32_t screen_height;
// OpenGL|ES objects
   DISPMANX_DISPLAY_HANDLE_T dispman_display;
   DISPMANX_ELEMENT_HANDLE_T dispman_element;
   EGLDisplay display;
   EGLSurface surface;
   EGLContext context;
   GLuint tex[6];
// model rotation vector and direction
   GLfloat rot_angle_x_inc;
   GLfloat rot_angle_y_inc;
   GLfloat rot_angle_z_inc;
// current model rotation angles
   GLfloat rot_angle_x;
   GLfloat rot_angle_y;
   GLfloat rot_angle_z;
// current distance from camera
   GLfloat distance;
   GLfloat distance_inc;
// pointers to texture buffers
   char *tex_buf1;
   char *tex_buf2;
   char *tex_buf3;
} CUBE_STATE_T;

#endif // TRIANGLE_H

