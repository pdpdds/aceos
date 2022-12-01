#ifndef __STRING_H
#define __STRING_H
/*---------------------------------------------------------------------------*/
/* string.h - string header file                                             */
/*---------------------------------------------------------------------------*/
#ifndef _SIZE_T_DEFINED
  typedef unsigned int size_t;
  #define _SIZE_T_DEFINED
#endif
#include <TypeDefs.h>
/*---------------------------------------------------------------------------*/
/* function prototpes                                                        */
/*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

void *memccpy(void *dest, const void *src, int c, unsigned int count);
void *memchr(const void *buf, int c, size_t count);
int memcmp(const void *buf1, const void *buf2, size_t count);
int memicmp(const void *buf1, const void *buf2, unsigned int count);
void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t count);
void *memset(void *dest, int c, size_t count);

int strnicmp(const char *string1, const char *string2, size_t count);
char *strnchr(const char *string, int c,size_t count);


char *strcat(char *string1, const char *string2);
char *strchr(const char *string, int c);
int strcmp(const char *string1, const char *string2);
int stricmp(const char *string1, const char *string2);
char *strcpy(char *string1, const char *string2);
size_t strcspn(const char *string1, const char *string2);
char *strdup(const char *string);
char *strerror(int errnum);
size_t strlen(const char *string);
char *strlwr(char *string);
char *strncat(char *string1, const char *string2, size_t count);
int strncmp(const char *string1, const char *string2, size_t count);

char *strncpy(char *string1, const char *string2, size_t count);
char *strnset(char *string, int c, size_t count);
char *strpbrk(const char *string1, const char *string2);
char *strrchr(const char *string, int c);
char *strrev(char *string);
char *strset(char *string, int c);
size_t strspn(const char *string1, const char *string2);
char *strstr(const char *string1, const char *string2);
char *strtok(char *string1, const char *string2);
char *strupr(char *string);

#ifdef __cplusplus
}
#endif

#endif

