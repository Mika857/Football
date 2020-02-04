#include <MathJ/MathJ.h>

int MathJ::MaxValue(int values[],int size)
{
    int maxValue = values[0];
    for (int i = 1; i < size; i++)
    {
        if(values[i] > maxValue)
        {
            maxValue = values[i];
        }
    }
    return maxValue;
    
}

int MathJ::Direction(int values[],int size)
{
    int maxValue = values[0];
    int dir = 0;
    for (int i = 1; i < size; i++)
    {
        if(values[i] > maxValue)
        {
            maxValue = values[i];
            dir = 1;
        }
    }
    return dir;
}