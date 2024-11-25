/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:43:13 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/25 13:38:36 by azubieta         ###   ########.fr       */
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
		//fprintf(stderr, "dentro de here_doc\n fd here_doc read: %d fd here_doc write: %d\n", temp_pipe[READ], temp_pipe[WRITE]);
		write(1, ">", 1);
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
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

void	ft_init(t_pipex *pipex, int argc, char *argv[])
{
	int	i;

	if (argv[1] && ft_strcmp(argv[1], "here_doc"))
		pipex->n = argc - 4;
	else
		pipex->n = argc - 3;
	pipex->count = 0;
	//fprintf(stderr, "Forks hechos: %d Forks por hacer: %d\n", pipex->count, (pipex->n - pipex->count));
	pipex->pipes = (int **)malloc((pipex->n - 1) * sizeof(int *));
	if (!pipex->pipes)
		(free(pipex), ft_perror("Malloc failed: pipes"));
	i = 0;
	while (i < pipex->n - 1)
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
		while (i < pipex->n - 1)
		{
			close(pipex->pipes[i][READ]);
			close(pipex->pipes[i][WRITE]);
			free(pipex->pipes[i]);
			i++;
		}
		free(pipex->pipes);
	}
	free(pipex);
}

/*void	ft_perror(const char *str)
{
	while (str && *str)
		write(2, str++, 1);
	write(2, "\n", 1);
	exit(1);
}*/

void	ft_perror(const char *str)
{
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
}

void	ft_errno(char **arguments, char *argument)
{
	(void)arguments;
	if (errno == ENOENT)
        ft_perror(ft_strjoin("pipex: ", ft_strjoin(argument, ": No such file or directory")));
    else if (errno == EACCES)
        ft_perror(ft_strjoin("pipex: ", ft_strjoin(argument, ": Permission denied")));
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
