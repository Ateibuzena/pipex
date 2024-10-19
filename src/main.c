#include "../pipexft.h"

static	void	ft_perror(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

static void	ft_init(t_pipex *pipex, int argc)
{
	int		i;

	pipex->np = argc - 4;
	if (argc < 5) 
	{
        ft_printf("This program must be executed like:\n");
        ft_printf("./pipex file1 command1 command2 ... commandN file2\n");
        exit(EXIT_FAILURE);
    }
    // memoria para np pipes
    pipex->pipes = (int **)malloc(pipex->np * sizeof(int *));
    if (!pipex->pipes) 
		ft_perror("Malloc failed: pipes\n");
	i = 0;
    while (i < pipex->np) 
	{
		// memoria para los pipes de cada pipe
        pipex->pipes[i] = (int *)malloc(2 * sizeof(int));
        if (!pipex->pipes[i] || pipe(pipex->pipes[i]) == -1)
			ft_perror("Pipe error: pipes[i]\n");
		i++;
    }
	// memoria para los np pids
	pipex->pids = (pid_t *)malloc((pipex->np) * sizeof(pid_t)); // Un proceso por comando
    if (!pipex->pids) 
		ft_perror("Malloc failed: pids\n");
	pipex->i = 0;
}

static void	ft_print_array_double(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->np)
	{
		ft_printf("pipe[%d], fd[0]: %d, fd[1]: %d\n",
				i, (pipex->pipes)[i][0], (pipex->pipes)[i][1]);
		i++;
	}
	ft_printf("procesos llevados a cabo: %d\n", pipex->i);
	ft_printf("procesos totales a realizar %d\n:", pipex->np);
}

static void	ft_first_proces(t_pipex *pipex, char *argv[], char **env)
{
	int	fd_infile;

	ft_printf("hola2\n");
	(pipex->pids)[pipex->i] = fork();
	if ((pipex->pids)[pipex->i] < 0)
		ft_perror("Fork error: first_process\n");
	if ((pipex->pids)[pipex->i] == 0)
	{
		fd_infile = open(argv[(pipex->i + 1)], O_RDONLY);
		if (fd_infile < 0)
			ft_perror("Open failed: first input\n");
		if (dup2(fd_infile, STDIN_FILENO) == -1)
			ft_perror("Dup2 failed: \n");
		ft_printf("hola3\n");
		ft_printf("fds: %d\n", pipex->pipes[pipex->i][1]);
		if (dup2(pipex->pipes[pipex->i][1], STDOUT_FILENO) == -1)
			ft_perror("Dup2 failed: first output\n");
		ft_execute_cmd(pipex, argv[(pipex->i + 2)], env, NULL);
		close(pipex->pipes[pipex->i][0]);
		close((pipex->pipes)[pipex->i][0]);
		pipex->i += 1;
		ft_print_array_double(pipex);
	}
}

static void	ft_middle_proces(t_pipex *pipex, char *argv[], char **env)
{
	(pipex->pids)[pipex->i] = fork();
	if ((pipex->pids)[pipex->i] < 0)
		ft_perror("Fork error: middle_process\n");
	close(pipex->pipes[pipex->i][0]);
	if (dup2(pipex->pipes[(pipex->i - 1)][0], STDIN_FILENO) == -1)
		ft_perror("Dup2 failed: middle input\n");
	if (dup2(pipex->pipes[pipex->i][1], STDOUT_FILENO) == -1)
		ft_perror("Dup2 failed: middle output\n");
	ft_execute_cmd(pipex, argv[(pipex->i + 2)], env, NULL);
	close((pipex->pipes)[pipex->i][1]);
	pipex->i += 1;
	ft_print_array_double(pipex);
}

static void	ft_last_proces(t_pipex *pipex, char *argv[], char **env)
{
	int	fd_outfile;

	(pipex->pids)[pipex->i] = fork();
	if ((pipex->pids)[pipex->i] < 0)
		ft_perror("Fork error: last_process\n");
	if (dup2((pipex->pipes)[(pipex->i - 2)][0], STDIN_FILENO) == -1)
		ft_perror("Dup2 failed: last input\n");
	fd_outfile = open(argv[(pipex->i + 2)], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dup2(fd_outfile, STDOUT_FILENO) == -1)
		ft_perror("Dup2 failed: last output\n");
	ft_execute_cmd(pipex, argv[(pipex->i + 2)], env, NULL);
	close(fd_outfile);
	close((pipex->pipes)[(pipex->i - 2)][0]);
	pipex->i += 1;
	ft_print_array_double(pipex);
}

int main(int argc, char **argv, char **env) 
{
	t_pipex	*pipex;
	
	pipex = malloc(sizeof(t_pipex));
	if(!pipex)
	{
		perror("Malloc filed: pipex\n");
		exit(EXIT_FAILURE);
	}
	ft_init(pipex, argc);
	ft_printf("hola1\n");
	ft_print_array_double(pipex);
	// primer proceso:
	ft_first_proces(pipex, argv, env);
	// procesos intermedios:
	while (pipex->i < pipex->np)
		ft_middle_proces(pipex, argv, env);
	// último proceso:
	ft_last_proces(pipex, argv, env);
	return (0);
}
