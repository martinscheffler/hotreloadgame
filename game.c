#ifdef ___BUILD_LINUX_GNU_X86_64___
#define ___OS_UNIX___ 1
#define ___CC_GNU___ 1
#define ___OS_WINDOWS___ 0
#define ___HOT___ "game.so"
#include <math.h>
#include <GL/gl.h>
#include <GL/glx.h>

#endif

#include <stdio.h>

// copied from glext.h:
GLAPI void APIENTRY glAttachShader (GLuint program, GLuint shader);
GLAPI void APIENTRY glBindBuffer (GLenum target, GLuint buffer);
GLAPI void APIENTRY glBindFragDataLocation (GLuint program, GLuint color, const GLchar *name);
GLAPI void APIENTRY glBindVertexArray (GLuint array);
GLAPI void APIENTRY glBufferData (GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
GLAPI void APIENTRY glCompileShader (GLuint shader);
GLAPI GLuint APIENTRY glCreateProgram (void);
GLAPI GLuint APIENTRY glCreateShader (GLenum type);
GLAPI void APIENTRY glDeleteShader (GLuint shader);
GLAPI void APIENTRY glDeleteProgram (GLuint program);
GLAPI void APIENTRY glDeleteVertexArrays (GLsizei n, const GLuint *arrays);
GLAPI void APIENTRY glEnableVertexAttribArray (GLuint index);
GLAPI void APIENTRY glGenBuffers (GLsizei n, GLuint *buffers);
GLAPI void APIENTRY glGenVertexArrays (GLsizei n, GLuint *arrays);
GLAPI GLint APIENTRY glGetAttribLocation (GLuint program, const GLchar *name);
GLAPI GLint APIENTRY glGetUniformLocation (GLuint program, const GLchar *name);
GLAPI void APIENTRY glLinkProgram (GLuint program);
GLAPI void APIENTRY glShaderSource (GLuint shader, GLsizei count, const GLchar* *string, const GLint *length);
GLAPI void APIENTRY glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
GLAPI void APIENTRY glUseProgram (GLuint program);
GLAPI void APIENTRY glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
GLAPI const GLubyte * GLAPIENTRY gluErrorString (GLenum error);
#include <stdio.h>
#include <assert.h>

// other externals to keep the number of included headers down:
extern void exit (int __status) __THROW __attribute__ ((__noreturn__));
struct FILE;
extern int fflush (FILE *__stream);
extern void *memcpy (void *__restrict __dest, __const void *__restrict __src, size_t __n) __THROW __nonnull ((1, 2));
extern int printf (__const char *__restrict __format, ...);
extern void *malloc(size_t size);
extern void free(void *ptr);
extern char *strtok(char *str, const char *delim);


/*============================================================*/
#if (___CC_GNU___)
typedef unsigned long U8;
typedef signed long S8;
#if (___CPU_BITS___==32)
typedef unsigned int UP;
typedef signed int SP;
#else
typedef unsigned long UP;
typedef signed long SP;
#endif

#endif

/*==============================================================

                     [ALL] EVERYTHING

==============================================================*/
/*============================================================*/

typedef unsigned int (*LibCloseF)(UP);
typedef UP (*LibOpenF)(const unsigned char*, unsigned int);
typedef UP (*LibSymbolF)(UP, const unsigned char*);
typedef UP (*FileTimeF)(const unsigned char*);

typedef void (*HotClean)(UP);
typedef struct {
    LibSymbolF Symbol;
    void* data;
    UP bytes;
    HotClean Clean;
    UP lib;
    UP oldLib;
    UP ver;
    FileTimeF filetime;
} HotArgT;

typedef struct {
    LibSymbolF Symbol;
    UP time;
} HotT;

typedef struct {
    HotT hot;
    /*------------------------------------------------------------*/
    // ... all game global data is in here ...
    /*------------------------------------------------------------*/
    Window window;
    unsigned int fnr; // frame number
    UP pad[8];
} All;
/*============================================================*/
static All __attribute__((aligned(4096))) all= {{(LibSymbolF)(~0)}};


/*============================================================*/

static inline __attribute__((always_inline)) UP HotOpen(HotArgT* arg) {
   void* oldData=arg->data;
   if(oldData) memcpy((UP*)(&all), oldData, sizeof(all));
   arg->Clean((UP)(arg));
   arg->data=&all;
   arg->bytes=sizeof(all);
   all.hot.Symbol=arg->Symbol;

   return (UP)(oldData);
}

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);


//////////////////////////////////// GL 3 Hello World Code ////////////////////////////////////


const char* vertexshader = ""
"#version 150\n"
"uniform mat4 viewMatrix, projMatrix;\n"
"in vec4 position;in vec2 texcoord; out vec2 Texcoord;\n"
"void main() {\n"
"  Texcoord = texcoord; gl_Position = projMatrix * viewMatrix * position;\n"
"}\n";


const char* fragmentshader = ""
"#version 150\n"
"uniform sampler2D diffuseTex; in vec2 Texcoord;out vec4 outputF;\n"
"void main() {\n"
"  vec4 diffuseColor = texture(diffuseTex, Texcoord.st);\n"
"  outputF = diffuseColor;\n"
"}\n";

// Data for drawing Axis
float verticesAxis[] = {-20.0f, 0.0f, 0.0f, 1.0f,
            20.0f, 0.0f, 0.0f, 1.0f,

            0.0f, -20.0f, 0.0f, 1.0f,
            0.0f,  20.0f, 0.0f, 1.0f,

            0.0f, 0.0f, -20.0f, 1.0f,
            0.0f, 0.0f,  20.0f, 1.0f};

// Data for triangle 1
float vertices1[] = {   -3.0f, 0.0f, -5.0f, 1.0f,
            -1.0f, 0.0f, -5.0f, 1.0f,
            -2.0f, 2.0f, -5.0f, 1.0f};


// Data for triangle 2
float vertices2[] = {   1.0f, 0.0f, -5.0f, 1.0f,
            3.0f, 0.0f, -5.0f, 1.0f,
            2.0f, 2.0f, -5.0f, 1.0f};

#define M_PI       3.14159265358979323846

// Program and Shader Identifiers
GLuint p,v,f;

// Vertex Attribute Locations
GLuint vertexLoc, texcoordLoc, diffuseTexLoc;
// Uniform variable Locations
GLuint projMatrixLoc, viewMatrixLoc;

// Vertex Array Objects Identifiers
GLuint vao[3];

// storage for Matrices
float projMatrix[16];
float viewMatrix[16];

// ----------------------------------------------------
// VECTOR STUFF
//

// res = a cross b;
void crossProduct( float *a, float *b, float *res) {
    res[0] = a[1] * b[2]  -  b[1] * a[2]; res[1] = a[2] * b[0]  -  b[2] * a[0]; res[2] = a[0] * b[1]  -  b[0] * a[1];
}

// Normalize a vec3
void normalize(float *a) {
    float mag = sqrt(a[0] * a[0]  +  a[1] * a[1]  +  a[2] * a[2]);
    a[0] /= mag; a[1] /= mag; a[2] /= mag;
}

// ----------------------------------------------------
// MATRIX STUFF
//

// sets the square matrix mat to the identity matrix,
// size refers to the number of rows (or columns)
void setIdentityMatrix( float *mat, int size) {

    // fill matrix with 0s
    int i;
    for (i = 0; i < size * size; ++i) mat[i] = 0.0f;

    // fill diagonal with 1s
    for (i = 0; i < size; ++i) mat[i + i * size] = 1.0f;
}

//
// a = a * b;
//
void multMatrix(float *a, float *b) {

    float res[16];
    int i, j, k;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < 4; ++j) {
            res[j*4 + i] = 0.0f;
            for (k = 0; k < 4; ++k) { res[j*4 + i] += a[k*4 + i] * b[j*4 + k]; }
        }
    }
    memcpy(a, res, 16 * sizeof(float));
}

// Defines a transformation matrix mat with a translation
void setTranslationMatrix(float *mat, float x, float y, float z) {
    setIdentityMatrix(mat,4);
    mat[12] = x; mat[13] = y; mat[14] = z;
}

// ----------------------------------------------------
// Projection Matrix
//

void buildProjectionMatrix(float fov, float ratio, float nearP, float farP) {
    float f = 1.0f / tan (fov * (M_PI / 360.0));
    setIdentityMatrix(projMatrix,4);

    projMatrix[0] = f / ratio; projMatrix[1 * 4 + 1] = f;
    projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    projMatrix[2 * 4 + 3] = -1.0f; projMatrix[3 * 4 + 3] = 0.0f;
}

// ----------------------------------------------------
// View Matrix
//
// note: it assumes the camera is not tilted,
// i.e. a vertical up vector (remmeber gluLookAt?)
//

void setCamera(float posX, float posY, float posZ,
               float lookAtX, float lookAtY, float lookAtZ) {
    float dir[3], right[3], up[3];
    up[0] = 0.0f;   up[1] = 1.0f;   up[2] = 0.0f;

    dir[0] =  (lookAtX - posX); dir[1] =  (lookAtY - posY); dir[2] =  (lookAtZ - posZ);
    normalize(dir);
    crossProduct(dir,up,right); normalize(right);
    crossProduct(right,dir,up); normalize(up);
    float aux[16];
    viewMatrix[0]  = right[0]; viewMatrix[4]  = right[1]; viewMatrix[8]  = right[2]; viewMatrix[12] = 0.0f;
    viewMatrix[1]  = up[0]; viewMatrix[5]  = up[1]; viewMatrix[9]  = up[2]; viewMatrix[13] = 0.0f;
    viewMatrix[2]  = -dir[0]; viewMatrix[6]  = -dir[1]; viewMatrix[10] = -dir[2]; viewMatrix[14] =  0.0f;
    viewMatrix[3]  = 0.0f; viewMatrix[7]  = 0.0f; viewMatrix[11] = 0.0f; viewMatrix[15] = 1.0f;
    setTranslationMatrix(aux, -posX, -posY, -posZ);
    multMatrix(viewMatrix, aux);
}

// ----------------------------------------------------

void changeSize(int w, int h) {

    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0) h = 1;

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    ratio = (1.0f * w) / h;
    buildProjectionMatrix(53.13f, ratio, 1.0f, 30.0f);
}

void setupBuffers() {

    GLuint buffers[1];
    glGenVertexArrays(3, vao);
    // VAO for first triangle
    glBindVertexArray(vao[0]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(2, buffers);

    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);
    // VAO for second triangle
    glBindVertexArray(vao[1]);
    glGenBuffers(1, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);

    // This VAO is for the Axis
    glBindVertexArray(vao[2]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(1, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesAxis), verticesAxis, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 4, GL_FLOAT, 0, 0, 0);

}

void setUniforms() {
    // must be called after glUseProgram
    glUniformMatrix4fv(projMatrixLoc,  1, 0, projMatrix);
    glUniformMatrix4fv(viewMatrixLoc,  1, 0, viewMatrix);
    glUniform1i(diffuseTexLoc, 0);
}

void processNormalKeys(unsigned char key, int x, int y) {

    if (key == 27) {
        glDeleteVertexArrays(3,vao);
        glDeleteProgram(p);
        glDeleteShader(v);
        glDeleteShader(f);
        exit(0);
    }
}

void printGLErrors()
{
    GLenum errCode;
    const GLubyte *errString;
    if ((errCode = glGetError()) != GL_NO_ERROR) {
       errString = gluErrorString(errCode);
       printf("OpenGL Error: %s\n", errString); fflush(0);
    }
}

GLuint setupShaders() {
    GLuint p,v,f;
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(v, 1, &vertexshader,NULL);
    glShaderSource(f, 1, &fragmentshader,NULL);
    glCompileShader(v);
    glCompileShader(f);
    p = glCreateProgram();
    glAttachShader(p,v);
    glAttachShader(p,f);
    glBindFragDataLocation(p, 0, "outputF");
    glLinkProgram(p);
    vertexLoc     = glGetAttribLocation(p,"position");
    texcoordLoc   = glGetAttribLocation(p, "texcoord");
    diffuseTexLoc = glGetUniformLocation(p, "diffuseTex");
    projMatrixLoc = glGetUniformLocation(p, "projMatrix");
    viewMatrixLoc = glGetUniformLocation(p, "viewMatrix");

    printf("Dif: %u Texcoord: %u", diffuseTexLoc, texcoordLoc);fflush(0);
    return p;
}


void handle_events(Display *dpy, Window win)
{
   while (XPending(dpy) > 0) {
      XEvent e;
      XNextEvent(dpy, &e);
      switch (e.type) {
         case ConfigureNotify:  changeSize(e.xconfigure.width, e.xconfigure.height); break;
         case KeyPress:
         {
            char buffer[10]; int r, code;
            code = XLookupKeysym(&e.xkey, 0);
            if (code == XK_Left) {
               //view_roty += 5.0;
            }
            else if (code == XK_Right) {
              // view_roty -= 5.0;
            }
            else if (code == XK_Up) {
              // view_rotx += 5.0;
            }
            else if (code == XK_Down) {
              // view_rotx -= 5.0;
            }
            else {
               r = XLookupString(&e.xkey, buffer, sizeof(buffer), NULL, NULL);
               if (buffer[0] == 27) { exit(0); } // esc pressed
            }
         }
      }
   }
}


GLXFBConfig* fbConf(Display* display) {

    /*const char *extensions = glXQueryExtensionsString(display, DefaultScreen(display));
    printf("Extensions: %s\n", extensions); fflush(0);*/
    static int visual_attribs[] =
    {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, 1,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        None
     };

    int fbcount;
    GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
    if (!fbc) printf("Failed to retrieve a framebuffer config\n");
    return fbc;
}

Window openWindow(Display* display, GLXFBConfig *fbc) {

    XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[0]);
    XSetWindowAttributes swa;
    swa.colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask | KeyPressMask;
    Window win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 800, 600,
                               0, vi->depth, InputOutput, vi->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);
    if (!win) printf("Failed to create window.\n");
    return win;
}


static glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;

GLXContext createContext(Display* display, GLXFBConfig *fbc) {
    if(glXCreateContextAttribsARB == NULL) {
        // Create an oldstyle context first, to get the correct function pointer for glXCreateContextAttribsARB
        XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[0]);
        GLXContext ctx_old = glXCreateContext(display, vi, 0, GL_TRUE);
        glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
        glXMakeCurrent(display, 0, 0);
        glXDestroyContext(display, ctx_old);

        if (glXCreateContextAttribsARB == NULL) {
            printf("glXCreateContextAttribsARB entry point not found. Aborting.\n");
            return 0;
        }
    }

    static int context_attribs[] =
    {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        None
    };

    GLXContext ctx = glXCreateContextAttribsARB(display, fbc[0], NULL, 1, context_attribs);
    if (!ctx)
    {
        printf("Failed to create GL3 context.\n");
    }
    return ctx;
}

//////////////////////////////////// Functions for loading .obj files ////////////////////////////////////

struct Mesh
{
    GLuint _vao;
    GLuint _diffuseTex;
    unsigned int _numFaces;
    unsigned int _numVerts;
};


static void readBMP32bits(const char* path, unsigned int w, unsigned int h)
{
  FILE *fp = fopen(path, "r"); assert(fp != NULL);
  fseek(fp, 54, SEEK_SET); // ignore header
  GLbyte* pixels = malloc(w * h * 3);
  fread(pixels,w * h * 3,1,fp);
  fclose(fp);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 512, 512, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
  free(pixels);
}

void objLoaderLoadFile(struct Mesh* m, const char* path) {
    unsigned int numnormals = 0, numtexcoords = 0;
    char buf[256];
    FILE *fp;
    m->_numFaces = m->_numVerts = 0;
    fp = fopen(path, "r"); assert(fp != NULL);

    // get number of verts, faces etc
    while(fgets(buf , 256 , fp) != NULL) {
        if(buf[0] == 'v' && buf[1] == ' ')      m->_numVerts++;
        else if(buf[0] == 'f' && buf[1] == ' ') m->_numFaces++;
        else if(buf[0] == 'v' && buf[1] == 't') numtexcoords++;
        else if(buf[0] == 'v' && buf[1] == 'n') numnormals++;
    }

    assert(numtexcoords == m->_numVerts);
    assert(numnormals == m->_numVerts);

    // allocate memory for storing verts etc
    float *vb, *vertbuf, *normbuf, *nb, *tcb, *tcbuf;
    unsigned int *facebuf, *fb;
    vertbuf = vb  = malloc(sizeof(float) * 3 * m->_numVerts);
    normbuf = nb  = malloc(sizeof(float) * 3 * m->_numVerts);
    tcbuf   = tcb = malloc(sizeof(float) * 2 * m->_numVerts);
    facebuf = fb  = malloc(sizeof(unsigned int) * 9 * m->_numFaces);

    rewind(fp);
    while(fgets (buf, 256, fp) != NULL) {
        if(buf[0] == 'v' && buf[1] == ' ') {
            sscanf(buf + 2, "%f %f %f", vb, vb + 1, vb + 2);
            vb += 3;
        }
        else if(buf[0] == 'f' && buf[1] == ' ') {
            char* f = strtok(buf + 2, " "); sscanf(f, "%u/%u/%u", fb + 0,fb + 1,fb + 2);
            f = strtok(NULL, " ");          sscanf(f, "%u/%u/%u", fb + 3,fb + 4,fb + 5);
            f = strtok(NULL, " ");          sscanf(f, "%u/%u/%u", fb + 6,fb + 7,fb + 8);
            fb += 9;
        }
        else if(buf[0] == 'v' && buf[1] == 'n') {
            sscanf(buf + 3, "%f %f %f", nb, nb + 1, nb + 2);
            nb += 3;
        }
        else if(buf[0] == 'v' && buf[1] == 't') {
            sscanf(buf + 3, "%f %f", tcb, tcb + 1);
            tcb += 2;
        }
    }

    fclose(fp);

    // Now create VBOs from data
    float* vbo, *vp;
    size_t vbufsize = sizeof(float) * 9 * m->_numFaces;
    vbo = vp = (float*)malloc(vbufsize);

    float *tcbo, *tcp;
    size_t tcbufsize = sizeof(float) * 6 * m->_numFaces;
    tcbo = tcp = (float*)malloc(tcbufsize);

    fb = facebuf;
    unsigned int i,j,k;
    for(i = 0; i < m->_numFaces * 9; ++i) {
        if(i % 3 == 0) { // read vertex data
            k = ((*fb) - 1) * 3;
            for(j = 0; j < 3; ++j) *(vp++) = vertbuf[k  + j];
        }
        else if(i % 3 == 1) { // read tex coord data
            k = ((*fb) - 1 ) * 2;
            for(j = 0; j < 2; ++j) *(tcp++) = tcbuf[k + j];
        }
        ++fb;
    }
    for(i = 0; i < numtexcoords; ++i) {
        printf("Vrt %f %f %f tc %f %f\n", vbo[i * 3], vbo[i * 3 + 1], vbo[i * 3 + 2], tcbo[i * 2], tcbo[i * 2 + 1]);
    }
    fflush(0);

    // bind vertex array object
    glGenVertexArrays(1, &m->_vao);
    glBindVertexArray(m->_vao);

    GLuint buffers[2];
    glGenBuffers(2, buffers);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, vbufsize, vbo, GL_STATIC_DRAW);
    glEnableVertexAttribArray(vertexLoc);
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, 0, 0, 0);

    // tex coord array
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, tcbufsize, tcbo, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texcoordLoc);
    glVertexAttribPointer(texcoordLoc, 2, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(texcoordLoc);

    free(tcbo);
    free(vertbuf); free(normbuf); free(tcbuf); free(facebuf); free(vbo);

    glGenTextures(1, &m->_diffuseTex);

    glBindTexture(GL_TEXTURE_2D, m->_diffuseTex);
    glActiveTexture(GL_TEXTURE0 + 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    printf("Dif: %u", diffuseTexLoc);fflush(0);

    readBMP32bits("kitten.bmp", 512, 512);
}


//////////////////////////////////// Hot Loading main function ////////////////////////////////////
void Hot(HotArgT* arg) {

    Display *display = XOpenDisplay(0);
    GLXFBConfig *fbc = fbConf(display);
    if (HotOpen(arg) == 0) {
      // First entry.
      all.fnr = 0;
      all.window = openWindow(display, fbc);
    }

    XMapWindow(display, all.window);
    GLXContext ctx = createContext(display, fbc);
    glXMakeCurrent(display, all.window, ctx);
    p = setupShaders();
    setupBuffers();
    changeSize(800, 600);
    printGLErrors();

    glEnable(GL_CULL_FACE);
    struct Mesh m;
    objLoaderLoadFile(&m, "physics_crate.obj");

    unsigned int lib_load_time = arg->filetime(___HOT___);
    while(lib_load_time == arg->filetime(___HOT___)) {

       handle_events(display, all.window);

       glClearColor (1, 0.0, 0.50, 1);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       setCamera(sin((float)all.fnr / 100) * 20, 2, cos((float)all.fnr / 100) * 10,0,2,-5);
       glUseProgram(p);
       setUniforms();

       glBindVertexArray(vao[0]); glDrawArrays(GL_TRIANGLES, 0, 3);
       glBindVertexArray(vao[1]); glDrawArrays(GL_TRIANGLES, 0, 3);
       glBindVertexArray(vao[2]); glDrawArrays(GL_LINES, 0, 6);
       glBindVertexArray(m._vao); glDrawArrays(GL_TRIANGLES, 0, m._numFaces * 3);
       glBindVertexArray(0);
       glXSwapBuffers (display, all.window);

       printGLErrors();
       ++all.fnr;
    }
    glXMakeCurrent(display, 0, 0);
    glXDestroyContext(display, ctx);
    printf("----------------- Reloading! -----------------\n\n");
}
