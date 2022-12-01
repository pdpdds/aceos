# 1 "stdlib/malloc.c"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "stdlib/malloc.c"
# 273 "stdlib/malloc.c"
# 1 "/mobius/include/os/syscall.h" 1
# 10 "/mobius/include/os/syscall.h"
# 1 "/mobius/include/sys/types.h" 1
# 23 "/mobius/include/sys/types.h"
typedef signed char int8_t;
typedef short int16_t;
typedef long int32_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;





typedef long long int64_t;
typedef unsigned long long uint64_t;


typedef uint32_t addr_t;
typedef unsigned int size_t;
typedef int ssize_t;
typedef long ptrdiff_t;

typedef addr_t off_t;
typedef uint64_t time_t;


typedef unsigned short wchar_t;


typedef int wint_t;


typedef void *va_list;




typedef addr_t handle_t;
typedef int32_t status_t;
# 11 "/mobius/include/os/syscall.h" 2



struct fileop_t;
struct process_info_t;
struct sysinfo_t;
struct systimes_t;
struct context_v86_t;
struct dirent_t;

# 1 "/mobius/include/os/sysdef.h" 1
# 88 "/mobius/include/os/sysdef.h"









# 106 "/mobius/include/os/sysdef.h"
int DbgWrite (const wchar_t *text, size_t length);
# 116 "/mobius/include/os/sysdef.h"
int Hello (int a, int b);
# 126 "/mobius/include/os/sysdef.h"
unsigned SysUpTime (void);
# 135 "/mobius/include/os/sysdef.h"
_Bool SysGetInfo (struct sysinfo_t *info);
# 144 "/mobius/include/os/sysdef.h"
_Bool SysGetTimes (struct systimes_t *times);
# 164 "/mobius/include/os/sysdef.h"
_Bool SysShutdown (unsigned action);




void KeLeakBegin (void);




void KeLeakEnd (void);
# 187 "/mobius/include/os/sysdef.h"
void SysYield (void);







void KeSetSingleStep (_Bool enable);

# 206 "/mobius/include/os/sysdef.h"

# 215 "/mobius/include/os/sysdef.h"
void ThrExitThread (int exitcode);
# 226 "/mobius/include/os/sysdef.h"
_Bool ThrWaitHandle (handle_t hnd);
# 236 "/mobius/include/os/sysdef.h"
void ThrSleep (unsigned delay);
# 264 "/mobius/include/os/sysdef.h"
handle_t ThrCreateV86Thread (uint32_t entry, uint32_t stack_top, unsigned priority, void (*handler)(void), const wchar_t *name);
# 276 "/mobius/include/os/sysdef.h"
_Bool ThrGetV86Context (struct context_v86_t* context);
# 288 "/mobius/include/os/sysdef.h"
_Bool ThrSetV86Context (const struct context_v86_t *context);
# 300 "/mobius/include/os/sysdef.h"
_Bool ThrContinueV86 (void);
# 316 "/mobius/include/os/sysdef.h"
handle_t ThrCreateThread (void (*entry)(void), void *param, unsigned priority, const wchar_t *name);

int ThrWaitMultiple (const handle_t *handles, unsigned count, _Bool wait_all);

# 329 "/mobius/include/os/sysdef.h"

# 346 "/mobius/include/os/sysdef.h"
void ProcExitProcess (int exitcode);
# 365 "/mobius/include/os/sysdef.h"
handle_t ProcSpawnProcess (const wchar_t *exe, const struct process_info_t *info);
# 374 "/mobius/include/os/sysdef.h"
int ProcGetExitCode (handle_t proc);

# 426 "/mobius/include/os/sysdef.h"

# 440 "/mobius/include/os/sysdef.h"
handle_t FsCreate (const wchar_t *name, uint32_t flags);
# 453 "/mobius/include/os/sysdef.h"
handle_t FsOpen (const wchar_t *name, uint32_t flags);
# 466 "/mobius/include/os/sysdef.h"
_Bool FsRead (handle_t file, void *buffer, size_t bytes, struct fileop_t *op);
# 479 "/mobius/include/os/sysdef.h"
_Bool FsWrite (handle_t file, const void *buffer, size_t bytes, struct fileop_t *op);
# 503 "/mobius/include/os/sysdef.h"
off_t FsSeek (handle_t file, off_t offset, unsigned origin);
# 512 "/mobius/include/os/sysdef.h"
handle_t FsOpenDir (const wchar_t *name);
# 532 "/mobius/include/os/sysdef.h"
_Bool FsQueryFile (const wchar_t *name, uint32_t code, void *buffer, size_t size);
# 550 "/mobius/include/os/sysdef.h"
_Bool FsRequest (handle_t file, uint32_t code, void *buffer, size_t bytes, struct fileop_t* op);






_Bool FsIoCtl (handle_t file, uint32_t code, void *buffer, size_t bytes, struct fileop_t* op);

_Bool FsReadDir (handle_t dir, struct dirent_t *buffer, size_t bytes);
_Bool FsQueryHandle (handle_t file, uint32_t code, void *buffer, size_t bytes);
_Bool FsCreateDir (const wchar_t* dir);
_Bool FsChangeDir (const wchar_t* dir);
_Bool FsMount (const wchar_t *path, const wchar_t *filesys, const wchar_t *dest);
_Bool FsDismount (const wchar_t *path);
_Bool FsCreatePipe (handle_t *ends);

# 576 "/mobius/include/os/sysdef.h"

void * VmmAlloc (size_t, addr_t, uint32_t);
_Bool VmmFree (void*);
void * VmmMapSharedArea (handle_t, addr_t, uint32_t);
void * VmmMapFile (handle_t, addr_t, size_t, uint32_t);
void * VmmReserveArea (size_t, addr_t);
handle_t VmmShare (void *base, const wchar_t *name);

# 593 "/mobius/include/os/sysdef.h"

handle_t EvtCreate (void);
_Bool HndClose (handle_t);
void EvtSignal (handle_t);
_Bool EvtIsSignalled (handle_t);
_Bool HndSetInheritable (handle_t, _Bool);
handle_t MuxCreate (void);
_Bool MuxAcquire (handle_t);
_Bool MuxRelease (handle_t);
_Bool HndExport (handle_t, const wchar_t*);
handle_t HndOpen (const wchar_t*);

# 22 "/mobius/include/os/syscall.h" 2
# 274 "stdlib/malloc.c" 2
# 1 "/mobius/include/os/defs.h" 1
# 53 "/mobius/include/os/defs.h"
typedef struct module_info_t module_info_t;
struct module_info_t
{
    module_info_t *prev, *next;
    wchar_t name[256];
    addr_t base;
    size_t length;
};

typedef struct process_info_t process_info_t;

struct process_info_t
{
    unsigned id;
    addr_t base;
    handle_t std_in, std_out;
    wchar_t cwd[256];
    wchar_t cmdline[256];
    module_info_t *module_first, *module_last;
};
# 83 "/mobius/include/os/defs.h"
typedef uint32_t FARPTR;
# 92 "/mobius/include/os/defs.h"
typedef struct pusha_t pusha_t;
struct pusha_t
{
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
};
# 105 "/mobius/include/os/defs.h"
typedef struct context_t context_t;
struct context_t
{
    uint32_t kernel_esp;
    context_t *ctx_prev;
    pusha_t regs;
    uint32_t gs, fs, es, ds;
    uint32_t intr, error;
    uint32_t eip, cs, eflags, esp, ss;
};

typedef struct context_v86_t context_v86_t;
struct context_v86_t
{
    uint32_t kernel_esp;
    context_t *ctx_prev;
    pusha_t regs;
    uint32_t gs, fs, es, ds;
    uint32_t intr, error;
    uint32_t eip, cs, eflags, esp, ss;


    uint32_t v86_es, v86_ds, v86_fs, v86_gs;
};
# 139 "/mobius/include/os/defs.h"
typedef struct thread_info_t thread_info_t;





struct thread_info_t
{





    thread_info_t *info;

    uint32_t id;

    process_info_t *process;

    int status;

    void *param;
    void (*exception_handler)(void);
    context_t exception_info;
    void *wmgr_info;
};
# 201 "/mobius/include/os/defs.h"
typedef struct fileop_t fileop_t;




struct fileop_t
{







    status_t result;



    handle_t event;



    size_t bytes;
};
# 244 "/mobius/include/os/defs.h"
typedef struct dirent_t dirent_t;

struct dirent_t
{
    unsigned vnode;
    wchar_t name[256];
};

typedef struct dirent_standard_t dirent_standard_t;
struct dirent_standard_t
{
    uint64_t length;
    uint64_t attributes;
    wchar_t mimetype[64];
};

typedef struct dirent_device_t dirent_device_t;
struct dirent_device_t
{
    wchar_t description[256];
    uint32_t device_class;
};
# 296 "/mobius/include/os/defs.h"
typedef struct sysinfo_t sysinfo_t;

struct sysinfo_t
{
    size_t page_size;
    size_t pages_total;
    size_t pages_free;
    size_t pages_physical;
    size_t pages_kernel;
};

typedef struct systimes_t systimes_t;

struct systimes_t
{
    unsigned quantum;
    unsigned uptime;
    unsigned current_cputime;
};
# 336 "/mobius/include/os/defs.h"
typedef struct lmutex_t lmutex_t;
struct lmutex_t
{
    unsigned locks;
    void *eip;
    unsigned owner;
};
# 275 "stdlib/malloc.c" 2
# 1 "/mobius/include/malloc.h" 1
# 13 "/mobius/include/malloc.h"
typedef struct malloc_state *mstate;
extern struct malloc_state __av_default;

void * calloc(size_t _nelem, size_t _size);
void * malloc(size_t _size);
void free(void *_ptr);
void * realloc(void *_ptr, size_t _size);
void * memalign(size_t alignment, size_t n);
void * valloc(size_t n);

void ** independent_calloc(size_t n_elements, size_t size, void* chunks[]);
void ** independent_comalloc(size_t n_elements, size_t sizes[], void* chunks[]);
void * pvalloc(size_t n);
void cfree(void* p);

mstate malloc_create_heap(mstate av, void *(*allocator)(size_t));
void malloc_delete_heap(mstate av);

void * acalloc(mstate av, size_t _nelem, size_t _size);
void * amalloc(mstate av, size_t _size);
void afree(mstate av, void *_ptr);
void * arealloc(mstate av, void *_ptr, size_t _size);
void * amemalign(mstate av, size_t alignment, size_t n);
void * avalloc(mstate av, size_t n);

void ** independent_acalloc(mstate av, size_t n_elements, size_t size, void* chunks[]);
void ** independent_acomalloc(mstate av, size_t n_elements, size_t sizes[], void* chunks[]);
void * apvalloc(mstate av, size_t n);
void acfree(mstate av, void* p);

typedef struct __maldbg_header_t __maldbg_header_t;
struct __maldbg_header_t
{
    int line;
    const char *file;
    __maldbg_header_t *prev, *next;
    size_t size;
    int tag;
    unsigned int magic[2];
};

void * __malloc(size_t _size, const char *, int);
void __free(void *_ptr, const char *, int);
void * __realloc(void *_ptr, size_t _size, const char *, int);
wchar_t *__wcsdup(const wchar_t *_str, const char *, int);
char * __strdup(const char *_str, const char *, int);
__maldbg_header_t *__malloc_find_block(void *addr);
void __malloc_leak(int tag);
void __malloc_leak_dump(void);
int __malloc_lock(int lock_unlock, unsigned long *lock);
# 276 "stdlib/malloc.c" 2
# 307 "stdlib/malloc.c"
void *__morecore(size_t nbytes);
# 348 "stdlib/malloc.c"
# 1 "/mobius/include/stddef.h" 1
# 349 "stdlib/malloc.c" 2
# 370 "stdlib/malloc.c"
# 1 "/mobius/include/stdio.h" 1
# 12 "/mobius/include/stdio.h"
# 1 "/mobius/include/sys/version.h" 1
# 13 "/mobius/include/stdio.h" 2
# 39 "/mobius/include/stdio.h"
typedef struct FILE {
  int _cnt;
  char *_ptr;
  char *_base;
  int _bufsiz;
  int _flag;
  unsigned _file;
  char *_name_to_remove;
  int _fillsize;
} FILE;



typedef unsigned long fpos_t;

FILE *__get_stdin(void), *__get_stdout(void), *__get_stderr(void);


extern FILE __dj_stdin, __dj_stdout, __dj_stderr;
# 67 "/mobius/include/stdio.h"
void clearerr(FILE *_stream);
int fclose(FILE *_stream);
int feof(FILE *_stream);
int ferror(FILE *_stream);
int fflush(FILE *_stream);
int fgetc(FILE *_stream);
int fgetpos(FILE *_stream, fpos_t *_pos);
char * fgets(char *_s, int _n, FILE *_stream);
FILE * fopen(const char *_filename, const char *_mode);
int fprintf(FILE *_stream, const char *_format, ...);
int fputc(int _c, FILE *_stream);
int fputs(const char *_s, FILE *_stream);
size_t fread(void *_ptr, size_t _size, size_t _nelem, FILE *_stream);
FILE * freopen(const char *_filename, const char *_mode, FILE *_stream);
int fscanf(FILE *_stream, const char *_format, ...);
int fseek(FILE *_stream, long _offset, int _mode);
int fsetpos(FILE *_stream, const fpos_t *_pos);
long ftell(FILE *_stream);
size_t fwrite(const void *_ptr, size_t _size, size_t _nelem, FILE *_stream);
int getc(FILE *_stream);
int getchar(void);
char * gets(char *_s);
void perror(const char *_s);
int printf(const char *_format, ...);
int putc(int _c, FILE *_stream);
int putchar(int _c);
int puts(const char *_s);
int remove(const char *_filename);
int rename(const char *_old, const char *_new);
void rewind(FILE *_stream);
int scanf(const char *_format, ...);
void setbuf(FILE *_stream, char *_buf);
int setvbuf(FILE *_stream, char *_buf, int _mode, size_t _size);
int sprintf(char *_s, const char *_format, ...);
int sscanf(const char *_s, const char *_format, ...);
FILE * tmpfile(void);
char * tmpnam(char *_s);
int ungetc(int _c, FILE *_stream);
int vfprintf(FILE *_stream, const char *_format, va_list _ap);
int vprintf(const char *_format, va_list _ap);
int vsprintf(char *_s, const char *_format, va_list _ap);

int wprintf(const wchar_t *_format, ...);
FILE *_wfopen( const wchar_t *filename, const wchar_t *mode );


int _cputws(const wchar_t *str, size_t count);
int _wdprintf(const wchar_t *fmt, ...);
# 371 "stdlib/malloc.c" 2
# 1 "/mobius/include/errno.h" 1
# 13 "/mobius/include/errno.h"
int *_geterrno(void);
# 48 "/mobius/include/errno.h"
extern char * sys_errlist[];
extern wchar_t * sys_werrlist[];
extern int sys_nerr;
extern const char * __sys_errlist[];
extern const wchar_t * __sys_werrlist[];
extern int __sys_nerr;
# 372 "stdlib/malloc.c" 2
# 624 "stdlib/malloc.c"
void* memset(void*, int, size_t);
void* memcpy(void*, const void*, size_t);
# 659 "stdlib/malloc.c"
extern void* sbrk(ptrdiff_t);
# 876 "stdlib/malloc.c"
struct mallinfo {
  int arena;
  int ordblks;
  int smblks;
  int hblks;
  int hblkhd;
  int usmblks;
  int fsmblks;
  int uordblks;
  int fordblks;
  int keepcost;
};
# 916 "stdlib/malloc.c"
void* public_mALLOc(size_t);
# 933 "stdlib/malloc.c"
void public_fREe(void*);
# 944 "stdlib/malloc.c"
void* public_cALLOc(size_t, size_t);
# 977 "stdlib/malloc.c"
void* public_rEALLOc(void*, size_t);
# 995 "stdlib/malloc.c"
void* public_mEMALIGn(size_t, size_t);
# 1006 "stdlib/malloc.c"
void* public_vALLOc(size_t);
# 1033 "stdlib/malloc.c"
int mallopt(mstate, int, int);
# 1063 "stdlib/malloc.c"
struct mallinfo mallinfo(mstate);
# 1121 "stdlib/malloc.c"
void** public_aiCALLOc(mstate, size_t, size_t, void**);
# 1186 "stdlib/malloc.c"
void** independent_acomalloc(mstate av, size_t, size_t*, void**);
# 1198 "stdlib/malloc.c"
void* public_pVALLOc(size_t);
# 1212 "stdlib/malloc.c"
void public_cFREe(void*);
# 1242 "stdlib/malloc.c"
int malloc_atrim(mstate, size_t);
# 1263 "stdlib/malloc.c"
size_t public_mUSABLe(void*);
# 1289 "stdlib/malloc.c"
void public_mSTATs();
# 1519 "stdlib/malloc.c"
static void* amALLOc(mstate, size_t);
static void afREe(mstate, void*);
static void* arEALLOc(mstate, void*, size_t);
static void* amEMALIGn(mstate, size_t, size_t);
static void* avALLOc(mstate, size_t);
static void* apVALLOc(mstate, size_t);
static void* acALLOc(mstate, size_t, size_t);
static void** iaCALLOc(mstate, size_t, size_t, void**);
static void** iaCOMALLOc(mstate, size_t, size_t*, void**);
static void acFREe(mstate, void*);
static int amTRIm(mstate, size_t);
static size_t amUSABLe(mstate, void*);
static void amSTATs(mstate);
static int amALLOPt(mstate, int, int);
static struct mallinfo amALLINFo(mstate);
# 1661 "stdlib/malloc.c"
struct malloc_chunk {

  size_t prev_size;
  size_t size;

  struct malloc_chunk* fd;
  struct malloc_chunk* bk;
};


typedef struct malloc_chunk* mchunkptr;
# 1933 "stdlib/malloc.c"
typedef struct malloc_chunk* mbinptr;
# 2071 "stdlib/malloc.c"
typedef struct malloc_chunk* mfastbinptr;
# 2148 "stdlib/malloc.c"
struct malloc_state {


  size_t max_fast;


  mfastbinptr fastbins[(((((unsigned int)((((80) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((80) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) >> 3) - 2)+1)];


  mchunkptr top;


  mchunkptr last_remainder;


  mchunkptr bins[128 * 2];


  unsigned int binmap[(128 / (1U << 5))];


  unsigned long trim_threshold;
  size_t top_pad;
  size_t mmap_threshold;


  int n_mmaps;
  int n_mmaps_max;
  int max_n_mmaps;


  unsigned int pagesize;


  size_t mmapped_mem;
  size_t sbrked_mem;
  size_t max_sbrked_mem;
  size_t max_mmapped_mem;
  size_t max_total_mem;

  void *(*__morecore)(size_t);
  unsigned long lock[3];
};
# 2201 "stdlib/malloc.c"
struct malloc_state __av_default;
# 2224 "stdlib/malloc.c"
static void malloc_init_state(mstate av)



{
  int i;
  mbinptr bin;


  for (i = 1; i < 128; ++i) {
    bin = ((mbinptr)((char*)&((av)->bins[(i)<<1]) - ((sizeof(size_t))<<1)));
    bin->fd = bin->bk = bin;
  }

  av->top_pad = (0);
  av->n_mmaps_max = (0);
  av->mmap_threshold = (128 * 1024);
  av->trim_threshold = (128 * 1024);


  ((av)->max_fast |= (2U));


  (av)->max_fast = (((64) == 0)? 8: (((64) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((64) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) | (1U) | ((av)->max_fast & (2U));

  av->top = ((((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1)))));
  av->pagesize = (4 * 4096);

  if (av->__morecore == 0)
      av->__morecore = __morecore;
}






static void* sYSMALLOc(size_t, mstate);
static int sYSTRIm(size_t, mstate);
static void malloc_consolidate(mstate);
static void** aiALLOc(mstate, size_t, size_t*, int, void**);
# 2616 "stdlib/malloc.c"
static void* sYSMALLOc(size_t nb, mstate av)



{
  mchunkptr old_top;
  size_t old_size;
  char* old_end;

  long size;
  char* brk;

  long correction;
  char* snd_brk;

  size_t front_misalign;
  size_t end_misalign;
  char* aligned_brk;

  mchunkptr p;
  mchunkptr remainder;
  unsigned long remainder_size;

  unsigned long sum;

  size_t pagemask = av->pagesize - 1;
# 2714 "stdlib/malloc.c"
  old_top = av->top;
  old_size = ((old_top)->size & ~((0x1|0x2)));
  old_end = (char*)(((mchunkptr)(((char*)(old_top)) + (old_size))));

  brk = snd_brk = (char*)(((void*)(-1)));






  ((void)0);




  ((void)0);


  ((void)0);




  size = nb + av->top_pad + (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)));







  if ((((av)->max_fast & (2U)) == 0))
    size -= old_size;
# 2757 "stdlib/malloc.c"
  size = (size + pagemask) & ~pagemask;







  if (size > 0)
    brk = (char*)(av->__morecore(size));
# 2810 "stdlib/malloc.c"
  if (brk != (char*)(((void*)(-1)))) {
    av->sbrked_mem += size;





    if (brk == old_end && snd_brk == (char*)(((void*)(-1)))) {
      ((old_top)->size = ((size + old_size) | 0x1));
    }
# 2840 "stdlib/malloc.c"
    else {
      front_misalign = 0;
      end_misalign = 0;
      correction = 0;
      aligned_brk = brk;


      if ((((av)->max_fast & (2U)) == 0)) {



        front_misalign = (size_t)((void*)((char*)(brk) + 2*(sizeof(size_t)))) & ((2 * (sizeof(size_t))) - 1);
        if (front_misalign > 0) {
# 2862 "stdlib/malloc.c"
          correction = (2 * (sizeof(size_t))) - front_misalign;
          aligned_brk += correction;
        }






        correction += old_size;


        end_misalign = (size_t)(brk + size + correction);
        correction += ((end_misalign + pagemask) & ~pagemask) - end_misalign;

        ((void)0);
        snd_brk = (char*)(av->__morecore(correction));
# 2890 "stdlib/malloc.c"
        if (snd_brk == (char*)(((void*)(-1)))) {
          correction = 0;
          snd_brk = (char*)(av->__morecore(0));
        }
      }


      else {

        ((void)0);


        if (snd_brk == (char*)(((void*)(-1)))) {
          snd_brk = (char*)(av->__morecore(0));
        }
      }


      if (snd_brk != (char*)(((void*)(-1)))) {
        av->top = (mchunkptr)aligned_brk;
        ((av->top)->size = ((snd_brk - aligned_brk + correction) | 0x1));
        av->sbrked_mem += correction;
# 2922 "stdlib/malloc.c"
        if (old_size != 0) {





          old_size = (old_size - 3*(sizeof(size_t))) & ~((2 * (sizeof(size_t))) - 1);
          ((old_top)->size = (old_size | 0x1));







          ((mchunkptr)(((char*)(old_top)) + (old_size)))->size =
            (sizeof(size_t))|0x1;

          ((mchunkptr)(((char*)(old_top)) + (old_size + (sizeof(size_t)))))->size =
            (sizeof(size_t))|0x1;


          if (old_size >= (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) {
            afREe(av, ((void*)((char*)(old_top) + 2*(sizeof(size_t)))));
          }

        }
      }
    }


    sum = av->sbrked_mem;
    if (sum > (unsigned long)(av->max_sbrked_mem))
      av->max_sbrked_mem = sum;

    sum += av->mmapped_mem;
    if (sum > (unsigned long)(av->max_total_mem))
      av->max_total_mem = sum;

    ;


    p = av->top;
    size = ((p)->size & ~((0x1|0x2)));


    if ((unsigned long)(size) >= (unsigned long)(nb + (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))))) {
      remainder_size = size - nb;
      remainder = ((mchunkptr)(((char*)(p)) + (nb)));
      av->top = remainder;
      ((p)->size = (nb | 0x1));
      ((remainder)->size = (remainder_size | 0x1));
      ;
      return ((void*)((char*)(p) + 2*(sizeof(size_t))));
    }
  }


  (*_geterrno()) = 9;;
  return 0;
}
# 2995 "stdlib/malloc.c"
static int sYSTRIm(size_t pad, mstate av)



{
  long top_size;
  long extra;
  long released;
  char* current_brk;
  char* new_brk;
  size_t pagesz;

  pagesz = av->pagesize;
  top_size = ((av->top)->size & ~((0x1|0x2)));


  extra = ((top_size - pad - (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) + (pagesz-1)) / pagesz - 1) * pagesz;

  if (extra > 0) {





    current_brk = (char*)(av->__morecore(0));
    if (current_brk == (char*)(av->top) + top_size) {
# 3032 "stdlib/malloc.c"
      av->__morecore(-extra);
      new_brk = (char*)(av->__morecore(0));

      if (new_brk != (char*)((void*)(-1))) {
        released = (long)(current_brk - new_brk);

        if (released != 0) {

          av->sbrked_mem -= released;
          ((av->top)->size = ((top_size - released) | 0x1));
          ;
          return 1;
        }
      }
    }
  }
  return 0;
}






void* amALLOc(mstate av, size_t bytes)



{


  size_t nb;
  unsigned int idx;
  mbinptr bin;
  mfastbinptr* fb;

  mchunkptr victim;
  size_t size;
  int victim_index;

  mchunkptr remainder;
  unsigned long remainder_size;

  unsigned int block;
  unsigned int bit;
  unsigned int map;

  mchunkptr fwd;
  mchunkptr bck;
# 3091 "stdlib/malloc.c"
  if (((unsigned long)(bytes) >= (unsigned long)(size_t)(-2 * (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))))) { (*_geterrno()) = 9;; return 0; } (nb) = (((bytes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((bytes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1));;







  if ((unsigned long)(nb) <= (unsigned long)(av->max_fast)) {
    fb = &(av->fastbins[(((((unsigned int)(nb)) >> 3) - 2))]);
    if ( (victim = *fb) != 0) {
      *fb = victim->fd;
      ;
      return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
    }
  }
# 3116 "stdlib/malloc.c"
  if (((unsigned long)(nb) < (unsigned long)512)) {
    idx = (((unsigned)(nb)) >> 3);
    bin = ((mbinptr)((char*)&((av)->bins[(idx)<<1]) - ((sizeof(size_t))<<1)));

    if ( (victim = ((bin)->bk)) != bin) {
      if (victim == 0)
        malloc_consolidate(av);
      else {
        bck = victim->bk;
        (((mchunkptr)(((char*)(victim)) + (nb)))->size |= 0x1);
        bin->bk = bck;
        bck->fd = bin;

        ;
        return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
      }
    }
  }
# 3146 "stdlib/malloc.c"
  else {
    idx = (((((unsigned long)(nb)) >> 6) <= 32)? 56 + (((unsigned long)(nb)) >> 6): ((((unsigned long)(nb)) >> 9) <= 20)? 91 + (((unsigned long)(nb)) >> 9): ((((unsigned long)(nb)) >> 12) <= 10)? 110 + (((unsigned long)(nb)) >> 12): ((((unsigned long)(nb)) >> 15) <= 4)? 119 + (((unsigned long)(nb)) >> 15): ((((unsigned long)(nb)) >> 18) <= 2)? 124 + (((unsigned long)(nb)) >> 18): 126);
    if ((((av)->max_fast & (1U)) == 0))
      malloc_consolidate(av);
  }
# 3165 "stdlib/malloc.c"
  for(;;) {

    while ( (victim = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->bk) != (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))) {
      bck = victim->bk;
      size = ((victim)->size & ~((0x1|0x2)));
# 3179 "stdlib/malloc.c"
      if (((unsigned long)(nb) < (unsigned long)512) &&
          bck == (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1)))) &&
          victim == av->last_remainder &&
          (unsigned long)(size) > (unsigned long)(nb + (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))))) {


        remainder_size = size - nb;
        remainder = ((mchunkptr)(((char*)(victim)) + (nb)));
        (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->bk = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->fd = remainder;
        av->last_remainder = remainder;
        remainder->bk = remainder->fd = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))));

        ((victim)->size = (nb | 0x1));
        ((remainder)->size = (remainder_size | 0x1));
        (((mchunkptr)((char*)(remainder) + (remainder_size)))->prev_size = (remainder_size));

        ;
        return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
      }


      (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->bk = bck;
      bck->fd = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))));



      if (size == nb) {
        (((mchunkptr)(((char*)(victim)) + (size)))->size |= 0x1);
        ;
        return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
      }



      if (((unsigned long)(size) < (unsigned long)512)) {
        victim_index = (((unsigned)(size)) >> 3);
        bck = ((mbinptr)((char*)&((av)->bins[(victim_index)<<1]) - ((sizeof(size_t))<<1)));
        fwd = bck->fd;
      }
      else {
        victim_index = (((((unsigned long)(size)) >> 6) <= 32)? 56 + (((unsigned long)(size)) >> 6): ((((unsigned long)(size)) >> 9) <= 20)? 91 + (((unsigned long)(size)) >> 9): ((((unsigned long)(size)) >> 12) <= 10)? 110 + (((unsigned long)(size)) >> 12): ((((unsigned long)(size)) >> 15) <= 4)? 119 + (((unsigned long)(size)) >> 15): ((((unsigned long)(size)) >> 18) <= 2)? 124 + (((unsigned long)(size)) >> 18): 126);
        bck = ((mbinptr)((char*)&((av)->bins[(victim_index)<<1]) - ((sizeof(size_t))<<1)));
        fwd = bck->fd;


        if (fwd != bck) {
          size |= 0x1;

          if ((unsigned long)(size) <= (unsigned long)(bck->bk->size)) {
            fwd = bck;
            bck = bck->bk;
          }
          else {
            while ((unsigned long)(size) < (unsigned long)(fwd->size))
              fwd = fwd->fd;
            bck = fwd->bk;
          }
        }
      }

      ((av)->binmap[((victim_index) >> 5)] |= ((1U << ((victim_index) & ((1U << 5)-1)))));
      victim->bk = bck;
      victim->fd = fwd;
      fwd->bk = victim;
      bck->fd = victim;
    }
# 3253 "stdlib/malloc.c"
    if (!((unsigned long)(nb) < (unsigned long)512)) {
      bin = ((mbinptr)((char*)&((av)->bins[(idx)<<1]) - ((sizeof(size_t))<<1)));


      if ((victim = ((bin)->bk)) != bin &&
          (unsigned long)(((bin)->fd)->size) >= (unsigned long)(nb)) {

        while (((unsigned long)(size = ((victim)->size & ~((0x1|0x2)))) <
                (unsigned long)(nb)))
          victim = victim->bk;

        remainder_size = size - nb;
        { fwd = victim->fd; bck = victim->bk; fwd->bk = bck; bck->fd = fwd; };


        if (remainder_size < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) {
          (((mchunkptr)(((char*)(victim)) + (size)))->size |= 0x1);
          ;
          return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
        }

        else {
          remainder = ((mchunkptr)(((char*)(victim)) + (nb)));
          (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->bk = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->fd = remainder;
          remainder->bk = remainder->fd = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))));
          ((victim)->size = (nb | 0x1));
          ((remainder)->size = (remainder_size | 0x1));
          (((mchunkptr)((char*)(remainder) + (remainder_size)))->prev_size = (remainder_size));
          ;
          return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
        }
      }
    }
# 3298 "stdlib/malloc.c"
    ++idx;
    bin = ((mbinptr)((char*)&((av)->bins[(idx)<<1]) - ((sizeof(size_t))<<1)));
    block = ((idx) >> 5);
    map = av->binmap[block];
    bit = ((1U << ((idx) & ((1U << 5)-1))));

    for (;;) {


      if (bit > map || bit == 0) {
        do {
          if (++block >= (128 / (1U << 5)))
            goto use_top;
        } while ( (map = av->binmap[block]) == 0);

        bin = ((mbinptr)((char*)&((av)->bins[((block << 5))<<1]) - ((sizeof(size_t))<<1)));
        bit = 1;
      }


      while ((bit & map) == 0) {
        bin = ((mbinptr)((char*)(bin) + (sizeof(mchunkptr)<<1)));
        bit <<= 1;
        ((void)0);
      }


      victim = ((bin)->bk);


      if (victim == bin) {
        av->binmap[block] = map &= ~bit;
        bin = ((mbinptr)((char*)(bin) + (sizeof(mchunkptr)<<1)));
        bit <<= 1;
      }

      else {
        size = ((victim)->size & ~((0x1|0x2)));


        ((void)0);

        remainder_size = size - nb;


        bck = victim->bk;
        bin->bk = bck;
        bck->fd = bin;


        if (remainder_size < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) {
          (((mchunkptr)(((char*)(victim)) + (size)))->size |= 0x1);
          ;
          return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
        }


        else {
          remainder = ((mchunkptr)(((char*)(victim)) + (nb)));

          (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->bk = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))))->fd = remainder;
          remainder->bk = remainder->fd = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))));

          if (((unsigned long)(nb) < (unsigned long)512))
            av->last_remainder = remainder;

          ((victim)->size = (nb | 0x1));
          ((remainder)->size = (remainder_size | 0x1));
          (((mchunkptr)((char*)(remainder) + (remainder_size)))->prev_size = (remainder_size));
          ;
          return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
        }
      }
    }

  use_top:
# 3389 "stdlib/malloc.c"
    victim = av->top;
    size = ((victim)->size & ~((0x1|0x2)));

    if ((unsigned long)(size) >= (unsigned long)(nb + (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))))) {
      remainder_size = size - nb;
      remainder = ((mchunkptr)(((char*)(victim)) + (nb)));
      av->top = remainder;
      ((victim)->size = (nb | 0x1));
      ((remainder)->size = (remainder_size | 0x1));

      ;
      return ((void*)((char*)(victim) + 2*(sizeof(size_t))));
    }







    else if ((((av)->max_fast & (1U)) == 0)) {
      ((void)0);
      malloc_consolidate(av);
      idx = (((unsigned)(nb)) >> 3);
    }




    else
      return sYSMALLOc(nb, av);
  }
}






void afREe(mstate av, void* mem)



{


  mchunkptr p;
  size_t size;
  mfastbinptr* fb;
  mchunkptr nextchunk;
  size_t nextsize;
  int nextinuse;
  size_t prevsize;
  mchunkptr bck;
  mchunkptr fwd;



  if (mem != 0) {
    p = ((mchunkptr)((char*)(mem) - 2*(sizeof(size_t))));
    size = ((p)->size & ~((0x1|0x2)));

    ;






    if ((unsigned long)(size) <= (unsigned long)(av->max_fast)
# 3467 "stdlib/malloc.c"
        ) {

      ((av)->max_fast &= ~(1U));
      fb = &(av->fastbins[((((unsigned int)(size)) >> 3) - 2)]);
      p->fd = *fb;
      *fb = p;
    }





    else if (!((p)->size & 0x2)) {
      nextchunk = ((mchunkptr)(((char*)(p)) + (size)));
      nextsize = ((nextchunk)->size & ~((0x1|0x2)));


      if (!((p)->size & 0x1)) {
        prevsize = p->prev_size;
        size += prevsize;
        p = ((mchunkptr)(((char*)(p)) + (-((long) prevsize))));
        { fwd = p->fd; bck = p->bk; fwd->bk = bck; bck->fd = fwd; };
      }

      if (nextchunk != av->top) {

        nextinuse = (((mchunkptr)(((char*)(nextchunk)) + (nextsize)))->size & 0x1);
        ((nextchunk)->size = (nextsize));


        if (!nextinuse) {
          { fwd = nextchunk->fd; bck = nextchunk->bk; fwd->bk = bck; bck->fd = fwd; };
          size += nextsize;
        }







        bck = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))));
        fwd = bck->fd;
        p->bk = bck;
        p->fd = fwd;
        bck->fd = p;
        fwd->bk = p;

        ((p)->size = (size | 0x1));
        (((mchunkptr)((char*)(p) + (size)))->prev_size = (size));

        ;
      }






      else {
        size += nextsize;
        ((p)->size = (size | 0x1));
        av->top = p;
        ;
      }
# 3546 "stdlib/malloc.c"
      if ((unsigned long)(size) >= (65536UL)) {
        if ((((av)->max_fast & (1U)) == 0))
          malloc_consolidate(av);






      }

    }
# 3566 "stdlib/malloc.c"
    else {
# 3576 "stdlib/malloc.c"
    }
  }
}
# 3595 "stdlib/malloc.c"
static void malloc_consolidate(mstate av)



{
  mfastbinptr* fb;
  mfastbinptr* maxfb;
  mchunkptr p;
  mchunkptr nextp;
  mchunkptr unsorted_bin;
  mchunkptr first_unsorted;


  mchunkptr nextchunk;
  size_t size;
  size_t nextsize;
  size_t prevsize;
  int nextinuse;
  mchunkptr bck;
  mchunkptr fwd;






  if (av->max_fast != 0) {
    ((av)->max_fast |= (1U));

    unsorted_bin = (((mbinptr)((char*)&((av)->bins[(1)<<1]) - ((sizeof(size_t))<<1))));
# 3634 "stdlib/malloc.c"
    maxfb = &(av->fastbins[((((unsigned int)(av->max_fast)) >> 3) - 2)]);
    fb = &(av->fastbins[0]);
    do {
      if ( (p = *fb) != 0) {
        *fb = 0;

        do {
          ;
          nextp = p->fd;


          size = p->size & ~0x1;
          nextchunk = ((mchunkptr)(((char*)(p)) + (size)));
          nextsize = ((nextchunk)->size & ~((0x1|0x2)));

          if (!((p)->size & 0x1)) {
            prevsize = p->prev_size;
            size += prevsize;
            p = ((mchunkptr)(((char*)(p)) + (-((long) prevsize))));
            { fwd = p->fd; bck = p->bk; fwd->bk = bck; bck->fd = fwd; };
          }

          if (nextchunk != av->top) {
            nextinuse = (((mchunkptr)(((char*)(nextchunk)) + (nextsize)))->size & 0x1);
            ((nextchunk)->size = (nextsize));

            if (!nextinuse) {
              size += nextsize;
              { fwd = nextchunk->fd; bck = nextchunk->bk; fwd->bk = bck; bck->fd = fwd; };
            }

            first_unsorted = unsorted_bin->fd;
            unsorted_bin->fd = p;
            first_unsorted->bk = p;

            ((p)->size = (size | 0x1));
            p->bk = unsorted_bin;
            p->fd = first_unsorted;
            (((mchunkptr)((char*)(p) + (size)))->prev_size = (size));
          }

          else {
            size += nextsize;
            ((p)->size = (size | 0x1));
            av->top = p;
          }

        } while ( (p = nextp) != 0);

      }
    } while (fb++ != maxfb);
  }
  else {
    malloc_init_state(av);
    ;
  }
}







void* arEALLOc(mstate av, void* oldmem, size_t bytes)



{


  size_t nb;

  mchunkptr oldp;
  size_t oldsize;

  mchunkptr newp;
  size_t newsize;
  void* newmem;

  mchunkptr next;

  mchunkptr remainder;
  unsigned long remainder_size;

  mchunkptr bck;
  mchunkptr fwd;

  unsigned long copysize;
  unsigned int ncopies;
  size_t* s;
  size_t* d;
# 3736 "stdlib/malloc.c"
  if (oldmem == 0) return amALLOc(av, bytes);

  if (((unsigned long)(bytes) >= (unsigned long)(size_t)(-2 * (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))))) { (*_geterrno()) = 9;; return 0; } (nb) = (((bytes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((bytes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1));;

  oldp = ((mchunkptr)((char*)(oldmem) - 2*(sizeof(size_t))));
  oldsize = ((oldp)->size & ~((0x1|0x2)));

  ;

  if (!((oldp)->size & 0x2)) {

    if ((unsigned long)(oldsize) >= (unsigned long)(nb)) {

      newp = oldp;
      newsize = oldsize;
    }

    else {
      next = ((mchunkptr)(((char*)(oldp)) + (oldsize)));


      if (next == av->top &&
          (unsigned long)(newsize = oldsize + ((next)->size & ~((0x1|0x2)))) >=
          (unsigned long)(nb + (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))))) {
        ((oldp)->size = (((oldp)->size & 0x1) | (nb)));
        av->top = ((mchunkptr)(((char*)(oldp)) + (nb)));
        ((av->top)->size = ((newsize - nb) | 0x1));
        return ((void*)((char*)(oldp) + 2*(sizeof(size_t))));
      }


      else if (next != av->top &&
               !((((mchunkptr)(((char*)(next))+((next)->size & ~0x1)))->size) & 0x1) &&
               (unsigned long)(newsize = oldsize + ((next)->size & ~((0x1|0x2)))) >=
               (unsigned long)(nb)) {
        newp = oldp;
        { fwd = next->fd; bck = next->bk; fwd->bk = bck; bck->fd = fwd; };
      }


      else {
        newmem = amALLOc(av, nb - ((2 * (sizeof(size_t))) - 1));
        if (newmem == 0)
          return 0;

        newp = ((mchunkptr)((char*)(newmem) - 2*(sizeof(size_t))));
        newsize = ((newp)->size & ~((0x1|0x2)));




        if (newp == next) {
          newsize += oldsize;
          newp = oldp;
        }
        else {






          copysize = oldsize - (sizeof(size_t));
          s = (size_t*)(oldmem);
          d = (size_t*)(newmem);
          ncopies = copysize / sizeof(size_t);
          ((void)0);

          if (ncopies > 9)
            memcpy(d, s, copysize);

          else {
            *(d+0) = *(s+0);
            *(d+1) = *(s+1);
            *(d+2) = *(s+2);
            if (ncopies > 4) {
              *(d+3) = *(s+3);
              *(d+4) = *(s+4);
              if (ncopies > 6) {
                *(d+5) = *(s+5);
                *(d+6) = *(s+6);
                if (ncopies > 8) {
                  *(d+7) = *(s+7);
                  *(d+8) = *(s+8);
                }
              }
            }
          }

          afREe(av, oldmem);
          ;
          return ((void*)((char*)(newp) + 2*(sizeof(size_t))));
        }
      }
    }



    ((void)0);

    remainder_size = newsize - nb;

    if (remainder_size < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) {
      ((newp)->size = (((newp)->size & 0x1) | (newsize)));
      (((mchunkptr)(((char*)(newp)) + (newsize)))->size |= 0x1);
    }
    else {
      remainder = ((mchunkptr)(((char*)(newp)) + (nb)));
      ((newp)->size = (((newp)->size & 0x1) | (nb)));
      ((remainder)->size = (remainder_size | 0x1));

      (((mchunkptr)(((char*)(remainder)) + (remainder_size)))->size |= 0x1);
      afREe(av, ((void*)((char*)(remainder) + 2*(sizeof(size_t)))));
    }

    ;
    return ((void*)((char*)(newp) + 2*(sizeof(size_t))));
  }





  else {
# 3912 "stdlib/malloc.c"
    ;
    (*_geterrno()) = 9;;
    return 0;

  }
}






void* amEMALIGn(mstate av, size_t alignment, size_t bytes)



{
  size_t nb;
  char* m;
  mchunkptr p;
  char* brk;
  mchunkptr newp;
  size_t newsize;
  size_t leadsize;
  mchunkptr remainder;
  unsigned long remainder_size;
  size_t size;



  if (alignment <= (2 * (sizeof(size_t)))) return amALLOc(av, bytes);



  if (alignment < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) alignment = (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)));


  if ((alignment & (alignment - 1)) != 0) {
    size_t a = (2 * (sizeof(size_t))) * 2;
    while ((unsigned long)a < (unsigned long)alignment) a <<= 1;
    alignment = a;
  }

  if (((unsigned long)(bytes) >= (unsigned long)(size_t)(-2 * (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))))) { (*_geterrno()) = 9;; return 0; } (nb) = (((bytes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((bytes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1));;
# 3965 "stdlib/malloc.c"
  m = (char*)(amALLOc(av, nb + alignment + (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))));

  if (m == 0) return 0;

  p = ((mchunkptr)((char*)(m) - 2*(sizeof(size_t))));

  if ((((unsigned long)(m)) % alignment) != 0) {
# 3981 "stdlib/malloc.c"
    brk = (char*)((mchunkptr)((char*)(((unsigned long)(m + alignment - 1)) & -((signed long) alignment)) - 2*(sizeof(size_t))));

    if ((unsigned long)(brk - (char*)(p)) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))))
      brk += alignment;

    newp = (mchunkptr)brk;
    leadsize = brk - (char*)(p);
    newsize = ((p)->size & ~((0x1|0x2))) - leadsize;


    if (((p)->size & 0x2)) {
      newp->prev_size = p->prev_size + leadsize;
      ((newp)->size = (newsize|0x2));
      return ((void*)((char*)(newp) + 2*(sizeof(size_t))));
    }


    ((newp)->size = (newsize | 0x1));
    (((mchunkptr)(((char*)(newp)) + (newsize)))->size |= 0x1);
    ((p)->size = (((p)->size & 0x1) | (leadsize)));
    afREe(av, ((void*)((char*)(p) + 2*(sizeof(size_t)))));
    p = newp;

    ((void)0);

  }


  if (!((p)->size & 0x2)) {
    size = ((p)->size & ~((0x1|0x2)));
    if ((unsigned long)(size) > (unsigned long)(nb + (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))))) {
      remainder_size = size - nb;
      remainder = ((mchunkptr)(((char*)(p)) + (nb)));
      ((remainder)->size = (remainder_size | 0x1));
      ((p)->size = (((p)->size & 0x1) | (nb)));
      afREe(av, ((void*)((char*)(remainder) + 2*(sizeof(size_t)))));
    }
  }

  ;
  return ((void*)((char*)(p) + 2*(sizeof(size_t))));
}






void* acALLOc(mstate av, size_t n_elements, size_t elem_size)



{
  mchunkptr p;
  unsigned long clearsize;
  unsigned long nclears;
  size_t* d;

  void* mem = amALLOc(av, n_elements * elem_size);

  if (mem != 0) {
    p = ((mchunkptr)((char*)(mem) - 2*(sizeof(size_t))));


    if (!((p)->size & 0x2))

    {






      d = (size_t*)mem;
      clearsize = ((p)->size & ~((0x1|0x2))) - (sizeof(size_t));
      nclears = clearsize / sizeof(size_t);
      ((void)0);

      if (nclears > 9)
        memset(d, 0, clearsize);

      else {
        *(d+0) = 0;
        *(d+1) = 0;
        *(d+2) = 0;
        if (nclears > 4) {
          *(d+3) = 0;
          *(d+4) = 0;
          if (nclears > 6) {
            *(d+5) = 0;
            *(d+6) = 0;
            if (nclears > 8) {
              *(d+7) = 0;
              *(d+8) = 0;
            }
          }
        }
      }
    }
  }
  return mem;
}






void acFREe(mstate av, void *mem)



{
  afREe(av, mem);
}






void** iaCALLOc(mstate av, size_t n_elements, size_t elem_size, void* chunks[])



{
  size_t sz = elem_size;

  return aiALLOc(av, n_elements, &sz, 3, chunks);
}






void** iaCOMALLOc(mstate av, size_t n_elements, size_t sizes[], void* chunks[])



{
  return aiALLOc(av, n_elements, sizes, 0, chunks);
}
# 4138 "stdlib/malloc.c"
static void** aiALLOc(mstate av,
                       size_t n_elements,
                       size_t* sizes,
                       int opts,
                       void* chunks[])



{

  size_t element_size;
  size_t contents_size;
  size_t array_size;
  void* mem;
  mchunkptr p;
  size_t remainder_size;
  void** marray;
  mchunkptr array_chunk;
  int mmx;
  size_t size;
  size_t i;


  if ((((av)->max_fast & (1U)) == 0)) malloc_consolidate(av);


  if (chunks != 0) {
    if (n_elements == 0)
      return chunks;
    marray = chunks;
    array_size = 0;
  }
  else {

    if (n_elements == 0)
      return (void**) amALLOc(av, 0);
    marray = 0;
    array_size = (((n_elements * (sizeof(void*))) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((n_elements * (sizeof(void*))) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1));
  }


  if (opts & 0x1) {
    element_size = (((*sizes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((*sizes) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1));
    contents_size = n_elements * element_size;
  }
  else {
    element_size = 0;
    contents_size = 0;
    for (i = 0; i != n_elements; ++i)
      contents_size += (((sizes[i]) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((sizes[i]) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1));
  }


  size = contents_size + array_size - ((2 * (sizeof(size_t))) - 1);







  mmx = av->n_mmaps_max;
  av->n_mmaps_max = 0;
  mem = amALLOc(av, size);
  av->n_mmaps_max = mmx;
  if (mem == 0)
    return 0;

  p = ((mchunkptr)((char*)(mem) - 2*(sizeof(size_t))));
  ((void)0);
  remainder_size = ((p)->size & ~((0x1|0x2)));

  if (opts & 0x2) {
    memset(mem, 0, remainder_size - (sizeof(size_t)) - array_size);
  }


  if (marray == 0) {
    array_chunk = ((mchunkptr)(((char*)(p)) + (contents_size)));
    marray = (void**) (((void*)((char*)(array_chunk) + 2*(sizeof(size_t)))));
    ((array_chunk)->size = ((remainder_size - contents_size) | 0x1));
    remainder_size = contents_size;
  }


  for (i = 0; ; ++i) {
    marray[i] = ((void*)((char*)(p) + 2*(sizeof(size_t))));
    if (i != n_elements-1) {
      if (element_size != 0)
        size = element_size;
      else
        size = (((sizes[i]) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((sizes[i]) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1));
      remainder_size -= size;
      ((p)->size = (size | 0x1));
      p = ((mchunkptr)(((char*)(p)) + (size)));
    }
    else {
      ((p)->size = (remainder_size | 0x1));
      break;
    }
  }
# 4254 "stdlib/malloc.c"
  return marray;
}







void* avALLOc(mstate av, size_t bytes)



{


  if ((((av)->max_fast & (1U)) == 0)) malloc_consolidate(av);
  return amEMALIGn(av, av->pagesize, bytes);
}







void* apVALLOc(mstate av, size_t bytes)



{

  size_t pagesz;


  if ((((av)->max_fast & (1U)) == 0)) malloc_consolidate(av);
  pagesz = av->pagesize;
  return amEMALIGn(av, pagesz, (bytes + pagesz - 1) & ~(pagesz - 1));
}






mstate malloc_create_heap(mstate av, void *(*allocator)(size_t))



{
    mstate ret;

    ret = amALLOc(av, sizeof(*ret));
    if (ret == 0)
        return 0;

        memset(ret, 0, sizeof(*ret));
    malloc_init_state(ret);
    ret->__morecore = allocator;
    return ret;
}






void malloc_delete_heap(mstate av)



{

}






int amTRIm(mstate av, size_t pad)



{


  malloc_consolidate(av);




  return 0;

}







size_t amUSABLe(mstate av, void* mem)



{
  mchunkptr p;
  if (mem != 0) {
    p = ((mchunkptr)((char*)(mem) - 2*(sizeof(size_t))));
    if (((p)->size & 0x2))
      return ((p)->size & ~((0x1|0x2))) - 2*(sizeof(size_t));
    else if (((((mchunkptr)(((char*)(p))+((p)->size & ~0x1)))->size) & 0x1))
      return ((p)->size & ~((0x1|0x2))) - (sizeof(size_t));
  }
  return 0;
}





struct mallinfo amALLINFo(mstate av)
{

  struct mallinfo mi;
  int i;
  mbinptr b;
  mchunkptr p;
  size_t avail;
  size_t fastavail;
  int nblocks;
  int nfastblocks;


  if (av->top == 0) malloc_consolidate(av);

  ;


  avail = ((av->top)->size & ~((0x1|0x2)));
  nblocks = 1;


  nfastblocks = 0;
  fastavail = 0;

  for (i = 0; i < (((((unsigned int)((((80) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((80) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) >> 3) - 2)+1); ++i) {
    for (p = av->fastbins[i]; p != 0; p = p->fd) {
      ++nfastblocks;
      fastavail += ((p)->size & ~((0x1|0x2)));
    }
  }

  avail += fastavail;


  for (i = 1; i < 128; ++i) {
    b = ((mbinptr)((char*)&((av)->bins[(i)<<1]) - ((sizeof(size_t))<<1)));
    for (p = ((b)->bk); p != b; p = p->bk) {
      ++nblocks;
      avail += ((p)->size & ~((0x1|0x2)));
    }
  }

  mi.smblks = nfastblocks;
  mi.ordblks = nblocks;
  mi.fordblks = avail;
  mi.uordblks = av->sbrked_mem - avail;
  mi.arena = av->sbrked_mem;
  mi.hblks = av->n_mmaps;
  mi.hblkhd = av->mmapped_mem;
  mi.fsmblks = fastavail;
  mi.keepcost = ((av->top)->size & ~((0x1|0x2)));
  mi.usmblks = av->max_total_mem;
  return mi;
}





void amSTATs(mstate av)
{
  struct mallinfo mi = amALLINFo(av);
# 4454 "stdlib/malloc.c"
  fprintf((&__dj_stderr), "max system bytes = %10lu\n",
          (unsigned long)(mi.usmblks));
  fprintf((&__dj_stderr), "system bytes     = %10lu\n",
          (unsigned long)(mi.arena + mi.hblkhd));
  fprintf((&__dj_stderr), "in use bytes     = %10lu\n",
          (unsigned long)(mi.uordblks + mi.hblkhd));
# 4473 "stdlib/malloc.c"
}







int amALLOPt(mstate av, int param_number, int value)



{


  malloc_consolidate(av);

  switch(param_number) {
  case 1:
    if (value >= 0 && value <= 80) {
      (av)->max_fast = (((value) == 0)? 8: (((value) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1) < (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1)))) ? (unsigned long)((((sizeof(struct malloc_chunk))+((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) : ((value) + (sizeof(size_t)) + ((2 * (sizeof(size_t))) - 1)) & ~((2 * (sizeof(size_t))) - 1))) | (1U) | ((av)->max_fast & (2U));
      return 1;
    }
    else
      return 0;

  case -1:
    av->trim_threshold = value;
    return 1;

  case -2:
    av->top_pad = value;
    return 1;

  case -3:
    av->mmap_threshold = value;
    return 1;

  case -4:

    if (value != 0)
      return 0;

    av->n_mmaps_max = value;
    return 1;

  default:
    return 0;
  }
}
# 5203 "stdlib/malloc.c"
void* amalloc(mstate av, size_t bytes) {
  void* m;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = amALLOc(av, bytes);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

void afree(mstate av, void* m) {
  if (__malloc_lock(1, av->lock) != 0) {
    return;
  }
  afREe(av, m);
  if (__malloc_lock(0, av->lock) != 0) {
  }
}

void* arealloc(mstate av, void* m, size_t bytes) {
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = arEALLOc(av, m, bytes);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

void* amemalign(mstate av, size_t alignment, size_t bytes) {
  void* m;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = amEMALIGn(av, alignment, bytes);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

void* avalloc(mstate av, size_t bytes) {
  void* m;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = avALLOc(av, bytes);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

void* apvalloc(mstate av, size_t bytes) {
  void* m;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = apVALLOc(av, bytes);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

void* acalloc(mstate av, size_t n, size_t elem_size) {
  void* m;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = acALLOc(av, n, elem_size);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}


void** independent_acalloc(mstate av, size_t n, size_t elem_size, void** chunks) {
  void** m;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = iaCALLOc(av, n, elem_size, chunks);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

void** independent_acomalloc(mstate av, size_t n, size_t sizes[], void** chunks) {
  void** m;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  m = iaCOMALLOc(av, n, sizes, chunks);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

void acfree(mstate av, void* m) {
  if (__malloc_lock(1, av->lock) != 0) {
    return;
  }
  acFREe(av, m);
  if (__malloc_lock(0, av->lock) != 0) {
  }
}

int malloc_atrim(mstate av, size_t s) {
  int result;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  result = amTRIm(av, s);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return result;
}

size_t malloc_usable_size(mstate av, void* m) {
  size_t result;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  result = amUSABLe(av, m);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return result;
}

void malloc_astats(mstate av) {
  if (__malloc_lock(1, av->lock) != 0) {
    return;
  }
  amSTATs(av);
  if (__malloc_lock(0, av->lock) != 0) {
  }
}

struct mallinfo public_amALLINFo(mstate av) {
  struct mallinfo m;
  if (__malloc_lock(1, av->lock) != 0) {
    struct mallinfo nm = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    return nm;
  }
  m = amALLINFo(av);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return m;
}

int public_amALLOPt(mstate av, int p, int v) {
  int result;
  if (__malloc_lock(1, av->lock) != 0) {
    return 0;
  }
  result = amALLOPt(av, p, v);
  if (__malloc_lock(0, av->lock) != 0) {
  }
  return result;
}
