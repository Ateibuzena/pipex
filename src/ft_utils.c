#include "../pipexft.h"

char	*ft_search_way(char *str, char **env, int len)
{
	int	i;
	int	boolean;

	i = 0;
	boolean = 0;
	while (env[i])
	{
		if (!ft_strncmp(str, env[i], len)) //los dos strings son iguales
			boolean = 1;
		else
			i++;
	}
	if (boolean)
		return (env[i]);
	return (NULL);
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
		if (!cmd)
			return (NULL);
		path[i] = ft_strjoin(path[i], cmd);
		if (access(path[i], X_OK) == 0) //comprobando si es ejecutable, el 0 indica exito
			i = ft_strlen_double(path) - 1; //para salir del bucle; puedo retornar path[i]?
		free(path[i]);
		path[i] = NULL;
		i++;
	}
	if (!path[i])
		return (NULL);
	return (path[i]);
}
