/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:43:13 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/22 22:38:25 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

int	ft_here_doc(char *delimiter)
{
	char	*line;
	int		temp_pipe[2];

	if (pipe(temp_pipe) < 0)
		ft_perror("Pipe error");
	while (1)
	{
		write(1, ">", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strncmp(line, delimiter, 1) == 0)
		{
			free(line);
			break ;
		}
		write(temp_pipe[WRITE], line, ft_strlen(line));
		free(line);
	}
	close(temp_pipe[WRITE]);
	return (temp_pipe[READ]);
}

void	ft_init(t_pipex *pipex, int argc)
{
	int	i;

	pipex->n = argc - 3;
	pipex->pipes = (int **)malloc((pipex->n) * sizeof(int *));
	if (!pipex->pipes)
		(free(pipex), ft_perror("Malloc failed: pipes"));
	i = 0;
	while (i < pipex->n)
	{
		pipex->pipes[i] = (int *)malloc(2 * sizeof(int));
		if (!pipex->pipes[i] || pipe(pipex->pipes[i]) == -1)
			(ft_freedouble_array(pipex->pipes, i),
				free(pipex), ft_perror("Pipe error: pipes[i]"));
		i++;
	}
	pipex->pids = (pid_t *)malloc((pipex->n) * sizeof(pid_t));
	if (!pipex->pids)
		(ft_freedouble_array(pipex->pipes, i),
			free(pipex), ft_perror("Malloc failed: pids"));
}

void	ft_free_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	if (pipex->commands)
		ft_freedouble(pipex->commands);
	if (pipex->clean_paths)
		ft_freedouble(pipex->clean_paths);
	if (pipex->pids)
		free(pipex->pids);
	if (pipex->pipes)
	{
		while (i < pipex->n)
		{
			free(pipex->pipes[i]);
			i++;
		}
		free(pipex->pipes);
	}
	free(pipex);
}

void	ft_perror(const char *str)
{
	while (str && *str)
		write(2, str++, 1);
	write(2, "\n", 1);
	exit(1);
}

void	ft_not_found(char *str, t_pipex *pipex)
{
	write(2, "Command not found:  ", 19);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	(ft_free_pipex(pipex), ft_perror("Path failed: comands"));
}
