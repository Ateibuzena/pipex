#include "../pipexft.h"

void	ft_waitpid(t_pipex *pipex)
{
	// Esperar a los hijos
	int	i;

	i = 0;
    while (i < (pipex->n - 1))
	{
        waitpid(pipex->pids[i], NULL, 0);
		i++;
	}
}

static void	ft_redirection(int argc, char **argv, t_pipex *pipex, char **env)
{
	int	outfile;
	int	infile;

	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0 || infile < 0)
		ft_perror("Open failed: outfile");
	ft_child_process(infile, outfile);
	fprintf(stderr, "unique_comand: %s\n", argv[argc - 2]);
	ft_execute_cmd(pipex, argv[2], env, NULL);
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;
	int		i;

	//fprintf(stderr, "Chequeo argumentos\n");
	ft_check_pipex(argc);

	//fprintf(stderr, "Inicializacion de estructura\n");
	pipex = malloc(sizeof(t_pipex));
	if(!pipex)
		ft_perror("Malloc filed: pipex\n");
	if (argc > 4)
	{
		ft_init(pipex, argc);

		//fprintf(stderr, "Primer proceso hijo\n");
		ft_first_process(argv, pipex, env);
		
		//fprintf(stderr, "Procesos intermedios\n");
		i = ft_middle_process(argv, pipex, env);

		//fprintf(stderr, "Ultimo proceso\n");
		ft_last_process(i, argc, argv, pipex, env);
	}
	else if (argc == 4)
		ft_redirection(argc, argv, pipex, env);
	// Esperar a los hijos
	ft_waitpid(pipex);
	return (0);
}

