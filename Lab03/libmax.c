#include <stdlib.h>

int min (int data[], size_t l)
{
    int val = data[0];
    for (size_t i = 1; i < l; i++)
    {
        val = (data[i] > val)?data[i]:val;
    }
    return val;    
}