#include "../include/error.h"
#include <stdio.h>

void LogError(Error error, FILE* outputStream) {
    if (error.errorMessage != NULL) {
        fprintf(outputStream, error.errorMessage);
    }
}

Error GenerateError(const char* errorMessage) {
    Error error;
    error.errorMessage = errorMessage;

    return error;
}
