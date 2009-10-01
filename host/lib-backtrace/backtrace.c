/* print a backtrace.
   written by Mark Hahn, SHARCnet, 2007.

gcc -fPIC backtrace.c /usr/lib64/libbfd-2.15.92.0.2.so -shared -o backtrace.so

using -lbfd chokes on a symbol addressing issue with (static) libbfd.a 
on my system.  your libbfd version number may differ.

LD_PRELOAD=./backtrace.so ./tester
signal(11)
Obtained 9 stack frames.
file: /home/hahn/private/tester.c, line: 10, func dosegv
file: /home/hahn/private/tester.c, line: 14, func bar
file: /home/hahn/private/tester.c, line: 17, func foo
file: /home/hahn/private/tester.c, line: 29, func main

all symbols (globals and functions) are static to avoid contamination.

you need -g on the target program, and potentially something like
-fno-inline-functions to dissuade the compiler from disappearing some 
functions.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <signal.h>
#include <bfd.h>
#include <unistd.h>

#define MAX_FRAMES (20)

/* globals retained across calls to resolve. */
static bfd* abfd = 0;
static asymbol **syms = 0;
static asection *text = 0;

static void resolve(char *address) {
    if (!abfd) {
    char ename[1024];
    int l = readlink("/proc/self/exe",ename,sizeof(ename));
    if (l == -1) {
        perror("failed to find executable\n");
        return;
    }
    ename[l] = 0;

    bfd_init();

    abfd = bfd_openr(ename, 0);
    if (!abfd) {
        perror("bfd_openr failed: ");
        return;
    }
    /* oddly, this is required for it to work... */
    bfd_check_format(abfd,bfd_object);

    unsigned storage_needed = bfd_get_symtab_upper_bound(abfd);
    syms = (asymbol **) malloc(storage_needed);
    unsigned cSymbols = bfd_canonicalize_symtab(abfd, syms);

    text = bfd_get_section_by_name(abfd, ".text");
    }
    long offset = ((long)address) - text->vma;
    if (offset > 0) {
        const char *file;
        const char *func;
        unsigned line;
        if (bfd_find_nearest_line(abfd, text, syms, offset, &file, 
&func, &line) && file)
            printf("file: %s, line: %u, func %s\n",file,line,func);
    }
}

static void print_trace() {
    void *array[MAX_FRAMES];
    size_t size;
    size_t i;
    void *approx_text_end = (void*) ((128+100) * 2<<20);

    size = backtrace (array, MAX_FRAMES);
    printf ("Obtained %zd stack frames.\n", size);
    for (i = 0; i < size; i++) {
    if (array[i] < approx_text_end) {
        resolve(array[i]);
    }
    }
}

static void handler(int sig) {
    printf("signal(%d)\n",sig);
    print_trace();
    _exit(1);
}

static void __attribute__((constructor)) init() {
    static struct sigaction sa;
    sa.sa_handler = handler;
    sigaction(SIGABRT, &sa, 0);
    sigaction(SIGFPE, &sa, 0);
    sigaction(SIGSEGV, &sa, 0);
}

