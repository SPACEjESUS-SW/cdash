#include "logger.h"

int init_logger(const char* filename, logger_t* logger) {
    /**
     * @brief Initiates a logger object. Opens file with the provided filename.
     * 
     * @param filename The path of the file.
     * @param logger Instance of struct logger_t.
     * 
     * @return int. -1 on failure, 0 on success.
     */

    // Validate inputs
    if(!filename || !logger) {
        errno = EINVAL;
        return -1;
    }

    // Open file
    logger->m_log_file = fopen(filename, "w");
    if (!logger->m_log_file) { return -1; }

    // Initialize mutex
    int rc = pthread_mutex_init(&logger->m_lock, NULL);
    if (rc != 0) {
        fclose(logger->m_log_file);
        errno = rc;
        return -1;
    }

    return 0;
}

void log_message(int level, const char* message, logger_t* logger) {
    /**
     * @brief Writes the message into the log file safelt
     * 
     * @param message The message to be logged
     */

    // Acquire lock
    pthread_mutex_lock(&logger->m_lock);

    // Get timestep
    time_t now = time(NULL);
    char timebuf[64];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

    char* log_level_msg = NULL;
    if (level == INFO)
        log_level_msg = "    INFO";
    else if (level == DEBUG)
        log_level_msg = "   DEBUG";
    else if (level == WARNING)
        log_level_msg = " WARNING";
    else if (level == CRITICAL)
        log_level_msg = "CRITICAL";
    else {
        perror("Invalid log level");
        exit(EXIT_FAILURE);
    }
    
    fprintf(logger->m_log_file, "%s | ", log_level_msg);
    fprintf(logger->m_log_file, "[%s] | ", timebuf);
    fprintf(logger->m_log_file, "%s", message);
    fprintf(logger->m_log_file, "\n");

    fflush(logger->m_log_file);

    // Give back lock
    pthread_mutex_unlock(&logger->m_lock);

}

int close_logger(logger_t* logger) {
    /**
     * @brief Gracefully removes the instance of this logger from memory
     * 
     * @param logger Instance of the logger struct
     * 
     * @return -1 on failure, 0 on success
     */

    if(!logger) {
        errno = EINVAL;
        return -1;
    }

    int ret = fclose(logger->m_log_file);
    if (ret != 0) {
        perror("Error in closing log file");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_destroy(&logger->m_lock);

    return 0;
}