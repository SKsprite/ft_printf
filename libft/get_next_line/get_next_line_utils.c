/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 22:06:56 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 03:40:35 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *str)
{
	size_t	count;

	count = 0;
	while (str[count])
		count++;
	return (count);
}

int	gnl_char_join(char	**storage, char *content)
{
	size_t	storage_size;
	size_t	content_size;
	char	*allocated;

	if (!*storage)
	{
		*storage = malloc(sizeof(char));
		if (!*storage)
			return (0);
		(*storage)[0] = '\0';
	}
	storage_size = gnl_strlen(*storage);
	content_size = gnl_strlen(content);
	allocated = malloc((storage_size + content_size + 1) * sizeof(char));
	if (!allocated)
		return (free(*storage), *storage = NULL, 0);
	allocated[storage_size + content_size] = '\0';
	while (content_size--)
		allocated[storage_size + content_size] = content[content_size];
	while (storage_size--)
		allocated[storage_size] = (*storage)[storage_size];
	free(*storage);
	*storage = allocated;
	return (1);
}

int	gnl_extract_line(char **storage, char **line, int size_read)
{
	size_t	storage_size;
	size_t	content_size;
	size_t	remaining;
	char	*allocated;

	storage_size = gnl_strlen(*storage);
	content_size = gnl_find_char(*storage, NEW_LINE_CHAR, size_read);
	*line = malloc((content_size + 1) * sizeof(char));
	if (!*line)
		return (free(*storage), *storage = NULL, 0);
	remaining = storage_size - content_size;
	allocated = malloc((remaining + 1) * sizeof(char));
	if (!allocated)
		return (free(*line), free(*storage), *storage = NULL, 0);
	allocated[remaining] = '\0';
	while (remaining--)
		allocated[remaining] = (*storage)[content_size + remaining];
	(*line)[content_size] = '\0';
	while (content_size--)
		(*line)[content_size] = (*storage)[content_size];
	free(*storage);
	*storage = allocated;
	return (1);
}

t_linked_fd	*find_fd_storage(t_linked_fd **fd_storage, int fd)
{
	t_linked_fd	*current;
	t_linked_fd	*new_node;

	current = *fd_storage;
	while (current)
	{
		if (current->fd == fd)
			return (current);
		current = current->next;
	}
	new_node = malloc(sizeof(t_linked_fd));
	if (!new_node)
		return (NULL);
	new_node->fd = fd;
	new_node->storage = NULL;
	new_node->next = *fd_storage;
	*fd_storage = new_node;
	return (new_node);
}

int	return_null(int fd, int *read_size,
	t_linked_fd **fd_storage, t_linked_fd **current)
{
	if (fd < 0)
		return (1);
	*read_size = 1;
	*current = find_fd_storage(fd_storage, fd);
	if (!*current)
		return (1);
	return (0);
}
