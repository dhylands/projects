/****************************************************************************
*
*   The following code snippet came from a post on AVR Freaks
*
*   http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=59453&start=all&postdays=0&postorder=asc
*
****************************************************************************/

#include <stdlib.h>

void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {};

extern "C" void __cxa_pure_virtual(void);
void __cxa_pure_virtual(void) {};

