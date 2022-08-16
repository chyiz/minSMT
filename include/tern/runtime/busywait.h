#ifndef QI_BUSYWAIT_H
#define QI_BUSYWAIT_H

#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

void busy_wait (unsigned int wait_usec);
struct timeval timeval_normalize(struct timeval tv);

#ifdef __cplusplus
}
#endif

#endif /* Qi_BUSYWAIT_H */