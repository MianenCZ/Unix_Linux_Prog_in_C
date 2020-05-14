//source: https://gist.github.com/maestrelli/1191287


#ifndef MEMORY_DEF_H
#define MEMORY_DEF_H
#include <stdlib.h>
#include <stdio.h>

#define ERROR_ALLOC_MESSAGE "Insufficient memory"

#define MALLOC(p, n) \
do \
{ \
  if ( !( (p) = malloc(sizeof(*(p)) * (n)) ) ) \
  { \
    fprintf(stderr, ERROR_ALLOC_MESSAGE); \
    exit(EXIT_FAILURE); \
  } \
} \
while(0)

#define CALLOC(p, n) \
do \
{ \
  if ( !( (p) = calloc((n), sizeof(*(p))) ) ) \
  { \
    fprintf(stderr, ERROR_ALLOC_MESSAGE); \
    exit(EXIT_FAILURE); \
  } \
} \
while(0)

#define REALLOC(p, n) \
do \
{ \
  if ( !( (p) = realloc((p), sizeof(*(p)) * (n)) ) ) \
  { \
    fprintf(stderr, ERROR_ALLOC_MESSAGE); \
    exit(EXIT_FAILURE); \
  } \
} \
while(0)

#define FREE_S(p) \
do \
{ \
  if(p != NULL) {\
  free(p); \
  p = NULL; \
  }\
} \
while(0)

#endif