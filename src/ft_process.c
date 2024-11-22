/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 17:37:22 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/22 22:32:33 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipexft.h"

void	ft_child_process(int input_fd, int output_fd)
{
	if (input_fd != STDIN_FILENO)
	{
		if (dup2(input_fd, STDIN_FILENO) == -1)
			ft_perror("Dup2 failed: input\n");
		close(input_fd);
	}
	if (output_fd != STDOUT_FILENO)
	{
		if (dup2(output_fd, STDOUT_FILENO) == -1)
			ft_perror("Dup2 failed: output\n");
		close(output_fd);
	}
}

void	ft_first_process(char **argv, t_pipex *pipex, char **env)
{
	int	infile;

	if (ft_strcmp(argv[1], "here_doc") != 0)
		infile = ft_here_doc(argv[2]);
	else
		infile = open(argv[1], O_RDONLY);
	if (infile < 0)
		(ft_free_pipex(pipex), ft_perror("Open failed: infile"));
	pipex->pids[0] = fork();
	if (pipex->pids[0] < 0)
		(ft_free_pipex(pipex), ft_perror("Fork error: first_process\n"));
	if (pipex->pids[0] == 0)
	{
		close(pipex->pipes[0][READ]);
		ft_child_process(infile, pipex->pipes[0][WRITE]);
		ft_execute_cmd(pipex, argv[1 + pipex->i], env, NULL);
	}
	close(infile);
	if (ft_strcmp(argv[1], "here_doc") != 0)
		unlink("../temporal.txt");
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
			ft_perror("Fork error: middle_process\n");
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

	if (ft_strcmp(argv[1], "here_doc") != 0)
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (outfile < 0)
		(ft_free_pipex(pipex), ft_perror("Open failed: outfile"));
	pipex->pids[pipex->i] = fork();
	if (pipex->pids[pipex->i] < 0)
		(ft_free_pipex(pipex), ft_perror("Fork error: first_process\n"));
	if (pipex->pids[pipex->i] == 0)
	{
		ft_child_process(pipex->pipes[pipex->i - 1][READ], outfile);
		ft_execute_cmd(pipex, argv[argc - 2], env, NULL);
	}
	close(pipex->pipes[pipex->i - 1][READ]);
	close(outfile);
}

void	ft_waitpid(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < (pipex->n - 1))
	{
		waitpid(pipex->pids[i], NULL, 0);
		i++;
	}
}
