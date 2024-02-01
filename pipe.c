#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	// no argument
	if (argc == 1) {
		errno = EINVAL;
		fprintf(stderr, "usage: ./pipe CMD...\n");
		exit(errno);
	};

	// loop through provided arguments
	for (int i = 1; i < argc; i++) {
		
		char* prog = argv[i];

		// create pipe before forking
		int pipe_fd[2];
		pipe(pipe_fd);

		int pid = fork();
		if (pid < 0) {
			fprintf(stderr, "fatal: fork failed\n");
			exit(errno);
		}
		// child process
		else if (pid == 0) {
			
			if (i != argc-1) {
				dup2(pipe_fd[1], 1);
			}

			close(pipe_fd[0]);
			close(pipe_fd[1]);

			execlp(prog, prog);

			return errno;
		}
		// parent process
		else {
			// connect parent's stdin to pipe's read-end
			dup2(pipe_fd[0], 0);

			// close pipe
			close(pipe_fd[0]);
			close(pipe_fd[1]);

			// suspend execution until child process finishes
			int status;
			waitpid(pid, &status, 0);

			int exit_code = WEXITSTATUS(status);
			if (WIFEXITED(status) && exit_code) {
				fprintf(stderr, "pipe: could not execute %s\n", prog);
				exit(exit_code);
			}
		}
	}
	return 0;
}
