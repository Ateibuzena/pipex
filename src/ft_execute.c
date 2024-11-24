/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:32:39 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/24 21:02:22 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

void	ft_close_pipes(t_pipex *pipex)
{
	int i = 0;
	if (pipex->pipes)
	{
		while (i < pipex->n - 1)
		{
			close(pipex->pipes[i][READ]);
			close(pipex->pipes[i][WRITE]);
			i++;
		}
	}
}

char	*ft_search_way(const char *key, char **env, size_t len)
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, len))
			return (env[i] + len);
		i++;
	}
	return (NULL);
}

char	**ft_clean_path(char *path_value)
{
	if (!path_value)
		return (NULL);
	return (ft_split(path_value, ':'));
}

char	*ft_accessible_path(char **paths, char *command)
{
	char	*full_path;
	int		i;

	if (!paths || !command)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin(ft_strjoin(paths[i], "/"), command);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	ft_execute_cmd(t_pipex *pipex, char *argv, char **env, char *pathname)
{
	if (!argv || !argv[0] || argv[0] == ' ')
		(ft_not_found(argv), exit(127));
	if (argv[0] == '/')
	{
		pipex->commands = ft_split(argv, ' ');
		pathname = pipex->commands[0];
		if (access(pathname, X_OK) != 0)
			(ft_free_pipex(pipex), ft_perror("No such file or directory"), exit(127));
	}
	else if (ft_strchr(argv, '/'))
	{
		pipex->commands = ft_split(argv, ' ');
		pathname = pipex->commands[0];

		if (access(pathname, X_OK) != 0)
			(ft_free_pipex(pipex), ft_perror("No such file or directory"), exit(127));
	}
	else
	{
		pipex->commands = ft_split(argv, ' ');
		if (!pipex->commands || !pipex->commands[0])
			(ft_free_pipex(pipex), ft_perror("Invalid command"), exit(127));
		pipex->found_way = ft_search_way("PATH=", env, 5);
		if (!pipex->found_way)
			(ft_free_pipex(pipex), ft_perror("PATH not found in environment"), exit(127));
		pipex->clean_paths = ft_clean_path(pipex->found_way);
		pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);
		//ft_freedouble(pipex->clean_paths);
		if (!pathname) {
			ft_not_found(argv);
			// ft_free_pipex(pipex);
			exit(127);
		}
	}
	ft_close_pipes(pipex);
	if (execve(pathname, pipex->commands, env) == -1)
	{
		if (errno == EACCES)
        	(ft_free_pipex(pipex), ft_perror("Permission denied"), exit(126));
    	else
        	(ft_free_pipex(pipex), ft_not_found(pipex->commands[0]), exit(127));
	}
}
