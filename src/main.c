#include "/home/azubieta/sgoinfre/azubieta/actual/pipex_github/pipexft.h"

int	main(int argc, char **argv, char **env)
{
	int	fd[2];
	int	pid;

	if (argc != 5)
	{
		ft_printf("This program must be execute like:\n");
		ft_printf("./pipex file1 comand1 comand2 file2\n");
		exit(1);
	}

	if (pipe(fd) == -1) //fallo al crear fd de lectura y fd de escritura
	{
		perror("Pipe error\n");
		exit(1);
	}
	pid = fork(); //creando proceso hijo
	if (pid < 0)
	{
		perror("Fork error\n");
		exit(1);
	}
	if (pid == 0)
	{
		ft_printf("I'm child process\n");
		ft_child_process_1(fd, argv, env);
	}
	else
	{
		ft_printf("I'm father process\n");
		ft_child_process_2(fd, argv, env);
	}
	wait(NULL); /*El motivo para usar wait(NULL) es evitar que el proceso padre continúe
	ejecutándose antes de que el proceso hijo haya terminado
	Función wait(): La función wait() hace que el proceso padre espere 
	a que cualquiera de sus procesos hijos termine su ejecución.
    Argumento NULL: El argumento NULL en wait(NULL) significa que el padre 
	no necesita obtener información detallada sobre el estado de terminación del hijo, 
	solo le interesa que el hijo termine.*/
}
