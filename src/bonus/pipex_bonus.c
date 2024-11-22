/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:56:26 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/22 23:40:08 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../pipexft.h"

static void	ft_check_args(int argc, char **argv, t_pipex *pipex)
{
	if (!argv[1] || ft_strcmp(argv[1], "here_doc") == 0)
	{
		pipex->i = 1;
		if (argc < MIN_ARGS)
		{
			free(pipex);
			ft_perror("./pipex file1 cmd1 cmd2 ... cmdn file2");
		}
	}
	else
	{
		pipex->i = 2;
		if (argc < (MIN_ARGS + 1))
		{
			free(pipex);
			ft_perror("./pipex here_doc DELIMITER cmd1 cmd2 ... cmdn file2");
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_perror("Malloc filed: pipex");
	pipex->i = 0;
	ft_check_args(argc, argv, pipex);
	ft_init(pipex, argc);
	ft_first_process(argv, pipex, env);
	pipex->i = ft_middle_process(argv, pipex, env);
	ft_last_process(argc, argv, pipex, env);
	ft_waitpid(pipex);
	return (0);
}
