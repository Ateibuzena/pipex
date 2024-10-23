/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:43:13 by azubieta          #+#    #+#             */
/*   Updated: 2024/10/23 18:03:18 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

void	ft_free_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	free(pipex->pids);
	while (i < pipex->n)
	{
		free(pipex->pipes[i]);
		i++;
	}
	free(pipex->pipes);
	free(pipex);
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

void	ft_perror(const char *str)
{
	while (str && *str)
		write(2, str++, 1);
	write(2, "\n", 1);
	exit(1);
}

void	ft_not_found(char *str, int *count)
{
	write(2, "Command not found:  ", 19);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	(*count) += 1;
}
