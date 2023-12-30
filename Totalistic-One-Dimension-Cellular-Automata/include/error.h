#ifndef ERROR_H
#define ERROR_H

/* Basic Error Handling structure and functions */

#include <stdio.h>

typedef struct Error
{
    /* Currently just hold a message,
     * In the future we can have a function pointer stored
     * and an error value, so that the function can be called 
     * with the given value to retrieve the message */
    const char* errorMessage;
} Error;

void LogError(Error error, FILE* outputStream);
Error GenerateError(const char* errorMessage);
#endif
