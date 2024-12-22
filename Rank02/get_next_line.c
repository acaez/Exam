#include get_next_line.h

char	*ft_strdup(char *src)
{
	char *dst;
	int	i;

	i = 0;
	while(src[i])
		i++;
	dst = (char *)malloc(sizeof(char) * (i + 1));
	if (!dst)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\n';
	return (dst);
}

char	get_next_line(int fd)
{
	static char buffer[BUFFER_SIZE];
	char	line[70000];
	static int b_read;
	static int b_pos;
	int	i;

	i = 0;
	i (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (b_pos >= b_read)
		{
			b_read = read(fd, buffer, BUFFER_SIZE)
			b_pos = 0;
			if (b_read <= 0)
				break ;
		}
		if (buffer[b_pos] == '\n')
			break ;
		line[i] = buffer[b_pos++]
		i++;
	}
	line[i] = '\0';
	if (i == 0)
		return (NULL);
	return (ft_strdup(line));
}
