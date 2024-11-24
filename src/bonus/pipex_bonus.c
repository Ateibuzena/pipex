/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:56:26 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/24 21:29:37 by azubieta         ###   ########.fr       */
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
			write(2, "./pipex file1 cmd1 cmd2 ... cmdn file2\n", 40);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		pipex->i = 2;
		if (argc < (MIN_ARGS + 1))
		{
			free(pipex);
			write(2, "./pipex here_doc DELIMITER cmd1 cmd2 ... cmdn file2\n", 53);
			exit(EXIT_FAILURE);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		return (perror("Malloc filed: pipex"), 1);
	ft_memset(pipex, 0, sizeof(pipex));
	//pipex->i = 0;
	ft_check_args(argc, argv, pipex);
	ft_init(pipex, argc);
	ft_first_process(argv, pipex, env);
	pipex->i = ft_middle_process(argv, pipex, env);
	ft_last_process(argc, argv, pipex, env);
	ft_waitpid(pipex);
	ft_free_pipex(pipex);
	close(0);
	close(1);
	close(2);
	return (pipex->status);
}
