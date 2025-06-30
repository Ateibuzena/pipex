/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:56:26 by azubieta          #+#    #+#             */
/*   Updated: 2025/06/30 21:56:23 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../pipexft.h"

static void	ft_check_args(int argc, char **argv, t_pipex *pipex)
{
	if (!argv[1] || ft_strcmp(argv[1], "here_doc") == 0)
	{
		pipex->i = 1;
		if (argc < MIN_ARGS)
			(write(2, "./pipex file1 cmd1 cmd2 ... cmdn file2\n", 40),
				ft_free_pipex(&pipex), exit(EXIT_FAILURE));
	}
	else
	{
		pipex->i = 2;
		if (argc < (MIN_ARGS + 1))
			(write(2, "./pipex here_doc DELIMITER ", 28),
				write(2, "cmd1 cmd2 ... cmdn file2\n", 26),
				ft_free_pipex(&pipex), exit(EXIT_FAILURE));
	}
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;
	int		status;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return (ft_perror("Malloc filed: pipex\n"), 1);
	ft_memset(pipex, 0, sizeof(t_pipex));
	ft_check_args(argc, argv, pipex);
	ft_init(pipex, argc, argv);
	ft_first_process(argv, pipex, env);
	pipex->i = ft_middle_process(argv, pipex, env);
	ft_last_process(argc, argv, pipex, env);
	ft_waitpid(pipex);
	status = pipex->status;
	ft_free_pipex(&pipex);
	return (status);
}
