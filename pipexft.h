/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipexft.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: azubieta <azubieta@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 22:13:34 by azubieta          #+#    #+#             */
/*   Updated: 2024/11/24 21:02:10 by azubieta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEXFT_H
# define PIPEXFT_H

# include "../../Utils/libft/libft.h"
# include <errno.h>
# include <sys/wait.h>
# include <aio.h>
# include <string.h>
# include <stdio.h>

# define MIN_ARGS 5
# define READ 0
# define WRITE 1

typedef struct s_par
{
	int	i;
	int	j;
}	t_par;

typedef struct s_pipex
{
	int		n;
	int		i;
	int		**pipes;
	pid_t	*pids;
	char	*found_way;
	char	**clean_paths;
	char	**commands;
	int		status;
}	t_pipex;

/*SRC/*/

/*ft_process.c*/
void	ft_child_process(int input_fd, int output_fd);
void	ft_first_process(char **argv, t_pipex *pipex, char **env);
int		ft_middle_process(char **argv, t_pipex *pipex, char **env);
void	ft_last_process(int argc, char **argv, t_pipex *pipex, char **env);
void	ft_waitpid(t_pipex *pipex);

/*ft_execute.c*/
void	ft_close_pipes(t_pipex *pipex);
char	*ft_search_way(const char *key, char **env, size_t len);
char	**ft_clean_path(char *path_value);
char	*ft_accessible_path(char **paths, char *command);
void	ft_execute_cmd(t_pipex *pipex, char *argv, char **env, char *pathname);

/*ft_utils.c*/
int		ft_here_doc(char *delimiter);
void	ft_init(t_pipex *pipex, int argc);
void	ft_free_pipex(t_pipex *pipex);
void	ft_perror(const char *str);
void	ft_not_found(char *str);

#endif
