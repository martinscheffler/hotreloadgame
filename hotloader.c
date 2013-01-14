
//http://timothylottes.blogspot.de/2013/01/run-time-binary-reload-detailed.html

/*==============================================================

                        ATOM HOT LOADER

==============================================================*/
#ifdef ___BUILD_LINUX_GNU_X86_64___
#define ___OS_UNIX___ 1
#define ___OS_WINDOWS___ 0
#define ___HOT___ "game.so"
#endif
/*============================================================*/
#if ___OS_UNIX___
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
/*------------------------------------------------------------*/
static void FileUnlink(const char* path)
{
    unlink(path);
}
static char __attribute__((aligned(4096))) fileBuf[4096];
static void FileCopy(const char* dst, const char* src)
{
    int srcF=open(src,O_RDONLY);
    int dstF=open(dst,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU);
    while(1) {
        ssize_t bytes=read(srcF,fileBuf,4096);
        if(bytes==0) {
            close(srcF);
            close(dstF);
            return;
        }
        if(bytes > 0) {
            char* buf=fileBuf;
            while(1) {
                const ssize_t bytes2=write(dstF,buf,bytes);
                if(bytes2==bytes) break;
                if(bytes2<0) {
                    if(errno!=EINTR) break;
                } else {
                    bytes-=bytes2;
                    buf+=bytes2;
                }
            }
            continue;
        }
        if(errno==EINTR) continue;
        break;
    }
    close(srcF);
    close(dstF);
    unlink(dst);
}
/*------------------------------------------------------------*/
#define LIB_SYM_STRING "dlsym"
static void* LibSym(void* mod, const char* str)
{
    return dlsym(mod,str);
}
static void* LibOpen(const char* str)
{
    return dlopen(str, RTLD_LAZY);
}
static void LibClose(void* mod)
{
    dlclose(mod);
}

static unsigned int FileTime(const char* path)
{
   struct stat statbuf;
    if (stat(path, &statbuf) == -1) {
        perror(path);
        exit(1);
    }
    return statbuf.st_mtim.tv_sec;
}

static void Sleeper(void)
{
    struct timespec t;
    t.tv_sec=0;
    t.tv_nsec=1000000000/60;
    nanosleep(&t,0);
}
#endif
/*============================================================*/
#if ___OS_WINDOWS___
#include <Windows.h>
#pragma comment(lib, "Kernel32")
static void FileCopy(const char* dst, const char* src)
{
    CopyFile(src, dst, 0);
}
static void FileUnlink(const char* path)
{
    DeleteFile(path);
}
#define LIB_SYM_STRING "GetProcAddress"
static void* LibSym(void* mod, const char* str)
{
    return GetProcAddress((HMODULE)mod,str);
}
static void* LibOpen(const char* str)
{
    return (void*) LoadLibrary(str);
}
static void LibClose(void* mod)
{
    FreeLibrary((HMODULE)mod);
}
static void Sleeper(void)
{
    Sleep(1000/60);
}
#endif
/*============================================================*/
typedef void (*HotF)(void**);
/*------------------------------------------------------------*/
enum {
    HOT_LIB_SYM,  // address to dlsym() or GetProcAddress()
    HOT_DATA,     // address to previous data, or 0 on first open
    HOT_BYTES,    // bytes of previous data, set by Hot()
    HOT_CLEAN,    // address to HotClean(), call after data copy
    HOT_LIB,      // handle to loaded lib
    HOT_LIB_OLD,  // handle to old loaded lib
    HOT_VER,      // temp file version
    HOT_FILETIME, // address to FileTime()
    HOT_TOTAL

};
/*------------------------------------------------------------*/
static void* hot[HOT_TOTAL];
/*============================================================*/
static void HotClean(void** h)
{
    char name[4]= {'0','.','t',0};
    {
        void* const old=h[HOT_LIB_OLD];
        if(old) {
            LibClose(old);
            {
                const unsigned int ver=(((unsigned int)(h[HOT_VER]))-1)&0xf;
                name[0]=(char)(ver+(ver<0xa?'0':'A'-0xA));
            }
            FileUnlink(name);
        }
        h[HOT_LIB_OLD]=h[HOT_LIB];
        h[HOT_VER]=h[HOT_VER]+1;
    }
}
/*============================================================*/
#if ___OS_UNIX___
int main(void)
#endif
/*------------------------------------------------------------*/
#if ___OS_WINDOWS___
#ifdef ___DEBUG___
int main(void)
#else
int CALLBACK WinMain(HINSTANCE i, HINSTANCE pi, LPSTR cmd, int show)
#endif
#endif

{
#if ___OS_UNIX___
    char name[8]= {'.','/','0','.','t',0,0,0};
#else
    char name[8]= {'.','\\','0','.','t',0,0,0};
#endif
    hot[HOT_LIB_SYM]=LibSym(0,LIB_SYM_STRING);
    hot[HOT_CLEAN]=HotClean;
    hot[HOT_FILETIME]= FileTime;
    printf("Starting\n"); fflush(0);
    while(1) {

        const unsigned int ver=((unsigned int)(hot[HOT_VER]))&0xf;
        name[2]=(char)(ver+(ver<0xA?'0':'A'-0xA));
        FileCopy(name,___HOT___);
        {
            void* const lib=hot[HOT_LIB]=LibOpen(name);
            if(lib) {
                HotF Hot=LibSym(lib,"Hot");
                if(Hot) {
                    Hot(hot);
                }
            }
        }
        Sleeper();
    }
    return 0;
}
