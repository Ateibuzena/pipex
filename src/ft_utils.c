/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:43:13 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/27 02:13:11 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

void	ft_init(t_pipex *pipex, int argc, char *argv[])
{
	int	i;

	pipex->status = 0;
	if (argv[1] && ft_strcmp(argv[1], "here_doc"))
		pipex->n = argc - 4;
	else
		pipex->n = argc - 3;
	pipex->count = 0;
	pipex->pipes = (int **)malloc((pipex->n - 1) * sizeof(int *));
	if (!pipex->pipes)
		(ft_perror("Malloc failed: pipes"), ft_free_pipex(pipex),
			exit(EXIT_FAILURE));
	i = 0;
	while (i < pipex->n - 1)
	{
		pipex->pipes[i] = (int *)malloc(2 * sizeof(int));
		if (!pipex->pipes[i] || pipe(pipex->pipes[i]) == -1)
			(ft_perror("Pipe error: pipes[i]"), ft_free_pipex(pipex),
				exit(EXIT_FAILURE));
		i++;
	}
	pipex->pids = (pid_t *)malloc((pipex->n) * sizeof(pid_t));
	if (!pipex->pids)
		(ft_perror("Malloc failed: pids"), ft_free_pipex(pipex),
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
	write(2, "\n", 1);
}

void	ft_errno(char *argument)
{
    if (errno == ENOENT)
        (ft_perror("pipex: "), ft_perror(argument),
			ft_perror(": No such file or directory"));
    else if (errno == EACCES)
        (ft_perror("pipex: "), ft_perror(argument),
			ft_perror(": Permission denied"));
	else if (errno == ENOMEM)
		ft_perror("pipex: Out of memory");
	else if (errno == EINVAL)
		ft_perror("pipex: Invalid argument");
	else if (errno == EISDIR)
		ft_perror("pipex: Is a directory");
	else if (errno == EMFILE)
		ft_perror("pipex: Too many open files");
	else if (errno == ENFILE)
		ft_perror("pipex: Too many open files in system");
	else if (errno == EFAULT)
		ft_perror("pipex: Bad address");
	else if (errno == EROFS)
		ft_perror("pipex: Read-only file system");
	else if (errno == ETXTBSY)
		ft_perror("pipex: Text file busy");
	else
		ft_perror("pipex: Open failed");
}

void	ft_free_pipex(t_pipex *pipex)
{
	int	i;

	if (!pipex)
        return ;
	i = 0;
	if (pipex->pipes)
    {
		while (i < pipex->n - 1 && pipex->pipes[i])
		{
			close(pipex->pipes[i][READ]);
			close(pipex->pipes[i][WRITE]);
			free(pipex->pipes[i]);
			i++;
		}
		free(pipex->pipes);
	}
	if (pipex->clean_paths)
    {
		i = 0;
		while (pipex->clean_paths[i])
		{
			free(pipex->clean_paths[i]);
			i++;
		}
		free(pipex->clean_paths);
	}
    if (pipex->commands)
    {
		i = 0;
		while (pipex->commands[i])
		{
			free(pipex->commands[i]);
			i++;
		}
		free(pipex->commands);
	}
	if (pipex->pids)
		free(pipex->pids);
	free(pipex);
}
