*This project has been created as part of the 42 curriculum by stkoh.*

# get_next_line

A function that reads a file descriptor one line at a time.

## Description

get_next_line is a 42 common core project. The goal is to write a function
that returns the next line from a file descriptor each time it is called:

```c
char	*get_next_line(int fd);
```

- It returns the line that was read, **including** the terminating `'\n'`,
  unless the end of the file is reached and the last line has no `'\n'`.
- It returns `NULL` when there is nothing left to read or when an error
  happens (invalid fd, `read` failure, allocation failure).
- It works with regular files and with standard input.
- The caller owns the returned line and must `free` it.

The project teaches **static variables**: data that survives between calls
of a function, which is needed to keep the characters read past the end of
the current line.

The project has two parts:

| Part | Files | Content |
|---|---|---|
| Mandatory | `get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h` | Reads lines from one fd at a time |
| Bonus | `get_next_line_bonus.c`, `get_next_line_utils_bonus.c`, `get_next_line_bonus.h` | Reads from several fds at the same time without losing each fd's place, with a single static variable |

Constraints of the project:
- Written in C and compiled with `cc -Wall -Wextra -Werror`.
- Follows the 42 Norm. No global variables, and no libft.
- The only external functions used are `read`, `malloc` and `free`.
- The read size is set at compile time with `-D BUFFER_SIZE=n`.

## Instructions

### Compile

There is no library to build. Compile the files with your own `main`:

```sh
# mandatory
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

# bonus
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c
```

If `BUFFER_SIZE` is not given, it defaults to `4096`. A `BUFFER_SIZE` of `0`
or less makes `get_next_line` return `NULL`.

### Example

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("file.txt", O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

### Testing

The tests I used are in a separate repository:
[get_next_line-Test](https://github.com/SKsprite/get_next_line-Test).
Clone it inside this folder and run it from there:

```sh
git clone https://github.com/SKsprite/get_next_line-Test
cd get_next_line-Test
make test      # mandatory part, many BUFFER_SIZE values
make bonus     # bonus part (several fds)
make valgrind  # same tests under valgrind, to check for leaks
```

See that repository's README for the full list of commands.

## Algorithm

### Mandatory part

`get_next_line` keeps one static string, `line_storage`, that holds the
characters already read from the fd but not returned yet.

1. **Check the input.** If `fd < 0` or the buffer of `BUFFER_SIZE + 1` bytes
   cannot be allocated, return `NULL`.
2. **Read until there is a full line.** While `line_storage` has no `'\n'`,
   `read` up to `BUFFER_SIZE` bytes and append them to `line_storage`
   (`char_join`). Stop when a `'\n'` is found or `read` returns `0` (end of
   file).
3. **Split the storage.** `extract_line` copies everything up to and
   including the first `'\n'` into a new string, `line`. The rest becomes the
   new `line_storage`. At end of file, the whole storage is the last line.
4. **Clean up.** The read buffer is freed on every call. If `line_storage` is
   empty after the split, or on any error, it is freed and set to `NULL`, so
   nothing is left allocated after the last line.

Why this approach:
- Only the characters needed for the next line are read, so the function
  does not read the whole file at once and works on pipes and standard input.
- The read buffer is allocated with `malloc` instead of being a local array,
  so a large `BUFFER_SIZE` does not overflow the stack.
- Every error path frees the buffer and the storage, so there are no leaks.

### Bonus part

To handle several fds at once with only one static variable, the bonus keeps
a **linked list**, one node per open fd:

```c
typedef struct s_linked_fd
{
	int					fd;
	char				*storage;
	struct s_linked_fd	*next;
}	t_linked_fd;
```

- `find_fd_storage` looks for the node of `fd`. If there is none, it adds a
  new node at the front of the list.
- The read / split steps are the same as in the mandatory part, but they use
  that node's `storage`.
- When an fd reaches the end of the file (or an error happens),
  `free_fd_storage` removes its node from the list and frees it. The other
  fds keep their place.

Why a linked list: it only uses memory for the fds that are being read,
instead of an array sized for every possible fd (`OPEN_MAX`), and it has no
limit on the fd number.

## Resources

- `man 2 read`, `man 2 open`, `man 3 malloc`.
- Brian W. Kernighan and Dennis M. Ritchie, *The C Programming Language*, 2nd edition.
- [GeeksforGeeks: Static variables in C](https://www.geeksforgeeks.org/static-variables-in-c/)
- [GeeksforGeeks: Linked list data structure](https://www.geeksforgeeks.org/linked-list-data-structure/)
- [Valgrind Quick Start Guide](https://valgrind.org/docs/manual/quick-start.html), for finding memory leaks.
- The 42 Norm (subject PDF from the intranet).

### Use of AI

AI (Claude, through Claude Code) was used as a learning and support tool.
All the get_next_line code in this repository was written by me.

- **Test cases:** AI generated the tests I used to check my function. They
  are kept in a separate repository and are not part of the submitted
  project. The tests cover many `BUFFER_SIZE` values, empty files, files
  without a final `'\n'`, invalid fds, `malloc` and `read` failures, and
  reading several fds at once. Link to
  [github repo](https://github.com/SKsprite/get_next_line-Test)
- **Learning the concepts:** I asked AI to explain how `read` and static
  variables work.
- **README:** AI helped me plan the structure of this README and drafted this
  section. I reviewed and edited the final text.

I understand and can explain every line of the submitted code.
