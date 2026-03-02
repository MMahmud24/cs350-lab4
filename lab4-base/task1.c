#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("IN CHILD: pid=%d\n", getpid());
        execlp("ls", "ls", "-l", "-a", (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    int status;
    if (waitpid(pid, &status, 0) < 1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }

    printf("In PARENT: successfully waited child (pid=%d)\n", pid);
    return 0;
}
