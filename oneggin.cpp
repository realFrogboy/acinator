#include "oneggin.h"


int countSymbol (FILE* file)
{
    assert (file);

    fseek (file, 0L, SEEK_END); //return
    int numSymbol = ftell (file);
    fseek (file, 0L, SEEK_SET);

    return numSymbol;
}


char* read_f (FILE* input)
{
    assert (input);

    int numSymbol = countSymbol(input);

    char *str = (char*)calloc(numSymbol + 1, sizeof(char));

    int ret_code = fread (str, sizeof (char), numSymbol, input);
    assert(ret_code == numSymbol);

    str[numSymbol] = '\0';
    
    return str;
}


int compare_f (const void* ptr1, const void* ptr2)  //rename
{
    assert (ptr1);
    assert (ptr2);
    assert (ptr1 != ptr2);

    const struct line* ptrStruct1 = (const struct line*) ptr1;
    const struct line* ptrStruct2 = (const struct line*) ptr2;

    int min = 0; //rename
    if (ptrStruct1->lenStr > ptrStruct2->lenStr)
        min = ptrStruct2->lenStr;
    else                                          //func
        min = ptrStruct1->lenStr;

    int res = strncmp (ptrStruct1->refonBegin, ptrStruct2->refonBegin, min);

    return res;
}


/*void compare_l (struct line* string, int numStrings)
{
    assert (string);

    int num_f = 0, num_s = 0;

    for (num_f = 0; num_f < numStrings - 1; num_f++)
    {
        for (num_s = num_f + 1; num_s < numStrings; num_s++)
        {
            int minLen = 0;


            if (string[num_f].lenStr > string[num_s].lenStr)
                minLen = string[num_s].lenStr;
            else
                minLen = string[num_f].lenStr;


            int ind = 0;
            for (ind = 1; ind <= minLen; ind++)                    //tolower
            {
                char* refonEnd_f = string[num_f].refonBegin + string[num_f].lenStr - ind;
                char* refonEnd_s = string[num_s].refonBegin + string[num_s].lenStr - ind;

                if (*(refonEnd_f) == *(refonEnd_s)) continue;

                else if (*(refonEnd_f) > *(refonEnd_s))
                    {
                        SWAP_CHAR (char, string[num_f].refonBegin, string[num_s].refonBegin);
                        SWAP_INT (int, string[num_f].lenStr, string[num_s].lenStr);

                        char* c = string[num_f].refonBegin; //rename func swap
                        int len = string[num_f].lenStr;
                        string[num_f].refonBegin = string[num_s].refonBegin;
                        string[num_f].lenStr = string[num_s].lenStr;
                        string[num_s].refonBegin = c;
                        string[num_s].lenStr = len;
                        break; 
                    }

                else if (*(refonEnd_f) < *(refonEnd_s)) break;
            }
        }
    }
}*/


void scanFile (int numSymbol, struct line* string, char* str_in) //rename
{
    assert (string);
    assert (str_in);

    int num = 0, numBeg = 0, ind = 0;

    for (num = 0; num <= numSymbol; num++)
    {
        if (string[ind].refonBegin == NULL)
        {
            string[ind].refonBegin = &str_in[num];
            numBeg = num;
        }

        if (str_in[num] == '\0')
        {
            //int num1 = num;   //rename

            string[ind].lenStr = num - numBeg;

            /*while (!isalpha (str_in[num1]))
            {
                string[ind].lenStr--;
                num1--;
            }*/

            string[ind].lenStr++;

            ind++;
        }
    }
}


int countStrings (int numSymbol, char* str_in)
{
    assert (isfinite (numSymbol));
    assert (str_in);

    int num = 0, numStrings = 0;

    for (num = 0; num < numSymbol; num++)
        if (str_in[num] == '\n')
        {
            str_in[num] = '\0';
            numStrings++;
        }

    str_in[num] = '\0';
    numStrings++;

    return numStrings;
}


void put_res (struct line* string, const int numStrings, FILE* output)
{
    assert (isfinite (numStrings));
    assert (string);
    assert (output);

    int num = 0;

    for (num = 0; num < numStrings; num++)
    {
        fputs (string[num].refonBegin, output);
        fputs ("\n", output);
    }
}


void splitFileFirst (FILE* output)
{
    assert (output);

    fputs ("\nBy the last letters\n\n", output);
}


void splitFileLast (FILE* output)
{
    assert (output);

    fputs ("\nUntouched text\n\n", output);
}


void returnStrings (char* str_in, int numSymbol)
{
    assert (isfinite (numSymbol));
    assert (str_in);

    int num = 0;

    for (num = 0; num < numSymbol; num++)
        if (str_in[num] == '\0')
            str_in[num] = '\n';

    str_in[num] = '\n';
}


//-----------------------------------------------------------------------------


int lineCtor(line *arr, int nElem) {
    ASSERT(arr == NULL, "Void ptr on line");

    for (int num = 0; num < nElem; num++) {
        arr[num].refonBegin = NULL;
        arr[num].lenStr = 0;
    }

    return 0;
}

int lineDtor(line *arr, int nElem) {
    ASSERT(arr == NULL, "Void ptr on list");
    ASSERT(arr[1].lenStr == -1, "Repeated lineDtor");

    for (int num = 0; num < nElem; num++) {
        arr[num].refonBegin = NULL;
        arr[num].lenStr = -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------


void cleanBuffer() {
    while(getchar() != '\n') {};
}
