#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define INPUT_FILE "./input/if1"
#define OUTPUT_FILE "./result"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork fail");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        printf("IN CHILD: pid=%d\n", getpid());

        int fd_in = open(INPUT_FILE, O_RDONLY);
        if (fd_in < 0) {
            perror("open input");
            _exit(EXIT_FAILURE);
        }

        int fd_out = open(OUTPUT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out < 0) {
            perror("open output");
            close(fd_in);
            _exit(EXIT_FAILURE);
        }

        if (dup2(fd_in, STDIN_FILENO) < 0) {
            perror("dup2 input");
            close(fd_in);
            close(fd_out);
            _exit(EXIT_FAILURE);
        }
        if (dup2(fd_out, STDOUT_FILENO) < 0) {
            perror("dup2 output");
            close(fd_in);
            close(fd_out);
            _exit(EXIT_FAILURE);
        }

        close(fd_in);
        close(fd_out);

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
