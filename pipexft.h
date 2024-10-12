#ifndef PIPEXFT_H
#define PIPEXFT_H

# include "/home/azubieta/sgoinfre/azubieta/utils/libft/libft.h"
# include <errno.h>
# include <sys/wait.h>
# include <aio.h>
# include <string.h>
# include <stdio.h>

typedef struct s_pipex
{
	char	*found_way;
	char	**clean_paths;
	char	**commands;
} t_pipex;

/*SRC/*/

/*ft_process.c*/
void	ft_first_process(int **fds, char **argv, char **env);
void	ft_middle_process(int **fds, char **argv, char **env, int i);
void	ft_last_process(int **fds, char **argv, char **env, int argc);
void	ft_execute_cmd(char *argv, char **env, char *pathname);

/*ft_utils.c*/
void	ft_close_pipes(int **fds, int num_pipes, int i);
void	ft_free_pipes(int **fds, int num_pipes);
char	*ft_search_way(char *str, char **env, int len);
char	**ft_clean_path(char *path, int	start);
char	*ft_accessible_path(char **path, char *cmd);

#endif
