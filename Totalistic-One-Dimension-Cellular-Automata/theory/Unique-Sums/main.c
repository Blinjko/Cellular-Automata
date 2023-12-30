/* Program that calculates the unique sums for a 3 digit number of
 * base k. Can be applied to any digit number really but I only need
 * a 3 digit number at the time of making this */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Combo
{
    int number[3];
    int sum;
} Combo;


void PrintCombo(Combo combo) {
    printf("Number: %d %d %d  Sum: %d\n", combo.number[2], combo.number[1], combo.number[0], combo.sum);
}

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

int Sum(int* numbers, int len) {
    int sum = 0;

    for (int i=0; i<len; i++)
    {
        sum+=numbers[i];
    }

    return sum;
}

int Contains(Combo* list, int len, int value) {

    for (int i=0; i<len; i++)
    {
        if (list[i].sum == value)
        {
            return 1;
        }
    }
    return 0;
}


int main(int argc, char** argv) {
    int number[3];
    number[0] = 0;
    number[1] = 0;
    number[2] = 0;

    int k = atoi(argv[1]);

    if (k < 2)
    {
        printf("invalid k value\n");
        return -1;
    }

    // the total number of unique sums cannot exceed (k-1) + (k-1) + (k-1) when using whole numbers
    // this is because 3(k-1) is the largest possible sum and making a list this size counts for the 
    // possibility that every sum is unique.

    int comboIndex = 1;
    const int totalCombos = 3 * (k-1);
    Combo* uniqueCombos = malloc(sizeof(Combo) * totalCombos);

    if (uniqueCombos == NULL)
    {
        printf("Failed to allocate memory\n");
        return -1;
    }

    memset(uniqueCombos, 0, sizeof(Combo) * totalCombos);

    for (int i=0; i<k*k*k; i++)
    {
        Combo currentCombo;
        currentCombo.number[0] = number[0];
        currentCombo.number[1] = number[1];
        currentCombo.number[2] = number[2];
        currentCombo.sum = Sum(number, 3);

        if (!Contains(uniqueCombos, totalCombos, currentCombo.sum))
        {
            uniqueCombos[comboIndex++] = currentCombo;
        }
        printf("Iteration: %d  ", i);
        PrintCombo(currentCombo);
        Increment(number, 3, k);
    }

    printf("Unique Combos\n");
    for (int i=0; i<comboIndex; i++)
    {
        PrintCombo(uniqueCombos[i]);
    }
    printf("%d unique combos\n", comboIndex);
    return 0;
}
