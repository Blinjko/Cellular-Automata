#include "../include/rules.h"


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
