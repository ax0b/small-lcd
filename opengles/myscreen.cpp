#include "myscreen.h"
#include "qtdirectapplication.h"
#include "textureloader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

MyScreen::MyScreen(QObject *parent) : QtDirectScreen(parent)
{
	m_state = new CUBE_STATE_T;	
	memset( m_state, 0, sizeof( *m_state ) );
}

MyScreen::~MyScreen()
{
	delete m_state;
}

void MyScreen::initOpenGL()
{
   int32_t success = 0;
   EGLBoolean result;
   EGLint num_config;

   static EGL_DISPMANX_WINDOW_T nativewindow;

   DISPMANX_UPDATE_HANDLE_T dispman_update;
   VC_RECT_T dst_rect;
   VC_RECT_T src_rect;

   static const EGLint attribute_list[] =
   {
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_NONE
   };
   
   EGLConfig config;

   // get an EGL display connection
   m_state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
   assert(m_state->display!=EGL_NO_DISPLAY);

   // initialize the EGL display connection
   result = eglInitialize(m_state->display, NULL, NULL);
   assert(EGL_FALSE != result);

   // get an appropriate EGL frame buffer configuration
   result = eglChooseConfig(m_state->display, attribute_list, &config, 1, &num_config);
   assert(EGL_FALSE != result);

   // create an EGL rendering context
   m_state->context = eglCreateContext(m_state->display, config, EGL_NO_CONTEXT, NULL);
   assert(m_state->context!=EGL_NO_CONTEXT);

   // create an EGL window surface
   success = graphics_get_display_size(0 /* LCD */, &m_state->screen_width, &m_state->screen_height);
   assert( success >= 0 );

   dst_rect.x = 0;
   dst_rect.y = 0;
   dst_rect.width = m_state->screen_width;
   dst_rect.height = m_state->screen_height;
      
   src_rect.x = 0;
   src_rect.y = 0;
   src_rect.width = m_state->screen_width << 16;
   src_rect.height = m_state->screen_height << 16;        

   m_state->dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
   dispman_update = vc_dispmanx_update_start( 0 );
         
   m_state->dispman_element = vc_dispmanx_element_add ( dispman_update, m_state->dispman_display,
      0/*layer*/, &dst_rect, 0/*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, 0/*transform*/);
      
   nativewindow.element = m_state->dispman_element;
   nativewindow.width = m_state->screen_width;
   nativewindow.height = m_state->screen_height;
   vc_dispmanx_update_submit_sync( dispman_update );
      
   m_state->surface = eglCreateWindowSurface( m_state->display, config, &nativewindow, NULL );
   assert(m_state->surface != EGL_NO_SURFACE);

   // connect the context to the surface
   result = eglMakeCurrent(m_state->display, m_state->surface, m_state->surface, m_state->context);
   assert(EGL_FALSE != result);

   // Set background color and clear buffers
   glClearColor(0.15f, 0.25f, 0.35f, 1.0f);

   // Enable back face culling.
   glEnable(GL_CULL_FACE);

   glMatrixMode(GL_MODELVIEW);	
}

void MyScreen::initModelProj()
{
   float nearp = 1.0f;
   float farp = 500.0f;
   float hht;
   float hwd;

   glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

   glViewport(0, 0, (GLsizei)m_state->screen_width, (GLsizei)m_state->screen_height);
      
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   hht = nearp * (float)tan(45.0 / 2.0 / 180.0 * M_PI);
   hwd = hht * (float)m_state->screen_width / (float)m_state->screen_height;

   glFrustumf(-hwd, hwd, -hht, hht, nearp, farp);
   
   glEnableClientState( GL_VERTEX_ARRAY );
   glVertexPointer( 3, GL_BYTE, 0, quadx );

   resetModel();
}

void MyScreen::resetModel()
{
   // reset model position
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.f, 0.f, -50.f);

   // reset model rotation
   m_state->rot_angle_x = 45.f; m_state->rot_angle_y = 30.f; m_state->rot_angle_z = 0.f;
   m_state->rot_angle_x_inc = 0.5f; m_state->rot_angle_y_inc = 0.5f; m_state->rot_angle_z_inc = 0.f;
   m_state->distance = 40.f;
}

void MyScreen::updateModel()
{
   // update position
   m_state->rot_angle_x = incAndWrapAngle(m_state->rot_angle_x, m_state->rot_angle_x_inc);
   m_state->rot_angle_y = incAndWrapAngle(m_state->rot_angle_y, m_state->rot_angle_y_inc);
   m_state->rot_angle_z = incAndWrapAngle(m_state->rot_angle_z, m_state->rot_angle_z_inc);
   m_state->distance    = incAndClipDistance(m_state->distance, m_state->distance_inc);

   glLoadIdentity();
   // move camera back to see the cube
   glTranslatef(0.f, 0.f, -m_state->distance);

   // Rotate model to new position
   glRotatef(m_state->rot_angle_x, 1.f, 0.f, 0.f);
   glRotatef(m_state->rot_angle_y, 0.f, 1.f, 0.f);
   glRotatef(m_state->rot_angle_z, 0.f, 0.f, 1.f);
}

GLfloat MyScreen::incAndWrapAngle(GLfloat angle, GLfloat angle_inc)
{
   angle += angle_inc;

   if (angle >= 360.0)
      angle -= 360.f;
   else if (angle <=0)
      angle += 360.f;

   return angle;
}

GLfloat MyScreen::incAndClipDistance(GLfloat distance, GLfloat distance_inc)
{
   distance += distance_inc;

   if (distance >= 120.0f)
      distance = 120.f;
   else if (distance <= 40.0f)
      distance = 40.0f;

   return distance;
}

void MyScreen::redrawScene()
{
   // Start with a clear screen
   glClear( GL_COLOR_BUFFER_BIT );

   // Draw first (front) face:
   // Bind texture surface to current vertices
   glBindTexture(GL_TEXTURE_2D, m_state->tex[0]);

   // Need to rotate textures - do this by rotating each cube face
   glRotatef(270.f, 0.f, 0.f, 1.f ); // front face normal along z axis

   // draw first 4 vertices
   glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);

   // same pattern for other 5 faces - rotation chosen to make image orientation 'nice'
   glBindTexture(GL_TEXTURE_2D, m_state->tex[1]);
   glRotatef(90.f, 0.f, 0.f, 1.f ); // back face normal along z axis
   glDrawArrays( GL_TRIANGLE_STRIP, 4, 4);

   glBindTexture(GL_TEXTURE_2D, m_state->tex[2]);
   glRotatef(90.f, 1.f, 0.f, 0.f ); // left face normal along x axis
   glDrawArrays( GL_TRIANGLE_STRIP, 8, 4);

   glBindTexture(GL_TEXTURE_2D, m_state->tex[3]);
   glRotatef(90.f, 1.f, 0.f, 0.f ); // right face normal along x axis
   glDrawArrays( GL_TRIANGLE_STRIP, 12, 4);

   glBindTexture(GL_TEXTURE_2D, m_state->tex[4]);
   glRotatef(270.f, 0.f, 1.f, 0.f ); // top face normal along y axis
   glDrawArrays( GL_TRIANGLE_STRIP, 16, 4);

   glBindTexture(GL_TEXTURE_2D, m_state->tex[5]);
   glRotatef(90.f, 0.f, 1.f, 0.f ); // bottom face normal along y axis
   glDrawArrays( GL_TRIANGLE_STRIP, 20, 4);

   eglSwapBuffers(m_state->display, m_state->surface);
}

bool MyScreen::loadTexImages()
{
   int bpp;
   m_state->tex_buf1 = TextureLoader::loadTexture("tex1.jpg", bpp, 1);
   if (m_state->tex_buf1 == 0) {
   	qDebug() << "Failed to load texture image";
	QtDirectApplication::exit(-1);
	return false;
   }
   
   m_state->tex_buf2 = TextureLoader::loadTexture("tex2.jpg", bpp, 1);
   if (m_state->tex_buf2 == 0) {
   	qDebug() << "Failed to load texture image";
	QtDirectApplication::exit(-1);
	return false;
   }

   m_state->tex_buf3 = TextureLoader::loadTexture("tex3.jpg", bpp, 1);
   if (m_state->tex_buf3 == 0) {
   	qDebug() << "Failed to load texture image";
	QtDirectApplication::exit(-1);
	return false;
   }
   return true;
}

void MyScreen::initTextures()
{
   loadTexImages();
   glGenTextures(6, &m_state->tex[0]);

   // setup first texture
   glBindTexture(GL_TEXTURE_2D, m_state->tex[0]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_state->tex_buf1);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

   // setup second texture - reuse first image
   glBindTexture(GL_TEXTURE_2D, m_state->tex[1]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_state->tex_buf1);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

   // third texture
   glBindTexture(GL_TEXTURE_2D, m_state->tex[2]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_state->tex_buf2);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

   // fourth texture  - reuse second image
   glBindTexture(GL_TEXTURE_2D, m_state->tex[3]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_state->tex_buf2);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

   //fifth texture
   glBindTexture(GL_TEXTURE_2D, m_state->tex[4]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_state->tex_buf3);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

   // sixth texture  - reuse third image
   glBindTexture(GL_TEXTURE_2D, m_state->tex[5]);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_SIZE, IMAGE_SIZE, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, m_state->tex_buf3);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat)GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat)GL_NEAREST);

   // setup overall texture environment
   glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   
   glEnable(GL_TEXTURE_2D);
}

void MyScreen::onExit()
{
   DISPMANX_UPDATE_HANDLE_T dispman_update;
   int s;
   // clear screen
   glClear( GL_COLOR_BUFFER_BIT );
   eglSwapBuffers(m_state->display, m_state->surface);

   glDeleteTextures(6, m_state->tex);
   eglDestroySurface( m_state->display, m_state->surface );

   dispman_update = vc_dispmanx_update_start( 0 );
   s = vc_dispmanx_element_remove(dispman_update, m_state->dispman_element);
   assert(s == 0);
   vc_dispmanx_update_submit_sync( dispman_update );
   s = vc_dispmanx_display_close(m_state->dispman_display);
   assert (s == 0);

   // Release OpenGL resources
   eglMakeCurrent( m_state->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
   eglDestroyContext( m_state->display, m_state->context );
   eglTerminate( m_state->display );

   // release texture buffers
   free(m_state->tex_buf1);
   free(m_state->tex_buf2);
   free(m_state->tex_buf3);
}

void MyScreen::init()
{
	bcm_host_init();
	memset( m_state, 0, sizeof( *m_state ) );
	initOpenGL();
        initModelProj();
	initTextures();
}

void MyScreen::clear()
{
	onExit();
}

void MyScreen::update()
{
	updateModel();
    	redrawScene();
		
}

