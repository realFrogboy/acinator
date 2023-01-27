#ifndef ONEGIN_H_
#define ONEGIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#ifndef ASSERT
#define ASSERT(statement, text) do {                                                                                    \
                                     if (statement) {                                                                   \
                                        printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
                                        printf (#text);                                                                 \
                                                                                                                        \
                                        return 1;                                                                       \
                                    }                                                                                   \
                                } while (0)
#endif 

enum RESULTS
{
    FAIL = 1,
    SUCCESS = 0,
};

struct line
{
    char* refonBegin;
    int lenStr;
};

int open_f (FILE** input, FILE** output);
int countSymbol (FILE* file);
char* read_f (FILE* input);
int compare_f (const void* p1, const void* p2);
void compare_l (struct line* string, int numStrings);
void scanFile (int len, struct line* string, char* str_in);
int countStrings (int len, char* str_in);
void put_res (struct line* string, int numN, FILE* output);
void splitFileFirst (FILE* output);
void splitFileLast (FILE* output);
void returnStrings (char* str_in, int numSymbol);
void cleanBuffer();

int lineCtor(line *arr, int nElem);
int lineDtor(line *arr, int nElem);

#endif