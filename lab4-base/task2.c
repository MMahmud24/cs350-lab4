#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        printf("IN CHILD: pid=%d\n", getpid());
        execvp(argv[1], &argv[1]);
        perror("execvp");
        _exit(EXIT_FAILURE);
    }

    int status;
    if (waitpid(pid, &status, 0) < 1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }

    printf("In PARENT: successfully waited child (pid=%d)\n", pid);
    return EXIT_SUCCESS;
}
