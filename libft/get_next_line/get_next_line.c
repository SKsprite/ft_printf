/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 22:03:38 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 03:40:24 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	gnl_find_char(const char *str, const char c, int size_read)
{
	int	i;

	i = 0;
	while (str[i] && ((unsigned char) str[i] != (unsigned char) c))
		i++;
	if (!str[i] && size_read)
		return (-1);
	if (size_read)
		i++;
	return (i);
}

int	read_and_append(char **storage, char *buffer, int fd)
{
	ssize_t	read_size;

	if (BUFFER_SIZE <= 0)
		return (-1);
	read_size = read(fd, buffer, BUFFER_SIZE);
	if (read_size <= 0)
		return (read_size);
	buffer[read_size] = '\0';
	return (gnl_char_join(storage, buffer));
}

char	*malloc_buff(char **buff, int buffer_size)
{
	if (buffer_size <= 0)
		return (NULL);
	*buff = malloc((buffer_size + 1) * sizeof(char));
	if (!*buff)
		return (NULL);
	return (*buff);
}

void	free_fd_storage(t_linked_fd **fd_storage, int fd)
{
	t_linked_fd	*current;
	t_linked_fd	*prev;

	current = *fd_storage;
	prev = NULL;
	while (current)
	{
		if (current->fd == fd)
		{
			if (prev)
				prev->next = current->next;
			else
				*fd_storage = current->next;
			current->next = NULL;
			free(current->storage);
			current->storage = NULL;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

char	*get_next_line(int fd)
{
	static t_linked_fd	*fd_storage;
	char				*buff;
	int					read_size;
	char				*line;
	t_linked_fd			*current;

	if (return_null(fd, &read_size, &fd_storage, &current))
		return (NULL);
	if (!malloc_buff(&buff, BUFFER_SIZE))
		return (free_fd_storage(&fd_storage, fd), NULL);
	while (read_size != 0 && (!current->storage || !current->storage[0]
			|| !(gnl_find_char(current->storage, NEW_LINE_CHAR, 1) >= 0)))
	{
		read_size = read_and_append(&current->storage, buff, fd);
		if (read_size < 0)
			return (free(buff), free_fd_storage(&fd_storage, fd), NULL);
	}
	if ((read_size == 0 && !current->storage)
		|| !gnl_extract_line(&current->storage, &line, read_size))
		return (free(buff), free_fd_storage(&fd_storage, fd), NULL);
	if (gnl_strlen(current->storage) == 0)
		return (free(buff), free_fd_storage(&fd_storage, fd), line);
	free(buff);
	return (line);
}
