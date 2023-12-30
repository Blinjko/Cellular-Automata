/* Program that calculates the unique sums for up to a 100 digit number of
 * base k. Can be applied to any digit number really. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Combo
{
    int number[100];
    int digits;
    int sum;
} Combo;


void PrintCombo(Combo* combo) {
    printf("Number: ");
    for(int i = combo->digits-1; i>-1; i--)
    {
        printf("%d ", combo->number[i]);
    }
    printf(" Sum: %d\n", combo->sum);
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
    int number[100];
    memset(number, 0, sizeof(int) * 100);

    int k = atoi(argv[1]);
    int digits = atoi(argv[2]);

    if (k < 2)
    {
        printf("invalid k value\n");
        return -1;
    }

    if (digits < 2 || digits > 100)
    {
        printf("Invalid number of digits\n");
        return -1;
    }


    int comboIndex = 1;
    const int totalCombos = digits * (k-1); // ensures list can hold maximum possible sums
    Combo* uniqueCombos = malloc(sizeof(Combo) * totalCombos);

    if (uniqueCombos == NULL)
    {
        printf("Failed to allocate memory\n");
        return -1;
    }

    number[0] = 1; // start at 1 because 0 is already in the first spot
    memset(uniqueCombos, 0, sizeof(Combo) * totalCombos);

    const int largestValue = Exponent(k, digits);
    for (int i=1; i<largestValue; i++)
    {
        Combo currentCombo;
        for (int x=0; x<digits; x++)
        {
            currentCombo.number[x] = number[x];
        }

        currentCombo.sum = Sum(number, digits);
        currentCombo.digits = digits;

        if (!Contains(uniqueCombos, totalCombos, currentCombo.sum))
        {
            uniqueCombos[comboIndex++] = currentCombo;
        }
        printf("Iteration: %d  ", i);
        PrintCombo(&currentCombo);
        Increment(number, digits, k);
    }

    printf("Unique Combos\n");
    for (int i=0; i<comboIndex; i++)
    {
        PrintCombo(&uniqueCombos[i]);
    }
    printf("%d unique combos\n", comboIndex);
    return 0;
}
