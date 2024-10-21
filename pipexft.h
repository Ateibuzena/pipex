#ifndef PIPEXFT_H
#define PIPEXFT_H

# include "/home/azubieta/sgoinfre/azubieta/utils/libft/libft.h"
# include <errno.h>
# include <sys/wait.h>
# include <aio.h>
# include <string.h>
# include <stdio.h>

# define MIN_ARGS 5
# define READ 0
# define WRITE 1

/*typedef struct s_pipex
{
	int		np;
	int		i;
	int		status;
	int		exit_code;
	int		**pipes;
	pid_t	*pids;
	char	*found_way;
	char	**clean_paths;
	char	**commands;

} t_pipex;*/

typedef struct s_pipex
{
	int		n;
	int		**pipes;
	pid_t	*pids;
	char	*found_way;
	char	**clean_paths;
	char	**commands;

} t_pipex;

/*SRC/*/

/*ft_process.c*/
void	ft_child_process(int input_fd, int output_fd);
void	ft_first_process(char **argv, t_pipex *pipex, char **env);
int		ft_middle_process(char **argv, t_pipex *pipex, char **env);
void	ft_last_process(int	i, int argc, char **argv, t_pipex *pipex, char **env);

/*ft_execute.c*/
char	*ft_search_way(char *str, char **env, int len);
char	**ft_clean_path(char *path, int	start);
char	*ft_accessible_path(char **path, char *cmd);
void	ft_execute_cmd(t_pipex *pipex, char *argv, char **env, char *pathname);

/*ft_utils.c*/
void	ft_check_pipex(int	argc);
void	ft_free_pipes(int **fds, int num_pipes);
void	ft_exit_error(char *str);
void	ft_perror(const char *str);
void	ft_init(t_pipex *pipex, int argc);

/*main.c*/
void	ft_waitpid(t_pipex *pipex);


#endif
