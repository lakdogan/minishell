#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/resource.h>

int main(void)
{
    pid_t pid;
    int status;
    struct rusage usage;

    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child process: PID = %d\n", getpid());
        sleep(2);  // simulate some work
        exit(42);
    }
    else
    {
        // Parent process
        printf("Parent process: waiting for child...\n");
        pid_t ret = wait3(&status, 0, &usage);
        if (ret == -1)
        {
            perror("wait3 failed");
            return 1;
        }

        printf("Child exited with status: %d\n", WEXITSTATUS(status));
        printf(" Resource usage:\n");
        printf("  User CPU time      : %ld.%06d sec\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        printf("  System CPU time    : %ld.%06d sec\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
        printf("  Max resident set   : %ld KB\n", usage.ru_maxrss);
        printf("  Shared memory size : %ld\n", usage.ru_ixrss);
        printf("  Unshared data size : %ld\n", usage.ru_idrss);
        printf("  Unshared stack size: %ld\n", usage.ru_isrss);
        printf("  Page reclaims (soft page faults): %ld\n", usage.ru_minflt);
        printf("  Page faults (hard) : %ld\n", usage.ru_majflt);
        printf("  Swaps              : %ld\n", usage.ru_nswap);
        printf("  Block input ops    : %ld\n", usage.ru_inblock);
        printf("  Block output ops   : %ld\n", usage.ru_oublock);
        printf("  Voluntary context switches  : %ld\n", usage.ru_nvcsw);
        printf("  Involuntary context switches: %ld\n", usage.ru_nivcsw);
    }
    return 0;
}
