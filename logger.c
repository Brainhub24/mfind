#include "logger.h"
#include <time.h>

void logToFile(const char *message, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file) {
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm);
        fprintf(file, "[%s] %s\n", timestamp, message);
        fclose(file);
    }
}
