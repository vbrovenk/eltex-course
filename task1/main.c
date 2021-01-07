#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

char	**ft_strsplit(char const *s, char c);

void clear_tokens(char **str_tokens) {
	int i = 0;

	if (str_tokens)
		return;
	while (str_tokens[i]) {
		free(str_tokens[i]);
		i++;
	}
	free(str_tokens);
}

void start_bash(void) {
	char string[1000];
	int ret;
	char **str_tokens = NULL;
	char path[200];
	pid_t pid;

	while (1) {
		memset(string, '\0', 1000);
		fgets(string, 999, stdin);
		string[strlen(string) - 1] = '\0';
		if (strlen(string) == 0)
			continue ;

		str_tokens = ft_strsplit(string, ' ');
		if (strcmp(str_tokens[0], "exit") == 0)
			return ;

		memset(path, '\0', 200);
		strcat(path, "/bin/");
		strcat(path, str_tokens[0]);

		if ((pid = fork()) == -1)
			perror("fork error");
		else if (pid == 0) {
			ret = execve(path, str_tokens, NULL);
			if (ret == -1)
				perror("execv: ");
			exit(0);
		}
		else {
			wait(NULL);
			clear_tokens(str_tokens);
		}
	}
}

int main(int argc, char const *argv[]) {
	printf("Usage: cmd params \n\texit - to quit from program.\n");

	start_bash();
	
	return 0;
}