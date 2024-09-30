#include "../pipexft.h"

void	ft_execute_cmd(char *argv, char **env, char *pathname)
{
	t_pipex	*pipex;

	ft_printf("estoy en execute\n");
	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
		pipex = NULL;
	pipex->commands = ft_split(argv, ' '); //spliteamos el comando general en sus mini comandos
	pipex->found_way = ft_search_way("PATH=", env, 4); //buscamos la variable de entorno PATH
	ft_printf("found_way: %s\n", pipex->found_way);
	if (pipex->found_way)
		pipex->clean_paths = ft_clean_path(pipex->found_way, 5); //limpiamos los posible path encontrados
	if (pipex->clean_paths)
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
	else //sobra si pongo que salga del programa ahí arriba?
		execve(pathname, pipex->commands, env);
	ft_printf("Command not found: %d\n", ft_strlen(argv));
	exit(127);
}

void	ft_child_process_1(int fd[2], char **argv, char **env)
{
	int		reading;
	char	*pathname;

	ft_printf("estoy en child_process_1\n");
	pathname = NULL;
	close(fd[0]); //cerramos fd de lectura para poder escribir en la pib
	reading = open(argv[1], O_RDONLY);
	if (reading < 0)
	{
		perror("Open fail in reading process\n");
		exit(1);
	}
	/*La razón por la que se usa dup2() en lugar de dup() radica en la flexibilidad y control que proporciona cada función sobre la duplicación de descriptores de archivo.
	Diferencias entre dup() y dup2():
	dup():
	Sintaxis: int dup(int oldfd);
	Descripción: dup() duplica el descriptor de archivo oldfd, pero devuelve el primer descriptor de archivo disponible, es decir, crea un nuevo descriptor de archivo que apunta al mismo recurso que oldfd.
	Uso típico: Si solo quieres duplicar un descriptor de archivo sin preocuparte por cuál será el nuevo descriptor asignado.
	Desventaja: No tienes control sobre qué número de descriptor de archivo será el nuevo. El sistema operativo asignará automáticamente el primer número de descriptor disponible (el menor número entero mayor que 2).
	dup2():
	Sintaxis: int dup2(int oldfd, int newfd);
	Descripción: dup2() también duplica oldfd, pero en este caso redirige específicamente el descriptor de archivo oldfd para que ocupe el número de descriptor newfd. Si newfd ya estaba en uso, primero cierra newfd y luego lo reutiliza para apuntar al mismo recurso que oldfd.
	Uso típico: Se usa cuando quieres duplicar un descriptor de archivo en un descriptor específico, como redirigir la entrada estándar (STDIN_FILENO) o la salida estándar (STDOUT_FILENO) hacia un archivo o tubería.
	Ventaja: Te permite controlar exactamente cuál será el nuevo descriptor de archivo, lo cual es útil en casos donde necesitas que el descriptor de archivo esté asociado a un número específico (como redirigir la entrada estándar, salida estándar o salida de error estándar).*/
	if (dup2(reading, STDIN_FILENO) == -1) //fallo al escribir la entrada por teclado en reading
	{
		perror("Input process fail in reading process\n");
		exit(1);
	}
	close(reading);
	/*Al redirigir STDOUT_FILENO, se cambia la salida de todas las funciones que imprimen en la consola (como printf(), puts(), etc.) para que en su lugar envíen sus datos a otro destino (en este caso, el pipe).
	Esto significa que todo lo que normalmente se imprimiría en la consola a través de la salida estándar será enviado a fd[1], el extremo de escritura del pipe. Luego, otro proceso o hilo puede leer esos datos desde el extremo de lectura del pipe (fd[0]).*/
	ft_printf("llegué aqui\n");
	//PROBLEMITAS EN EL IF ESTE
	/*if (dup2(fd[1], STDOUT_FILENO) == -1) //redirigir la salida al fd de escritura
	{
		perror("Output process fail in reading process\n");
		exit(1);
	}*/
	close(fd[1]);
	ft_execute_cmd(argv[2], env, pathname); //ejecutamos comando = segundo argumento
}

void	ft_child_process_2(int fd[2], char **argv, char **env)
{
	int		writing;
	char	*pathname;

	ft_printf("estoy en father_process_2\n");
	pathname = NULL;
	close(fd[1]); //cerramos fd escritura
	writing = (open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644));
	/*0644:
	Este es el valor que establece los permisos del archivo en el sistema de archivos.
	0644 es una máscara octal que se utiliza para definir los permisos de archivo. Los permisos se representan en un formato octal que controla los derechos de lectura, escritura y ejecución para el usuario (owner), grupo (group), y otros (others).
	El valor 0644 se desglosa de la siguiente manera:
	0: Un prefijo octal que indica que es un número en base 8.
	6: Lectura y escritura para el propietario del archivo (owner) → 4 (lectura) + 2 (escritura) = 6.
	4: Lectura para el grupo (group).
	4: Lectura para otros (others).
	Esto significa que:
	El propietario del archivo tiene permisos de lectura y escritura.
	El grupo y otros usuarios tienen solo permisos de lectura.*/
	if (writing < 0)
	{
		perror("Open fail in wrinting process\n");
		exit(1);
	}
	if (dup2(fd[0], STDIN_FILENO) == -1) // fallo al escribir la entrada por teclado en el pipe de lectura
	{
		perror("Input fail in writing process\n");
		exit(1);
	}
	close(fd[0]);
	if (dup2(writing, STDOUT_FILENO) == -1)
	{
		perror("Output fail in writing process\n");
		exit(1);
	}
	close(writing);
	ft_execute_cmd(argv[3], env, pathname);
}
