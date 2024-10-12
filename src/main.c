				
#include "../pipexft.h"

int main(int argc, char **argv, char **env) 
{
    int **fds;
    int pid;
	int	i;
	int	j;
	int	num_pipes;
    int status;
    int exit_code = 0;  // Código del proceso fallido
    pid_t *pids;        // Array para almacenar los PIDs de los procesos

    if (argc < 5) {
        ft_printf("This program must be executed like:\n");
        ft_printf("./pipex file1 command1 command2 ... commandN file2\n");
        exit(EXIT_FAILURE);
    }
    num_pipes = argc - 4;
    // Asignar memoria para las tuberías y los PIDs
    fds = (int **)malloc(num_pipes * sizeof(int *));
    pids = (pid_t *)malloc((num_pipes + 1) * sizeof(pid_t));  // Un proceso por comando
    if (!fds || !pids) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
	i = 0;
    while (i < num_pipes) 
	{
        fds[i] = (int *)malloc(2 * sizeof(int));
        if (!fds[i] || pipe(fds[i]) == -1)
		{
            perror("Pipe error");
            exit(EXIT_FAILURE);
        }
		i++;
    }
    // Crear el primer proceso para el primer comando
    pid = fork();
    if (pid < 0) {
        perror("Fork error");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) 
	{
        ft_close_pipes(fds, num_pipes, 1);
        ft_first_process(fds, argv, env);  // Proceso hijo para el primer comando
    }
	else
        pids[0] = pid;  // Guardar el PID del primer proceso
    // Crear procesos intermedios
	i = 1;
    while(i < num_pipes) 
	{
        pid = fork();
        if (pid < 0) 
		{
            perror("Fork error");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) 
		{
		   	j = 0;
			while (j < num_pipes)
			{
				if (j != i - 1)
					close(fds[j][0]);
                if (j != i)
					close(fds[j][1]);
				j++;
			}
            ft_middle_process(fds, argv, env, i);
        }
		else
            pids[i] = pid;  // Guardar el PID del proceso intermedio
		i++;
    }
    // Crear el último proceso para el último comando
    pid = fork();
    if (pid < 0) 
	{
        perror("Fork error");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) 
	{
        ft_close_pipes(fds, num_pipes, 0);
        ft_last_process(fds, argv, env, argc);
    }
	else
        pids[num_pipes] = pid;  // Guardar el PID del último proceso

    // Cerrar las tuberías en el proceso padre
	ft_close_pipes(fds, num_pipes, 0);
	i = 0;
    // Esperar a todos los procesos hijos
	while(i <= num_pipes)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0) 
		{
			fprintf(stderr, "Error in command %d\n", i + 1);
			exit_code = WEXITSTATUS(status);
		}
		i++;
	}
	ft_free_pipes(fds, num_pipes);
    free(pids);
	if (exit_code == 0)
		return (EXIT_SUCCESS);
	return (exit_code);
}
