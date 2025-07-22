#include <unistd.h>
#include <stdlib.h>

static void	child(int *fd, const char *file, char *const argv[], char type)
{
	if (type == 'r')
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	execvp(file, argv);
	exit(127);
}

static int	parent(int *fd, char type)
{
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	else
	{
		close(fd[0]);
		return (fd[1]);
	}
}

int	ft_popen(const char *file, char *const argv[], char type)
{
	int		fd[2];
	pid_t	pid;

	if (!file || !argv || (type != 'r' && type != 'w') || pipe(fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (pid == 0)
		child(fd, file, argv, type);
	return (parent(fd, type));
}

/*
#include <stdio.h>
#include <sys/wait.h>

int main()
{
    int 	fd;
    ssize_t bytes;
    char 	buffer[1024];
	char	*args1[] = {"ls", NULL};
	char	*args2[] = {"cat", NULL};

	// Test mode 'r'
    printf("=== Test mode 'r' avec ls ===\n");
    fd = ft_popen("ls", args1, 'r');
    if (fd != -1)
    {
        while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0)
        {
  			buffer[bytes] = '\0';
            printf("%s", buffer);
        }
        close(fd);
        wait(NULL);
    }
    else
        printf("Erreur ft_popen\n");
	//test mode 'w'
    printf("\n=== Test mode 'w' avec cat ===\n");
    fd = ft_popen("cat", args2, 'w');
    if (fd != -1)
    {
        write(fd, "Hello World!\n", 13);
        write(fd, "Test réussi!\n", 14);
        close(fd);
        wait(NULL);
    }
    else
        printf("Erreur ft_popen\n");
    return (0);
}
*/
