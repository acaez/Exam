#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

static int	fork_error(int prev, int *fd, int next)
{
	if (prev != -1)
		close(prev);
	if (next)
	{
		close(fd[0]);
		close(fd[1]);
	}
	return (1);
}

static void	child(int prev, int *fd, int next)
{
	if (prev != -1)
	{
		dup2(prev, STDIN_FILENO);
		close(prev);
	}
	if (next)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

static int	pipe_and_fork(char *cmd[], int i, int *fd, int *prev)
{
	pid_t	pid;

	if (cmd[i + 1] && pipe(fd) == -1)
	{
		if (*prev != -1)
			close(*prev);
		return (-1);
	}
	pid = fork();
	if (pid == -1)
		return (fork_error(*prev, fd, cmd[i + 1] != NULL));
	if (pid == 0)
	{
		child(*prev, fd, cmd[i + 1] != NULL);
		execvp(&cmd[i][0], &cmd[i]);
		exit(127);
	}
	return (0);
}

static void	cleanup(int *prev, int *fd, int next)
{
	if (*prev != -1)
		close(*prev);
	if (next)
	{
		close(fd[1]);
		*prev = fd[0];
	}
}

int	picoshell(char *cmd[])
{
	int	i;
	int	fd[2];
	int	prev;
	int	result;

	if (!cmd || !cmd[0])
		return (1);
	i = 0;
	prev = -1;
	while (cmd[i])
	{
		result = pipe_and_fork(cmd, i, fd, &prev);
		if (result == -1)
			return (1);
		if (result == 1)
			return (1);
		cleanup(&prev, fd, cmd[i + 1] != NULL);
		i++;
	}
	while (wait(NULL) > 0)
		;
	return (0);
}
/*
#include <stdio.h>

int main()
{
    // Test 1: Une seule commande
    char *ls[] = {"/bin/ls", NULL};
    char *test1[] = { (char *)ls, NULL };
    printf("Test 1: Une seule commande\n");
    int result = picoshell(test1);
    printf("Result: %d\n\n", result);

    // Test 2: Deux commandes avec un pipeline
    char *grep[] = {"/usr/bin/grep", "picoshell", NULL};
    char *test2[] = { (char *)ls, "|", (char *)grep, NULL };
    printf("Test 2: Deux commandes avec un pipeline\n");
    result = picoshell(test2);
    printf("Result: %d\n\n", result);

    // Test 3: Trois commandes avec deux pipelines
    char *echo[] = {"echo", "squalala", NULL};
    char *cat[] = {"cat", NULL};
    char *sed[] = {"sed", "s/a/b/g", NULL};
    char *test3[] = { (char *)echo, "|", (char *)cat, "|", (char *)sed, NULL };
    printf("Test 3: Trois commandes avec deux pipelines\n");
    result = picoshell(test3);
    printf("Result: %d\n\n", result);

    // Test 4: Commande inexistante
    char *nonexistent[] = {"/bin/nonexistent_command", NULL};
    char *test4[] = { (char *)nonexistent, NULL };
    printf("Test 4: Commande inexistante\n");
    result = picoshell(test4);
    printf("Result: %d\n\n", result);

    // Test 5: Commande qui échoue
    char *fail[] = {"/bin/ls", "/nonexistent_directory", NULL};
    char *test5[] = { (char *)fail, NULL };
    printf("Test 5: Commande qui échoue\n");
    result = picoshell(test5);
    printf("Result: %d\n\n", result);

    // Test 6: Pipeline contenant une commande inexistante
    char *test6[] = { (char *)ls, "|", (char *)nonexistent, NULL };
    printf("Test 6: Pipeline contenant une commande inexistante\n");
    result = picoshell(test6);
    printf("Result: %d\n\n", result);

    // Test 7: Pipeline contenant une commande qui échoue
    char *test7[] = { (char *)ls, "|", (char *)fail, NULL };
    printf("Test 7: Pipeline contenant une commande qui échoue\n");
    result = picoshell(test7);
    printf("Result: %d\n\n", result);

    // Test 8: Pipeline vide
    char *test8[] = { NULL };
    printf("Test 8: Pipeline vide\n");
    result = picoshell(test8);
    printf("Result: %d\n", result);
	return (0);
}
*/