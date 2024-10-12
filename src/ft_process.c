#include "../pipexft.h"

void	ft_exit_error(char *str)
{
	write(2, "Command not found:  ", 19);
	write(2, str, ft_strlen(str));
	write(2, "\n", 1);
	exit(127);
}

void ft_execute_cmd(char *argv, char **env, char *pathname) {
    t_pipex *pipex = malloc(sizeof(t_pipex));
    if (!pipex) {
        perror("Memory allocation failed");
        exit(1);
    }

    pipex->commands = ft_split(argv, ' ');
    pipex->found_way = ft_search_way("PATH=", env, 4);
    pipex->clean_paths = ft_clean_path(pipex->found_way, 5);

    pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);

    // Si no se encuentra un path accesible, buscamos en PWD
    if (!pathname) {
        ft_freedouble(pipex->clean_paths);
        pipex->found_way = ft_search_way("PWD=", env, 3);
        pipex->clean_paths = ft_clean_path(pipex->found_way, 4);

        pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);
        if (!pathname) {
            ft_freedouble(pipex->clean_paths);
            ft_freedouble(pipex->commands);
            free(pipex);
            ft_exit_error(argv);  // Termina el programa si no se encuentra el comando
        }
    }

    execve(pathname, pipex->commands, env);  // Ejecutamos el comando

    // Si `execve` falla, liberamos memoria y salimos con error
    perror("Execution failed");
    ft_freedouble(pipex->clean_paths);
    ft_freedouble(pipex->commands);
    free(pipex);
    exit(1);
}


void ft_first_process(int **fds, char **argv, char **env) {
    int reading = open(argv[1], O_RDONLY);
    if (reading < 0) {
        perror("Open input file error");
        exit(1);
    }

    if (dup2(reading, STDIN_FILENO) == -1) {
        perror("Dup2 input error");
        exit(1);
    }
    if (dup2(fds[0][1], STDOUT_FILENO) == -1) {
        perror("Dup2 output error");
        exit(1);
    }

    close(reading);
    //close(fds[0][0]);  // Cerrar la lectura que no usamos
    close(fds[0][1]);

    ft_execute_cmd(argv[2], env, NULL);
}

void ft_middle_process(int **fds, char **argv, char **env, int i) {
    if (dup2(fds[i - 1][0], STDIN_FILENO) == -1) {
        perror("Dup2 input error");
        exit(1);
    }
    if (dup2(fds[i][1], STDOUT_FILENO) == -1) {
        perror("Dup2 output error");
        exit(1);
    }

    close(fds[i - 1][0]);
    //close(fds[i - 1][1]);
    //close(fds[i][0]);
    close(fds[i][1]);

    ft_execute_cmd(argv[i + 2], env, NULL);
}

void ft_last_process(int **fds, char **argv, char **env, int argc) {
    int writing = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (writing < 0) {
        perror("Open output file error");
        exit(1);
    }
	close(fds[argc - 5][1]);  // Cerrar el pipe de escritura anterior

    if (dup2(fds[argc - 5][0], STDIN_FILENO) == -1) {
        perror("Dup2 input error");
        exit(1);
    }
    if (dup2(writing, STDOUT_FILENO) == -1) {
        perror("Dup2 output error");
        exit(1);
    }

    close(writing);
    close(fds[argc - 5][0]);

    ft_execute_cmd(argv[argc - 2], env, NULL);
}
