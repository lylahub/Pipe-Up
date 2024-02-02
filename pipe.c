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
		exit(errno);
	};

	// loop through provided arguments
	for (int i = 1; i < argc; i++) {
		
		// pipe before forking
		int pipe_fd[2];
		pipe(pipe_fd);

		int pid = fork();
		// parent
		if (pid > 0) {
			// parent's stdin -> pipe's read-end
			dup2(pipe_fd[0], 0);

			// close pipe
			close(pipe_fd[0]);
			close(pipe_fd[1]);

			int status;
			waitpid(pid, &status, 0);

			int exit_code = WEXITSTATUS(status);

			if (WIFEXITED(status) && exit_code) {
				// fail
				exit(exit_code);
			}
		}
		// child
		else if (pid == 0) {
			
			if (i != argc-1) {
				dup2(pipe_fd[1], 1);
			}

			close(pipe_fd[0]);
			close(pipe_fd[1]);

			execlp(argv[i], argv[i]);

			return errno;
		}
		// fail to create a fork
		else {
			exit(errno);
		}
	}
	return 0;
}
