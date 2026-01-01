#ifndef TASKS
#define TASKS

/**
 * @brief A kernel task to sendout DIS packets
 *
 * @param param Broadcast MAC adderess
 */
void discoveryTask(void *param);

/**
 * @brief A kernel task to send DAT packets (used for debugging)
 *
 * @param param Destination MAC adderess
 */
void dataTask(void *param);

/**
 * @brief A kernel task to perform automatic re-sending of packets
 *
 */
void retryTask(void *param);

/**
 * @brief A kernel task to execute range-reliability testing
 *
 */
void rrTestTask(void *param);

/**
 * @brief A kernel task to trigger range-reliability testing
 *
 */
void serialCommandTask(void *param);


#endif // !TASKS
