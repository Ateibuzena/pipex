				
#include "../pipexft.h"

int	main(int argc, char **argv, char **env)
{
	int	**fds;
	int	pid;
	int	i;
	int	num_pipes;
	int reading;

	if (argc < 5)
	{
		ft_printf("This program must be execute like:\n");
		ft_printf("./pipex file1 comand1 comand2 ... comandN file2\n");
		exit(1);
	}
	num_pipes = argc - 4;
	fds = (int **)malloc(num_pipes * sizeof(int *));
	if (!fds)
	{
		perror("Malloc fail");
		exit(1);
    }
	i = 0;
	// Asignar memoria a cada puntero en fds usando while
    while (i < num_pipes) 
	{
        fds[i] = (int *)malloc(2 * sizeof(int));  // Asignar memoria para un entero
		if (pipe(fds[i]) == -1) //fallo al crear fd de lectura y fd de escritura
		{
			perror("Pipe error\n");
			exit(1);
		}
		ft_printf("Pipe %d created with fds[%d][0]: %d, fds[%d][1]: %d\n", 
           i, i, fds[i][0], i, fds[i][1]);
        i++;
    }
	pid = fork(); //creando proceso hijo primero
	if (pid < 0)
	{
		perror("Fork error\n");
		exit(1);
	}
	if (pid == 0)
	{
		// Redirigir stdout al primer pipe y stdin al archivo de entrada
		close(fds[0][0]); // Cerrar lectura del primer pipe
		reading = open(argv[1], O_RDONLY); // Abrir el archivo de entrada
        if (reading < 0)
        {
            perror("Open input file error first");
            exit(1);
        }
		dup2(reading, STDOUT_FILENO);
		dup2(fds[0][1], STDOUT_FILENO); // Redirigir stdout al pipe
		close(reading);
		close(fds[0][1]);
		ft_first_process(fds, argv, env); // Primer comando (argv[2])
		exit(0);
	}
	//Procesos intermedios
	i = 1;
	while (i < num_pipes)
	{
		pid = fork(); //creando procesos hijos intermedios
		if (pid < 0)
		{
			perror("Fork error\n");
			exit(1);
		}
		if (pid == 0)
		{
			// Conectar la salida del pipe anterior a la entrada del actual
			close(fds[i - 1][1]); // Cerrar escritura del pipe anterior
			if (dup2(fds[i - 1][0], STDIN_FILENO) == -1) 
			{
				perror("Dup2 input error in main");
				exit(1);
        	}// Redirigir stdin al pipe anterior
			// Asegurarse de que estamos usando los pipes válidos
			if (fds[i][0] == -1 || fds[i][1] == -1) 
			{
				perror("Pipe not valid");
				exit(1);
			}
			close(fds[i][0]); // Cerrar lectura del siguiente pipe
			if (dup2(fds[i][1], STDOUT_FILENO) == -1) 
			{
				perror("Dup2 output error in main 2");
				exit(1);
        	} // Redirigir stdout al siguiente pipe
			ft_middle_process(fds, argv, env, i); // Comando intermedio (argv[2 + i])
			exit(0);
		}
		// Cerrar las tuberías que ya no se necesitan
		close(fds[i - 1][0]);
		close(fds[i - 1][1]);
		i++;
	}
	//Ultimo comando
	pid = fork();
	if (pid < 0)
	{
		perror("Fork error\n");
		exit(1);
	}
	if (pid == 0)
	{
		// Redirigir stdin al último pipe y stdout al archivo de salida
		close(fds[num_pipes - 1][1]); // Cerrar escritura del último pipe
		dup2(fds[num_pipes - 1][0], STDIN_FILENO); // Redirigir stdin al pipe
		ft_last_process(fds, argv, env, argc); // Último comando (argv[argc - 2])
		exit(0);
	}
	// Cerrar las últimas tuberías
	close(fds[num_pipes - 1][0]);
	close(fds[num_pipes - 1][1]);
	// Esperar a que todos los procesos terminen
	i = 0;
	while (i < num_pipes + 1) // num_pipes + 1 porque hay un proceso por comando
	{
		wait(NULL);
		i++;
	}
	// Liberar memoria
	i = 0;
	while (i < num_pipes)
	{
		free(fds[i]);
		i++;
	}
	free(fds);
	 /*El motivo para usar wait(NULL) es evitar que el proceso padre continúe
	ejecutándose antes de que el proceso hijo haya terminado
	Función wait(): La función wait() hace que el proceso padre espere 
	a que cualquiera de sus procesos hijos termine su ejecución.
    Argumento NULL: El argumento NULL en wait(NULL) significa que el padre 
	no necesita obtener información detallada sobre el estado de terminación del hijo, 
	solo le interesa que el hijo termine.*/
}
