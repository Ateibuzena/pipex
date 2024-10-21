#include "../pipexft.h"

void	ft_check_pipex(int	argc)
{
	if (argc < MIN_ARGS) 
	{
        fprintf(stderr, "This program must be executed like:\n");
        ft_perror("./pipex file1 command1 command2 ... commandN file2\n");
    }
}

void	ft_init(t_pipex *pipex, int argc)
{
	int		i;

	// Numero de comandos
	pipex->n = argc - 3;
    pipex->pipes = (int **)malloc((pipex->n - 1) * sizeof(int *));
    if (!pipex->pipes) 
		ft_perror("Malloc failed: pipes\n");

	// Crear pipes necesarios(coomandos - 1)
	i = 0;
    while (i < pipex->n)
	{
		// Memoria para los fds de cada pipe (read, write)
        pipex->pipes[i] = (int *)malloc(2 * sizeof(int));
        if (!pipex->pipes[i] || pipe(pipex->pipes[i]) == -1)
			ft_perror("Pipe error: pipes[i]\n");
		i++;
    }
	// Memoria para los n pids (comandos - 1) = numero de forks
	pipex->pids = (pid_t *)malloc((pipex->n - 1) * sizeof(pid_t)); // Un proceso por comando
    if (!pipex->pids) 
		ft_perror("Malloc failed: pids\n");
}

void	ft_perror(const char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

void	ft_exit_error(char *str)
{
	write(2, "Command not found:  ", 19);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(127);
}

void	ft_free_pipes(int **fds, int num_pipes)
{
	while (num_pipes)
	{
		free(fds[num_pipes]);
		num_pipes--;
	}
	free(fds);
}
