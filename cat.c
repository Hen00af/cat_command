#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void	read_and_write(int fd)
{
	char	*buf;
	ssize_t	bytes;
	int		buf_size;

	buf_size = 1024;
	buf = malloc(buf_size);
	if (!buf)
		return ;

	if (fd == STDIN_FILENO)
	{	
		while ((bytes = read(fd, buf, buf_size)) > 0)
			write(STDOUT_FILENO, buf, bytes);
		free(buf);
		return ;
	}

	while (1)
	{
		bytes = read(fd, buf, buf_size);
		if (bytes <= 0)
			break ;

		write(STDOUT_FILENO, buf, bytes);

		if (bytes < buf_size)
			break ;

		buf_size *= 2;
		char *new_buf = realloc(buf, buf_size);
		if (!new_buf)
		{
			free(buf);
			return ;
		}
		buf = new_buf;
	}
	free(buf);
}

int	main(int ac, char **av)
{
	int	fd;
	int	i;

	if (ac == 1)
	{
		read_and_write(STDIN_FILENO);
		return (0);
	}

	i = 1;
	while (i < ac)
	{
		if (strcmp(av[i], "-") == 0)
		{
			read_and_write(STDIN_FILENO);
		}
		else
		{
			fd = open(av[i], O_RDONLY);
			if (fd == -1)
				perror(av[i]);
			else
			{
				read_and_write(fd);
				close(fd);
			}
		}
		i++;
	}
	return (0);
}
