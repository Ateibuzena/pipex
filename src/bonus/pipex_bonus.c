/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:56:26 by azubieta          #+#    #+#             */
/*   Updated: 2024/10/23 18:38:11 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../pipexft.h"

static void	ft_redirection(int argc, char **argv, t_pipex *pipex, char **env)
{
	int	outfile;
	int	infile;

	infile = open(argv[1], O_RDONLY);
	outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0 || infile < 0)
		ft_perror("Open failed: outfile");
	ft_child_process(infile, outfile);
	ft_execute_cmd(pipex, argv[2], env, NULL);
}

static void	ft_check_args(int argc, t_pipex *pipex)
{
	if (pipex->i)
	{
		if (argc < MIN_ARGS || argc > 5)
		{
			free(pipex);
			ft_perror("./pipex file1 'command1' 'command2' file2");
		}
	}
	else
	{
		if (argc < MIN_ARGS)
		{
			free(pipex);
			ft_perror("./pipex file1 'command1' 'command2'...'commandN' file2");
		}
	}
}

static void	ft_check_comands(int argc, char **argv, t_pipex *pipex, char **env)
{
	char	*pathname;

	pipex->i = 1;
	pipex->count = 0;
	while ((pipex->i)++ < (argc - 2))
	{
		pipex->commands = ft_split(argv[pipex->i], ' ');
		pipex->found_way = ft_search_way("PATH=", env, 4);
		pipex->clean_paths = ft_clean_path(pipex->found_way, 5);
		pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);
		ft_freedouble(pipex->clean_paths);
		if (!pathname)
		{
			pipex->found_way = ft_search_way("PWD=", env, 3);
			pipex->clean_paths = ft_clean_path(pipex->found_way, 4);
			pathname = ft_accessible_path(pipex->clean_paths,
					pipex->commands[0]);
			ft_freedouble(pipex->clean_paths);
			if (!pathname)
				ft_not_found(argv[pipex->i], &(pipex->count));
		}
		(free(pathname), ft_freedouble(pipex->commands));
	}
	if (pipex->count)
		(free(pipex), ft_perror("Path failed: comands"));
}

int	main(int argc, char **argv, char **env)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		ft_perror("Malloc filed: pipex");
	pipex->i = 0;
	ft_check_args(argc, pipex);
	ft_check_comands(argc, argv, pipex, env);
	if (argc > 4)
	{
		ft_init(pipex, argc);
		ft_first_process(argv, pipex, env);
		pipex->i = ft_middle_process(argv, pipex, env);
		ft_last_process(argc, argv, pipex, env);
	}
	else if (argc == 4)
		ft_redirection(argc, argv, pipex, env);
	ft_waitpid(pipex);
	return (0);
}
