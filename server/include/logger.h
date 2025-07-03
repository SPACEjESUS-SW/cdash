#ifndef LOGGER_H
#define LOGGER_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include <time.h>

typedef enum {
    INFO,
    DEBUG,
    WARNING,
    CRITICAL
} log_levels;

typedef struct {
    FILE* m_log_file;
    pthread_mutex_t m_lock;
} logger_t;

int init_logger(const char* filename, logger_t* logger);
void log_message(int level, const char* message, logger_t* logger);
int close_logger(logger_t* logger);

#endif