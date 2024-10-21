#include "../pipexft.h"

char	*ft_search_way(char *str, char **env, int len)
{
	int	i;

	//ft_printf("llego aqui");
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(str, env[i], len)) //los dos strings son iguales 4 t54h 69 0
			return (env[i]);
		i++;
	}
	return (env[i]);
}

char	**ft_clean_path(char *path, int	start)
{
	char	**cleaned;
	int		i;

	i = 0;
	if (!path)
		return (ft_split(" ", ' '));
	cleaned = ft_split(&path[start], ':'); //caracter delimitador :, empezamos despues de PATH= o de PWD=
	if (!cleaned)
		return (NULL);
	while (cleaned[i])
	{
		cleaned[i] = ft_strjoin(cleaned[i], "/"); //creando paths con / al final para añadir el cmd
		i++;
	}
	return (cleaned);
}

char	*ft_accessible_path(char **path, char *cmd)
{
	int	i;

	i = 0;
	while (path[i])
	{
		//fprintf(stderr, "PETAMOS\n\n: \n");
		if (!cmd)
			return (NULL);
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], X_OK) == 0) //comprobando si es ejecutable, el 0 indica exito
		{
			//fprintf(stderr, "path[%d]: %s\n", i, path[i]);
			return (path[i]); //para salir del bucle; puedo retornar path[i]?
		}
		free(path[i]);
		path[i] = NULL;
		i++;
	}
	return (NULL);
}

void ft_execute_cmd(t_pipex *pipex, char *argv, char **env, char *pathname)
{
    pipex->commands = ft_split(argv, ' ');
    //fprintf(stderr, "comando: %s\n", pipex->commands[0]);
    pipex->found_way = ft_search_way("PATH=", env, 4);
    pipex->clean_paths = ft_clean_path(pipex->found_way, 5);

    pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);

    // Si no se encuentra un path accesible, buscamos en PWD
    if (!pathname)
    {
        ft_freedouble(pipex->clean_paths);
        pipex->found_way = ft_search_way("PWD=", env, 3);
        pipex->clean_paths = ft_clean_path(pipex->found_way, 4);

        pathname = ft_accessible_path(pipex->clean_paths, pipex->commands[0]);
        if (!pathname)
        {
            ft_freedouble(pipex->clean_paths);
            ft_freedouble(pipex->commands);
            //ft_free_pipex(pipex);
            ft_exit_error(argv);  // Termina el programa si no se encuentra el comando
        }
    }

    //fprintf(stderr, "PETAMOS\n\n: \n");
    execve(pathname, pipex->commands, env);  // Ejecutamos el comando

    // Si `execve` falla, liberamos memoria y salimos con error
    perror("Execution failed");
    ft_freedouble(pipex->clean_paths);
    ft_freedouble(pipex->commands);
    free(pipex);
    exit(1);
}

