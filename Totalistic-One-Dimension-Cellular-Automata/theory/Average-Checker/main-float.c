/* Simple program to add up all the numbers in a 3 digit sequence 
 * with a base of k, and count the amount of unique averages */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Increment(int* number, int len, int k) {
   if (number[0] == (k-1))
   {
       number[0] = 0;
       if (len > 1)
       {
           Increment(&number[1], len-1, k);
       }
   } 
   else
   {
       number[0] = number[0] + 1;
   }
}

float Average(int* numbers, int len) {
    float sum = 0;

    for (int i=0; i<len; i++)
    {
        sum += numbers[i];
    }

    return sum / len;
}

int Contains(float *list, int len, float value) {

    for (int i=0; i<len; i++)
    {
        if (list[i] == value)
        {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {

    int number[3];
    number[0] = 0.0;
    number[1] = 0.0;
    number[2] = 0.0;

    int k = atoi(argv[1]);

    float uniqueAverages[1000];
    int averageIndex = 1; // the fist one (0) is reserved for 0
    memset(uniqueAverages, 0, sizeof(int) * 1000);


    for (int i=0; i<(k*k*k); i++)
    {
        float average = Average(number, 3);
        if (!Contains(uniqueAverages, 1000, average))
        {
            uniqueAverages[averageIndex++] = average;
        }

        printf("Iteration: %d  Combination: %d %d %d  Average: %f\n", i, number[2], number[1], number[0], average);
        Increment(number, 3, k);
    }

    printf("Averages: ");
    for (int i=0; i<averageIndex; i++)
    {
        printf("%f, ", uniqueAverages[i]);
    }

    printf("Total Averages: %d\n", averageIndex);
    return 0;
}
