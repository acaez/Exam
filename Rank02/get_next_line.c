#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 1
#endif

char    *ft_join(char *line, char *buffer, int size, int from, int to)
{
    int        i = -1;
    char    *stash;

    i = -1;
    stash = (char *)malloc(sizeof(char) * (size + to + 1));
    while (++i < size)
        stash[i] = line[i];
    while (from < to)
    {
        stash[i] = buffer[from];
        i++;
        from++;
    }
    stash[i] = '\0';
    free(line);
    return (stash);
}

char    *get_next_line(int fd)
{
    char        *line;
    int            start;
    int            size = 0;
    static int    cursor = 0;
    static int    bytes = -1;
    static char    buffer[BUFFER_SIZE];

    if (fd < 0 || BUFFER_SIZE < 1)
        return (NULL);
    if (cursor >= bytes)
    {
        bytes = read(fd, buffer, BUFFER_SIZE);
        cursor = 0;
    }
    if (!bytes || bytes <= 0)
        return (cursor = 0, NULL);
    line = malloc(1);
    while (bytes > 0 && line)
    {
        start = cursor;
        while (cursor < bytes && buffer[cursor] != '\n')
            cursor++;
        if (cursor >= bytes)
        {
            line = ft_join(line, buffer, size, start, cursor);
            bytes = read(fd, buffer, BUFFER_SIZE);
            size += (cursor - start);
            cursor = 0;
        }
        else if (buffer[cursor] == '\n')
            return (ft_join(line, buffer, size, start, ++cursor));
    }
    if (bytes == -1)
        return (free(line), line = NULL, NULL);
    return (line);
}
