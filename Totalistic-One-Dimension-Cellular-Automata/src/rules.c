#include "../include/rules.h"
#include "../include/error.h"

#include <stdlib.h>
#include <string.h>

// Determines cell configuration and returns a 1 in the corresponding
// place. EX configuration 0 will put a 1 in the 0'th digit, thus
// returning 00000001
unsigned char DetermineConfiguration(int left, int middle, int right) {
    unsigned char bit = 0;

    if (left == 0 && middle == 0 && right == 0)
    {
        bit = 1;
    }
    else if (left == 0 && middle == 0 && right == 1)
    {
        bit = 2;
    }
    else if (left == 0 && middle == 1 && right == 0)
    {
        bit = 4;
    }
    else if (left == 0 && middle == 1 && right == 1)
    {
        bit = 8;
    }
    else if (left == 1 && middle == 0 && right == 0)
    {
        bit = 16;
    }
    else if (left == 1 && middle == 0 && right == 1)
    {
        bit = 32;
    }
    else if (left == 1 && middle == 1 && right == 0)
    {
        bit = 64;
    }
    else if (left == 1 && middle == 1 && right == 1)
    {
        bit = 128;
    }

    return bit;
}

// Given a rule and the left middle and right cell values, return
// the value of the middle cell per the given rule.
int ApplyRule(Rule rule, int left, int middle, int right) {
    return DetermineConfiguration(left, middle, right) & rule ? 1 : 0;
}

void ReverseArray(int* array, int length) {
    int temp = 0;
    if (length == 2)
    {
        temp = array[0];
        array[0] = array[1];
        array[1] = temp;
        return;
    }
    else if (length == 3)
    {
        temp = array[0];
        array[0] = array[2];
        array[2] = temp;
        return;
    }

    else
    {
        temp = array[0];
        array[0] = array[length - 1];
        array[length - 1] = temp;
        ReverseArray(array + 1, length - 2);
    }
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

// Convert a base 10 value to the supplied base.
// returns the remainder left in value after the conversion is complete
// if successful this will be 0, if it is not then the converted number
// needed more digits.
int ConvertBase(int* number, int length, int base, int value) {

    if (length == 0)
    {
        return value;
    }

    for (int i = (base - 1); i > 0; i--)
    {
        int positionValue = Exponent(base, (length - 1)) * i;

        if (positionValue <= value)
        { 
            number[0] = i;
            return ConvertBase(number + 1, length - 1, base, value - positionValue);
        }
    }

    // Condition where the value is less than any value this position could hold.
    number[0] = 0;
    return ConvertBase(number + 1, length - 1, base, value);
}

// Function that increments the given number of the given base by 1
void IncrementNumber(int* number, int length, int base) {
    
    if (length == 0)
    {
        return;
    }

    else if (number[0] == (base - 1))
    {
        number[0] = 0;
        IncrementNumber(number + 1, length - 1, base);
    }
    else
    {
        number[0]++;
    }

}

// Calculates all the unique averages for a 3 cell configuration each with values 0 to (k - 1)
// k dictates the possible number range, which effectively is the base. i.e k = 2 is binary. k = 8 is octal
void CalculateUniqueAverages(float* averages, int length, int k) {
    averages[0] = 0.0f;
    int averageIndex = 1;
    int cells[3] = {1, 0, 0};

    for (int i = 1; averageIndex < length; i++)
    {
        float average = (float) (cells[0] + cells[1] + cells[2]) / 3.0f; // 3.0 here because there are 3 cells
        
        if (average > averages[averageIndex - 1])
        {
            averages[averageIndex++] = average;
        }

        IncrementNumber(cells, 3, k);
    }
}

// Allocates the arrays, calculates the unique averages, and creates the rule for the given code
Error CalculateTotalisticRule(TotalisticRule* tr, int k, int code) {

    unsigned char* pointer = malloc( (3 * k - 2) * (sizeof(float) + sizeof(int)) );
    if (pointer == NULL)
    {
        return GenerateError("Failed to allocate memory, CalculateTotalisticRule, rules.c\n");
    }

    tr->k = k;
    tr->uniqueAverages = 3 * k - 2;

    tr->averages = (float*) pointer;
    tr->rule = (int*) (pointer + (tr->uniqueAverages * sizeof(float)));
    
    CalculateUniqueAverages(tr->averages, tr->uniqueAverages, tr->k);

    if (ConvertBase(tr->rule, tr->uniqueAverages, tr->k, code) != 0)
    {
        free(pointer);
        memset(tr, 0, sizeof(TotalisticRule));
        return GenerateError("Failed to generate valid rule from supplied code, invalid code. CalculateTotalisticRule, rules.c\n");

    }

    ReverseArray(tr->rule, tr->uniqueAverages);

    return GenerateError(NULL);
}

// Frees any allocated memory
void DestroyTotalisticRule(TotalisticRule* tr) {
    free(tr->averages);
    memset(tr, 0, sizeof(TotalisticRule));
}

// Using the given cells and the rule, returns the next cell state
int ApplyTotalisticRule(TotalisticRule* tr, int left, int middle, int right) {

    float average = (float) (left + middle + right) / 3.0f;

    for (int i = 0; i < tr->uniqueAverages; i++)
    {
        if (tr->averages[i] == average)
        {
            return tr->rule[i];
        }
    }

    // Should never happen if everything is as it should be
    return -1;
}
