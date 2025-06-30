/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:43:13 by azubieta          #+#    #+#             */
/*   Updated: 2025/06/30 22:19:48 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

void	ft_init(t_pipex *pipex, int argc, char *argv[])
{
	pipex->count = 0;
	pipex->status = 0;
	if (argv[1] && ft_strcmp(argv[1], "here_doc"))
		pipex->n = argc - 4;
	else
		pipex->n = argc - 3;
	pipex->pipes = (int **)malloc((pipex->n - 1) * sizeof(int *));
	if (!pipex->pipes)
		(ft_perror("Malloc failed: pipes\n"), ft_free_pipex(&pipex),
			exit(EXIT_FAILURE));
	while (pipex->count < pipex->n - 1)
	{
		pipex->pipes[pipex->count] = (int *)malloc(2 * sizeof(int));
		if (!pipex->pipes[pipex->count]
			|| pipe(pipex->pipes[pipex->count]) == -1)
			(ft_perror("Pipe error: pipes[i]\n"), ft_free_pipex(&pipex),
				exit(EXIT_FAILURE));
		(pipex->count)++;
	}
	pipex->pids = (pid_t *)malloc((pipex->n) * sizeof(pid_t));
	if (!pipex->pids)
		(ft_perror("Malloc failed: pids\n"), ft_free_pipex(&pipex),
			exit(EXIT_FAILURE));
	pipex->clean_paths = NULL;
	pipex->commands = NULL;
}

void	ft_close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
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

void	ft_perror(const char *str)
{
	write(2, str, ft_strlen(str));
}

void	ft_errno(char *argument)
{
	if (errno == ENOENT)
		(ft_perror("pipex: "), ft_perror(argument),
			ft_perror(": No such file or directory\n"));
	else if (errno == EACCES)
		(ft_perror("pipex: "), ft_perror(argument),
			ft_perror(": Permission denied\n"));
	else if (errno == ENOMEM)
		ft_perror("pipex: Out of memory\n");
	else if (errno == EINVAL)
		ft_perror("pipex: Invalid argument\n");
	else if (errno == EISDIR)
		ft_perror("pipex: Is a directory\n");
	else if (errno == EMFILE)
		ft_perror("pipex: Too many open files\n");
	else if (errno == ENFILE)
		ft_perror("pipex: Too many open files in system\n");
	else if (errno == EFAULT)
		ft_perror("pipex: Bad address\n");
	else if (errno == EROFS)
		ft_perror("pipex: Read-only file system\n");
	else if (errno == ETXTBSY)
		ft_perror("pipex: Text file busy\n");
	else
		ft_perror("pipex: Open failed\n");
}

void	ft_free_pipex(t_pipex **pipex)
{
	int	i;

	if (!(*pipex))
		return ;
	i = 0;
	if ((*pipex)->pipes)
	{
		while (i < (*pipex)->n - 1 && (*pipex)->pipes[i])
		{
			close((*pipex)->pipes[i][READ]);
			close((*pipex)->pipes[i][WRITE]);
			free((*pipex)->pipes[i]);
			i++;
		}
		free((*pipex)->pipes);
	}
	if ((*pipex)->clean_paths)
		ft_freedouble((*pipex)->clean_paths);
	if ((*pipex)->commands)
		ft_freedouble((*pipex)->commands);
	if ((*pipex)->pids)
		free((*pipex)->pids);
	free((*pipex));
}
