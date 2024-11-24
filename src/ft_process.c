/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:37:22 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/24 20:48:30 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

void	ft_child_process(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
			(close(output_fd), close(input_fd), ft_perror("Dup2 failed: input"), exit(1));
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		if (dup2(output_fd, STDOUT_FILENO) == -1)
			(close(output_fd), ft_perror("Dup2 failed: output"), exit(1));
		close(output_fd);
	}
}

void	ft_first_process(char **argv, t_pipex *pipex, char **env)
{
	int	infile;

	pipex->pids[0] = fork();
	if (pipex->pids[0] < 0)
		(ft_free_pipex(pipex), ft_perror("Fork error: first_process\n"), exit(1)); //exit, cerrar fds anteriores y pipes, cambiar a ft_perror, 
	if (pipex->pids[0] == 0)
	{
		if (argv[1] && ft_strcmp(argv[1], "here_doc"))
			infile = ft_here_doc(argv[2]);
		else if (argv[1] && !ft_strcmp(argv[1], "here_doc"))
			infile = open(argv[1], O_RDONLY);
		if (infile < 0)
		{
			if (errno == ENOENT)
				(ft_perror("No such file or directory")); //quitar exits
			else if (errno == EACCES)
				(ft_perror("Permission denied"));
			else
				(ft_perror("Open failed"));
			ft_free_pipex(pipex);
			exit(1);
		}
		close(pipex->pipes[0][READ]);
		ft_child_process(infile, pipex->pipes[0][WRITE]);
		ft_execute_cmd(pipex, argv[1 + pipex->i], env, NULL);
	}
	close(pipex->pipes[0][WRITE]);
}

int	ft_middle_process(char **argv, t_pipex *pipex, char **env)
{
	int	i;
	int	j;

	j = 0;
	if (ft_strcmp(argv[1], "here_doc") != 0)
		j = 1;
	i = 1;
	while (i < (pipex->n - 1))
	{
		pipex->pids[i] = fork();
		if (pipex->pids[i] < 0)
			(ft_free_pipex(pipex), ft_perror("Fork error: middle_process"), exit(1));
		if (pipex->pids[i] == 0)
		{
			close(pipex->pipes[i][READ]);
			ft_child_process(pipex->pipes[i - 1][READ], pipex->pipes[i][WRITE]);
			ft_execute_cmd(pipex, argv[i + j + 2], env, NULL);
		}
		close(pipex->pipes[i - 1][READ]);
		close(pipex->pipes[i][WRITE]);
		i++;
	}
	return (i);
}

void	ft_last_process(int argc, char **argv, t_pipex *pipex, char **env)
{
	int	outfile;

	pipex->pids[pipex->i] = fork();
	if (pipex->pids[pipex->i] < 0)
		(ft_free_pipex(pipex), ft_perror("Fork error: first_process"), exit(1)); //cerrar fds anteriores
	if (pipex->pids[pipex->i] == 0)
	{
		if (ft_strcmp(argv[1], "here_doc") != 0)
			outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile < 0)
		{
			if (errno == EACCES)
				(ft_perror("Permission denied"));
			else
				(ft_perror("Open failed"));
			ft_free_pipex(pipex);
			exit(1);
		}
		ft_child_process(pipex->pipes[pipex->i - 1][READ], outfile);
		ft_execute_cmd(pipex, argv[argc - 2], env, NULL);
	}
	close(pipex->pipes[pipex->i - 1][READ]);
}

void	ft_waitpid(t_pipex *pipex)
{
	int	i;
	int status;

	i = 0;
	while (i < (pipex->n))
	{
		if (waitpid(-1, &status, 0) == pipex->pids[pipex->n - 1])
			pipex->status = WEXITSTATUS(status);
		/*if (WIFSIGNALED(pipex->status))
			pipex->status = 128 + WTERMSIG(pipex->status);*/
		i++;
	}
}
