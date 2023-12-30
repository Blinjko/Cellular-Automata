#ifndef RULES_H
#define RULES_H

// A rule is in essence a 8 bit number
// Each bit place corresponds to 1 of the 8
// possible cell configurations and its value
// determines value the central cell will evaluate to.
typedef unsigned char Rule;

// Given a rule and the left middle and right cell values, return
// the value of the middle cell per the given rule.
int ApplyRule(Rule rule, int left, int middle, int right);
#endif
