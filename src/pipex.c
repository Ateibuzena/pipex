/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:55:10 by azubieta          #+#    #+#             */
/*   Updated: 2025/06/30 21:56:16 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

static void	ft_redirection(int argc, char **argv, t_pipex *pipex, char **env)
{
	int	outfile;
	int	infile;

	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0 || infile < 0)
		(ft_perror("Open failed: outfile\n"),
			ft_free_pipex(&pipex), exit(EXIT_FAILURE));
	ft_child_process(infile, outfile);
	ft_execute_cmd(pipex, argv[2], env, NULL);
}

static void	ft_check_args(int argc, t_pipex *pipex)
{
	if (argc < MIN_ARGS || argc > 5)
		(write(2, "./pipex file1 'command1' 'command2' file2\n", 43),
			ft_free_pipex(&pipex), exit(EXIT_FAILURE));
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;
	int		status;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return (ft_perror("Malloc filed: pipex\n"), 1);
	ft_memset(pipex, 0, sizeof(t_pipex));
	ft_check_args(argc, pipex);
	pipex->i = 1;
	if (argc >= 5)
	{
		ft_init(pipex, argc, argv);
		ft_first_process(argv, pipex, env);
		pipex->i = ft_middle_process(argv, pipex, env);
		ft_last_process(argc, argv, pipex, env);
	}
	else if (argc == 4)
		ft_redirection(argc, argv, pipex, env);
	ft_waitpid(pipex);
	status = pipex->status;
	ft_free_pipex(&pipex);
	return (status);
}
