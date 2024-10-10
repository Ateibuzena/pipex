#include "../pipexft.h"

void	ft_execute_cmd(char *argv, char **env, char *pathname)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		pipex = NULL;
	pipex->commands = ft_split(argv, ' '); //spliteamos el comando general en sus mini comandos
	pipex->found_way = ft_search_way("PATH=", env, 4); //buscamos la variable de entorno PATH
	pipex->clean_paths = ft_clean_path(pipex->found_way, 5); //limpiamos los posible path encontrados
	pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]); //buscamos path accesible para el primer comando
	if (!pathname) //si no hagy path accesible
	{
		//free(pathname); //creo que esto no es necesario;
		ft_freedouble(pipex->clean_paths); //liberamos clean paths para buscar nuevos
		pipex->found_way = ft_search_way("PWD=", env, 3); //buscamos la variable de entorno PWD
		pipex->clean_paths = ft_clean_path(pipex->found_way, 4);
		//no spliteamos los commancs porque ya lo tenemos
		pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]); //buscamos path accesible para el primer commando
		if (!pathname)
		{
			//free(pathname);
			ft_freedouble(pipex->clean_paths);
			ft_freedouble(pipex->commands);
			//faltaria cerrar el programa?
		}
	}
	if (pathname)//sobra si pongo que salga del programa ahí arriba?
		execve(pathname, pipex->commands, env);
	ft_printf("Command not found: %d\n", ft_strlen(argv));
	exit(127);
}

void	ft_first_process(int **fds, char **argv, char **env)
{
	int	reading;

	// Cerrar lectura del primer pipe
	//close(fds[0][0]);

	// Abrir el archivo de entrada
	reading = open(argv[1], O_RDONLY);
	//ft_printf("fd de file1: %d", reading);
	if (reading < 0)
	{
		perror("Open input file error first");
		exit(1);
	}

	// Redirigir la entrada estándar al archivo de entrada
	if (dup2(reading, STDIN_FILENO) == -1)
	{
		perror("Dup2 input error first");
		exit(1);
	}

	// Redirigir la salida estándar al primer pipe
	//ft_printf("pipe 1: %d", fds[0][1]);
	/*if (dup2(fds[0][1], STDOUT_FILENO) == -1)
	{
		perror("Dup2 output error first");
		exit(1);
	}*/

	// Cerrar el archivo y el pipe de escritura
	close(reading);
	close(fds[0][1]);

	// Ejecutar el primer comando (argv[2])
	ft_execute_cmd(argv[2], env, NULL);
}

void	ft_middle_process(int **fds, char **argv, char **env, int i)
{
	// Cerrar escritura del pipe anterior y lectura del siguiente pipe
	close(fds[i - 1][1]);
	close(fds[i][0]);

	// Redirigir la entrada estándar al pipe anterior
	if (dup2(fds[i - 1][0], STDIN_FILENO) == -1)
	{
		perror("Dup2 input error middle");
		exit(1);
	}

	// Redirigir la salida estándar al siguiente pipe
	if (dup2(fds[i][1], STDOUT_FILENO) == -1)
	{
		perror("Dup2 output error middle");
		exit(1);
	}

	// Cerrar los pipes ya redirigidos
	close(fds[i - 1][0]);
	close(fds[i][1]);

	// Ejecutar el comando intermedio (argv[2 + i])
	ft_execute_cmd(argv[i + 2], env, NULL);
}

void	ft_last_process(int **fds, char **argv, char **env, int argc)
{
	int	writing;

	// Cerrar escritura del último pipe
	close(fds[argc - 5][1]);

	// Abrir el archivo de salida
	writing = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (writing < 0)
	{
		perror("Open output file error last");
		exit(1);
	}

	// Redirigir la entrada estándar al último pipe
	if (dup2(fds[argc - 5][0], STDIN_FILENO) == -1)
	{
		perror("Dup2 input error last");
		exit(1);
	}

	// Redirigir la salida estándar al archivo de salida
	if (dup2(writing, STDOUT_FILENO) == -1)
	{
		perror("Dup2 output error last");
		exit(1);
	}

	// Cerrar el pipe de lectura y el archivo de salida
	close(fds[argc - 5][0]);
	close(writing);

	// Ejecutar el último comando (argv[argc - 2])
	ft_execute_cmd(argv[argc - 2], env, NULL);
}
