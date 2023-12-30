#ifndef RULES_H
#define RULES_H

#include "error.h"

// A rule is in essence a 8 bit number
// Each bit place corresponds to 1 of the 8
// possible cell configurations and its value
// determines value the central cell will evaluate to.
typedef unsigned char Rule;

// Given a rule and the left middle and right cell values, return
// the value of the middle cell per the given rule.
int ApplyRule(Rule rule, int left, int middle, int right);


typedef struct TotalisticRule
{
    int k;              // k = amount of different states a cell can have

    int uniqueAverages; // The amount of unique averages that exist for 3 cells each having a number between 0 and (k - 1)
                        // also serves as the length for the following arrays
                        
    float* averages;    // List of the actual averages as float values
                        
    int* rule;          // List that maps the resulting cell value / state corresponding to the average of the same index
                        // in the previous array. Hence why it is called the rule.
} TotalisticRule;

// Allocates the arrays, calculates the unique averages, and creates the rule for the given code
Error CalculateTotalisticRule(TotalisticRule* tr, int k, int code);

// Frees any allocated memory
void DestroyTotalisticRule(TotalisticRule* tr);

// Using the given cells and the rule, returns the next cell state
int ApplyTotalisticRule(TotalisticRule* tr, int left, int middle, int right);
#endif
