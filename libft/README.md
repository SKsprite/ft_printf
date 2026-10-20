*This project has been created as part of the 42 curriculum by stkoh.*

# Libft

My own C library: the standard functions recoded from scratch, plus extra string, output and linked-list helpers.

## Description

Libft is the first project of the 42 common core. The goal is to rewrite a set
of functions from the C standard library, plus other useful functions, and
package them into a static library, `libft.a`. Later 42 projects can then use
this library instead of libc.

The library has three parts:

| Part | Content | Functions |
|---|---|---|
| 1. Libc functions | Recoded versions of standard `<ctype.h>`, `<string.h>` and `<stdlib.h>` functions | 23 |
| 2. Additional functions | String functions not in libc (or in a different form) and output to a file descriptor | 11 |
| 3. Linked list | Functions to create and work with a singly linked list (`t_list`) | 9 |

Constraints of the project:
- Written in C and compiled with `cc -Wall -Wextra -Werror`.
- Follows the 42 Norm. No global variables.
- The only external functions used are `malloc`, `free` and `write`.
- Every function is prefixed with `ft_`, and all prototypes are in `libft.h`.

## Instructions

### Build

```sh
make        # compiles every ft_*.c into a .o and archives them into libft.a
make clean  # removes the .o files
make fclean # removes the .o files and libft.a
make re     # fclean, then make
```

### Use in another project

Include the header, then link with the library:

```c
#include "libft.h"

int	main(void)
{
	char	**words;
	int		i;

	words = ft_split("hello libft world", ' ');
	i = 0;
	while (words && words[i])
	{
		ft_putendl_fd(words[i], 1);
		free(words[i++]);
	}
	free(words);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror main.c -I path/to/libft -L path/to/libft -lft
./a.out
```

`-L` gives the folder that contains `libft.a`, and `-lft` links it.

## Library details

### Linked list structure

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;
```

- `content`: the data stored in the node. Because it is a `void *`, it can point to any type.
- `next`: the next node, or `NULL` if this is the last node. An empty list is just a `NULL` pointer.

### Part 1: libc functions

Each function behaves like the original libc function with the same name
(without `ft_`). `strlcpy`, `strlcat` and `strnstr` come from BSD (`libbsd` on Linux).

| Function | Description |
|---|---|
| `ft_isalpha(int c)` | Non-zero if `c` is a letter |
| `ft_isdigit(int c)` | Non-zero if `c` is `'0'`–`'9'` |
| `ft_isalnum(int c)` | Non-zero if `c` is a letter or a digit |
| `ft_isascii(int c)` | Non-zero if `c` is in 0–127 |
| `ft_isprint(int c)` | Non-zero if `c` is printable (32–126) |
| `ft_strlen(s)` | Length of `s`, not counting the `'\0'` |
| `ft_memset(s, c, n)` | Fills `n` bytes of `s` with the byte `c` |
| `ft_bzero(s, n)` | Sets `n` bytes of `s` to zero |
| `ft_memcpy(dest, src, n)` | Copies `n` bytes. The areas must not overlap |
| `ft_memmove(dest, src, n)` | Copies `n` bytes and handles overlapping areas |
| `ft_strlcpy(dst, src, size)` | Copies `src` into `dst` (at most `size - 1` chars), always ends with `'\0'` if `size > 0`. Returns `strlen(src)` |
| `ft_strlcat(dst, src, size)` | Appends `src` to `dst` within a buffer of `size` bytes. Returns the length of the string it tried to create |
| `ft_toupper(int c)` | Converts a lowercase letter to uppercase |
| `ft_tolower(int c)` | Converts an uppercase letter to lowercase |
| `ft_strchr(s, c)` | First occurrence of `c` in `s` (`c = '\0'` finds the terminator) |
| `ft_strrchr(s, c)` | Last occurrence of `c` in `s` |
| `ft_strncmp(s1, s2, n)` | Compares at most `n` chars, as unsigned chars |
| `ft_memchr(s, c, n)` | First byte equal to `c` in the first `n` bytes |
| `ft_memcmp(s1, s2, n)` | Compares `n` bytes, as unsigned chars |
| `ft_strnstr(big, little, len)` | Finds `little` in the first `len` chars of `big` |
| `ft_atoi(nptr)` | Converts a string to an `int` (skips whitespace, one optional sign) |
| `ft_calloc(nmemb, size)` | Allocates zeroed memory. Returns `NULL` if `nmemb * size` overflows |
| `ft_strdup(s)` | Returns a newly allocated copy of `s` |

### Part 2: additional functions

| Function | Description |
|---|---|
| `ft_substr(s, start, len)` | New string with at most `len` chars of `s`, from index `start`. Returns `""` if `start` is past the end |
| `ft_strjoin(s1, s2)` | New string made of `s1` followed by `s2` |
| `ft_strtrim(s1, set)` | New copy of `s1` with the chars in `set` removed from the start and the end |
| `ft_split(s, c)` | Splits `s` on the delimiter `c` into a `NULL`-terminated array of new strings. If an allocation fails, everything already allocated is freed and it returns `NULL` |
| `ft_itoa(n)` | New string with `n` in decimal (handles `INT_MIN`) |
| `ft_strmapi(s, f)` | New string where each char is `f(index, char)` |
| `ft_striteri(s, f)` | Calls `f(index, &char)` on each char of `s`, in place |
| `ft_putchar_fd(c, fd)` | Writes a char to `fd` |
| `ft_putstr_fd(s, fd)` | Writes a string to `fd` |
| `ft_putendl_fd(s, fd)` | Writes a string and then a newline to `fd` |
| `ft_putnbr_fd(n, fd)` | Writes an `int` in decimal to `fd` |

### Part 3: linked list

| Function | Description |
|---|---|
| `ft_lstnew(content)` | Allocates a new node with `content` and `next = NULL` |
| `ft_lstadd_front(lst, new)` | Adds `new` at the start of the list. `*lst` then points to `new` |
| `ft_lstsize(lst)` | Number of nodes (0 for an empty list) |
| `ft_lstlast(lst)` | Last node of the list (`NULL` for an empty list) |
| `ft_lstadd_back(lst, new)` | Adds `new` at the end. If the list is empty, `*lst` becomes `new` |
| `ft_lstdelone(lst, del)` | Calls `del` on the node's content and frees the node. The next node is not freed |
| `ft_lstclear(lst, del)` | Deletes and frees the node and all nodes after it, then sets `*lst` to `NULL` |
| `ft_lstiter(lst, f)` | Calls `f` on the content of each node |
| `ft_lstmap(lst, f, del)` | New list where each content is `f(content)`. If an allocation fails, the new list is cleared with `del` and it returns `NULL` |

## Resources

- Linux man pages, section 3: `man 3 strlcpy`, `man 3 memmove`, `man 3 atoi`, and so on.
- [libbsd man pages](https://manpages.debian.org/testing/libbsd-dev/strlcpy.3bsd.en.html), for `strlcpy`, `strlcat` and `strnstr`.
- Brian W. Kernighan and Dennis M. Ritchie, *The C Programming Language*, 2nd edition.
- [GeeksforGeeks: Linked list data structure](https://www.geeksforgeeks.org/linked-list-data-structure/)
- [Valgrind Quick Start Guide](https://valgrind.org/docs/manual/quick-start.html), for finding memory leaks.
- [GNU Make manual](https://www.gnu.org/software/make/manual/make.html)
- The 42 Norm (subject PDF from the intranet).

### Use of AI

AI (Claude, through Claude Code) was used as a learning and support tool.
All the library code in this repository was written by me.

- **Test cases:** AI generated the unit tests I used to check the library.
  They are kept in a separate folder (`libft Test`) and are not part of the
  submitted library. The tests include edge cases such as `NULL`/empty lists,
  checking that `del` and `free` are called correctly, and making `malloc`
  fail inside `ft_split` and `ft_lstmap`. I used these tests to find bugs in
  my own code. Link to [github repo](https://github.com/SKsprite/libft-Test)
- **Learning the functions:** I asked AI to explain how the original libc
  functions behave (for example `memmove` with overlapping memory, the
  return value of `strlcat`, and `size_t` versus `int`). I also asked what
  each linked-list function is expected to do.
- **Edge cases and debugging:** AI helped me understand compiler errors,
  crashes and failing tests, and explained the edge cases I had missed
  (`NULL` or empty input, allocation failure, off-by-one errors). I then
  fixed the code myself.
- **README:** AI helped me plan the structure of this README and drafted this
  section. I reviewed and edited the final text.

I understand and can explain every line of the submitted code.
