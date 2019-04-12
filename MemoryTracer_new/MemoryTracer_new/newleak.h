#ifndef __MEMTRACE_H__
#define __MEMTRACE_H__

void * operator new (size_t size, char *File, int Line);
void * operator new[](size_t size, char *File, int Line);
void operator delete (void * p);
void operator delete[](void * p);

#define new new(__FILE__, __LINE__)

#endif