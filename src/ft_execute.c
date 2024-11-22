/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:32:39 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/22 23:28:30 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

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

char **ft_parse_command(char *argv)
{
    char    **parsed;
    int     i;

    parsed = malloc(sizeof(char *) * 2097152);
    if (!parsed)
        return (NULL);
    i = 0;
    while (*argv)
    {
        while (*argv && ft_isspace(*argv))
            argv++;
        if (*argv == '"') // Maneja comillas dobles
            parsed[i++] = ft_extract_quoted(&argv, '"');
        else if (*argv == '\'') // Maneja comillas simples
            parsed[i++] = ft_extract_quoted(&argv, '\'');
        else
            parsed[i++] = ft_extract_word(&argv);
    }
    parsed[i] = NULL;
    return (parsed);
}


void	ft_execute_cmd(t_pipex *pipex, char *argv, char **env, char *pathname)
{
	if (argv[0] == '/')
	{
		pipex->commands = ft_split(argv, ' ');
		pathname = pipex->commands[0];
	}
	else
	{
		pipex->commands = ft_split(argv, ' ');
		if (!pipex->commands || !pipex->commands[0])
			(ft_free_pipex(pipex), ft_perror("Invalid command"));
		pipex->found_way = ft_search_way("PATH=", env, 5);
		if (!pipex->found_way)
			(ft_free_pipex(pipex), ft_perror("PATH not found in environment"));
		pipex->clean_paths = ft_clean_path(pipex->found_way);
		pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);
		ft_freedouble(pipex->clean_paths);
		if (!pathname)
			ft_not_found(pipex->commands[0], pipex);
	}
	execve(pathname, pipex->commands, env);
	ft_free_pipex(pipex);
	ft_perror("Execution failed");
}
