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
        sum += (float) numbers[i];
    }

    return sum / (float) len;
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

int Exponent(int base, int power) {
    if (power == 0)
    {
        return 1;
    }
    else if (power == 1)
    {
        return base;
    }
    else
    {
        return base * Exponent(base, power - 1);
    }
}

int main(int argc, char** argv) {

    int number[1000];
    memset(number, 0, sizeof(int) * 1000);
    number[0] = 1;

    int k = atoi(argv[1]);
    int digits = atoi(argv[2]);

    if (k < 2 || digits < 1 || digits > 1000)
    {
        printf("Invalid Parametsr\n");
        return -1;
    }

    float uniqueAverages[10000];
    int averageIndex = 1; // the fist one (0) is reserved for 0
    memset(uniqueAverages, 0, sizeof(float) * 10000);


    const int maxSums = Exponent(k, digits) - 1;
    for (int i=0; i<maxSums; i++)
    {
        float average = Average(number, digits);
        if (!Contains(uniqueAverages, 10000, average))
        {
            uniqueAverages[averageIndex++] = average;
        }

        printf("Iteration: %d  Combination: ", i);
        for (int x=digits-1; x > -1; x--)
        {
            printf("%d ", number[x]);
        }

        printf(" Average: %f\n", average);

        Increment(number, digits, k);
    }

    printf("Averages: ");
    for (int i=0; i<averageIndex; i++)
    {
        printf("%f, ", uniqueAverages[i]);
    }

    printf("Total Averages: %d\n", averageIndex);
    return 0;
}
