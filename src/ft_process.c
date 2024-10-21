#include "../pipexft.h"

void ft_child_process(int input_fd, int output_fd)
{
	if (input_fd != 0) 
	{  // Redirigir stdin si es necesario
		if (dup2(input_fd, STDIN_FILENO) == -1)
			ft_perror("Dup2 failed: input\n");
		close(input_fd);
	}
	if (output_fd != 1) 
	{  // Redirigir stdout si es necesario
		if (dup2(output_fd, STDOUT_FILENO) == -1)
			ft_perror("Dup2 failed: output\n");
		close(output_fd);
	}
}

void	ft_first_process(char **argv, t_pipex *pipex, char **env)
{
	int	infile;

	infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		ft_perror("Open failed: infile");
	pipex->pids[0] = fork();
	if (pipex->pids[0] < 0)
		ft_perror("Fork error: first_process\n");
	if (pipex->pids[0] == 0)
	{
		close(pipex->pipes[0][READ]);  // No necesitamos el extremo de lectura
		ft_child_process(infile, pipex->pipes[0][WRITE]);
		fprintf(stderr, "first_comand: %s\n", argv[2]);
		ft_execute_cmd(pipex, argv[2], env, NULL);
	}
	close(infile);  // El padre cierra infile
	close(pipex->pipes[0][WRITE]);  // Cierra el extremo de escritura del primer pipe
}

int	ft_middle_process(char **argv, t_pipex *pipex, char **env)
{
	int	i;

	i = 1;
	while (i < (pipex->n - 1))
	{
		pipex->pids[i] = fork();
		if (pipex->pids[i] < 0)
			ft_perror("Fork error: middle_process\n");
		if (pipex->pids[i] == 0)
		{
			close(pipex->pipes[i][READ]);
			ft_child_process(pipex->pipes[i - 1][READ], pipex->pipes[i][WRITE]);
			fprintf(stderr, "middle_comand: %s\n", argv[i + 2]);
			ft_execute_cmd(pipex, argv[i + 2], env, NULL);
		}
		close(pipex->pipes[i - 1][READ]);  // El padre cierra los extremos usados
		close(pipex->pipes[i][WRITE]);
		i++;
	}
	return (i);
}

void	ft_last_process(int	i, int argc, char **argv, t_pipex *pipex, char **env)
{
	int	outfile;

	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (outfile < 0)
		ft_perror("Open failed: outfile");
	ft_child_process(pipex->pipes[i - 1][READ], outfile);
	fprintf(stderr, "last_comand: %s\n", argv[argc - 2]);
	ft_execute_cmd(pipex, argv[argc - 2], env, NULL);
}