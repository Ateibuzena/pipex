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
void	ft_child_process_1(int fd[2], char **argv, char **env);
void	ft_child_process_2(int fd[2], char **argv, char **env);
void	ft_execute_cmd(char *argv, char **env, char *pathname);

/*ft_utils.c*/
char	*ft_search_way(char *str, char **env, int len);
char	**ft_clean_path(char *path, int	start);
char	*ft_accessible_path(char **path, char *cmd);

#endif
