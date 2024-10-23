/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:32:39 by azubieta          #+#    #+#             */
/*   Updated: 2024/10/23 18:37:51 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

char	*ft_search_way(char *str, char **env, int len)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(str, env[i], len))
			return (env[i]);
		i++;
	}
	return (env[i]);
}

char	**ft_clean_path(char *path, int start)
{
	char	**cleaned;
	char	*temp;
	int		i;

	i = 0;
	if (!path)
		return (ft_split(" ", ' '));
	cleaned = ft_split(&path[start], ':');
	if (!cleaned)
		return (NULL);
	while (cleaned[i])
	{
		temp = cleaned[i];
		cleaned[i] = ft_strjoin(cleaned[i], "/");
		free(temp);
		if (!cleaned[i])
			return (ft_freedouble(cleaned), NULL);
		i++;
	}
	cleaned[i] = NULL;
	return (cleaned);
}

char	*ft_accessible_path(char **path, char *cmd)
{
	int		i;
	int		j;
	char	*temp;

	if (!cmd)
		return (NULL);
	i = 0;
	while (path[i])
	{
		temp = path[i];
		path[i] = ft_strjoin(path[i], cmd);
		free(temp);
		if (!path[i])
			return (NULL);
		if (access(path[i], X_OK) == 0)
		{
			j = i - 2;
			while (path[j++])
				(free(path[j]), path[j] = NULL);
			return (path[i]);
		}
		(free(path[i]), path[i] = NULL);
		i++;
	}
	return (NULL);
}

void	ft_execute_cmd(t_pipex *pipex, char *argv, char **env, char *pathname)
{
	pipex->commands = ft_split(argv, ' ');
	pipex->found_way = ft_search_way("PATH=", env, 4);
	pipex->clean_paths = ft_clean_path(pipex->found_way, 5);
	pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);
	ft_freedouble(pipex->clean_paths);
	if (!pathname)
	{
		pipex->found_way = ft_search_way("PWD=", env, 3);
		pipex->clean_paths = ft_clean_path(pipex->found_way, 4);
		pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);
		ft_freedouble(pipex->clean_paths);
		if (!pathname)
			ft_free_pipex(pipex);
	}
	execve(pathname, pipex->commands, env);
	ft_free_pipex(pipex);
	ft_perror("Execution failed");
}
