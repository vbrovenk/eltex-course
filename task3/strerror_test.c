#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

char   *strerror(int err);
void    errExitEN(int errnum, const char *format, ...);

static void *threadFunc(void *arg) {
    char *str;

    printf("Other thread about to call strerror()\n");
    str = strerror(EPERM);
    printf("Other thread: str (%p) = %s\n", str, str);

    return (NULL);
}

int         main(int argc, char *argv) {
      pthread_t thr1;
      int s;
      char *str;

      str = strerror(EINVAL);
      printf("Main thread has called strerror()\n");

      s = pthread_create(&thr1, NULL, threadFunc, NULL);
      if (s != 0) {
        errExitEN(s, "pthread_create");
      }

      s = pthread_join(thr1, NULL);
      if (s != 0) {
        errExitEN(s, "pthread_join");
      }

      printf("Main thread: str(%p) = %s\n", str, str);

    return (0);
}