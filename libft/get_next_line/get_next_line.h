/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stkoh <stkoh@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 22:05:29 by stkoh             #+#    #+#             */
/*   Updated: 2026/10/20 03:38:42 by stkoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

# ifndef NEW_LINE_CHAR
#  define NEW_LINE_CHAR '\n'
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_linked_fd
{
	int					fd;
	char				*storage;
	struct s_linked_fd	*next;
}	t_linked_fd;

char		*get_next_line(int fd);
int			gnl_find_char(const char *str, const char c, int size_read);
int			gnl_char_join(char **storage, char *content);
int			gnl_extract_line(char **storage, char **line, int size_read);
size_t		gnl_strlen(const char *str);
t_linked_fd	*find_fd_storage(t_linked_fd **fd_storage, int fd);
int			return_null(int fd, int *read_size,
				t_linked_fd **fd_storage, t_linked_fd **current);
#endif
