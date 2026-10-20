# Makefile, libft and get_next_line — notes

I tested everything below on a **copy** of the project in a scratch folder,
using `cc` (clang 12), `gcc` and norminette 3.3.59. Nothing in this repo was
changed.

---

## 1. What's wrong with the current Makefile

### 1.1 `LIBFT = -Lft` does nothing (and is wrong three ways)

```make
LIBFT = -Lft
```

- The variable is never used in any rule.
- `-L<dir>` tells the **linker** which folder to search. `ar` is not a linker,
  so `-L` means nothing when you build a `.a`.
- `-Lft` names a folder called `ft`. Your folder is `libft`.

A static library (`.a`) is just an archive of `.o` files. It never links
against another library. When the grader compiles
`cc main.c libftprintf.a`, every symbol you use (`ft_strlen`, `ft_itoa`, and so
on) must already be **inside** `libftprintf.a`. That means the libft objects
have to be copied into it (see §2).

### 1.2 libft is never built, and clean/fclean skip it

Nothing runs `make` inside `libft/`, so `libft/libft.a` never exists. Likewise,
`clean` and `fclean` leave libft's `.o` files and `libft.a` behind.

### 1.3 The compiler can't find `libft.h`

`ft_printf.h` now has `# include "libft.h"`, but `libft.h` is in `libft/`.
Running the current compile rule gives:

```
./ft_printf.h:18:11: fatal error: 'libft.h' file not found
```

Add `-Ilibft` to `CFLAGS` so the compiler also searches that folder.

### 1.4 `SRC` lists files that don't exist yet

`handler.c`, `handle_unsigned.c`, `handle_signed.c` and `format_spec.c` are
listed, so `make` stops with `No rule to make target 'handler.c'` until you
create them. That's fine if they're on the way. Just remember the build is
broken until then.

### 1.5 Bonus variables are dead and use a wildcard

```make
BONUS_FILE = *_bonus.c
BONUS_OBJ = ${BONUS_FILE:.c=.o}
BONUS_HEADER = ft_printf_bonus.h
```

- A variable assignment never expands `*`, so `BONUS_OBJ` is literally the
  string `*_bonus.o`.
- None of these variables are used. `bonus: $(NAME)` just builds the mandatory
  part.
- Most 42 evaluations reject wildcards in Makefiles. List files explicitly.

If your bonus (flags, width, precision) lives in the mandatory files, which is
what your `t_parsed` struct suggests, simply delete these three lines and keep
`bonus: all`.

### 1.6 libft isn't actually in your git repo (the most important one)

```
$ cat .gitignore
.gitignore
libft/*
libft
```

- `.gitignore` ignores `libft`, so **libft is never committed or pushed**. The
  evaluator's `make` would fail immediately because there's no `libft/` folder.
- `libft/` also contains its own `.git` folder (it's a separate clone).
  Even after you remove it from `.gitignore`, `git add libft` would only
  record a pointer to the nested repo (a "gitlink"), not the files. Delete
  `libft/.git` first so the files get added normally.
- `libftprintf.a` is currently untracked but **not** ignored, so it could get
  committed by accident. That's the line `.gitignore` should contain instead:

```gitignore
*.o
*.a
```

---

## 2. Including libft (suggested Makefile)

The usual 42 approach is to build `libft.a`, **copy** it as the starting point
for `libftprintf.a`, and then add your own objects to it.

```make
NAME = libftprintf.a

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = ft_printf.c\
	  handler.c\
	  handle_unsigned.c\
	  handle_signed.c\
	  format_spec.c
OBJ = $(SRC:.c=.o)
HEADER = ft_printf.h

CC = cc
CFLAGS = -Wall -Werror -Wextra -I$(LIBFT_DIR)
AR = ar rcs
RMF = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	cp $(LIBFT) $(NAME)
	$(AR) $(NAME) $(OBJ)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

bonus: all

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	$(RMF) $(OBJ)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	$(RMF) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re
```

Line by line:

| Line | Why |
|---|---|
| `-I$(LIBFT_DIR)` | lets `#include "libft.h"` resolve |
| `$(LIBFT):` + `$(MAKE) -C $(LIBFT_DIR)` | runs libft's own Makefile. Use `$(MAKE)`, not plain `make`, so flags like `-s` and `-j` are passed through |
| `cp $(LIBFT) $(NAME)` then `ar rcs $(NAME) $(OBJ)` | `libftprintf.a` = all of libft's objects + yours |
| `$(NAME): $(LIBFT) $(OBJ)` | rebuilds the archive if either side changed |

I verified this with `SRC = ft_printf.c`. `nm libftprintf.a` showed
`ft_printf`, `ft_strlen` and `get_next_line` all inside. A second `make`
printed `Nothing to be done for 'all'`, so there's no relinking, and `fclean`
removed every build file in both folders.

> Small catch: `$(LIBFT)` is only rebuilt when `libft.a` is missing. If you
> edit a libft `.c` file, run `make re` (or `make -C libft`) yourself. Fixing
> this properly takes more Makefile machinery than the project needs.

---

## 3. Adding get_next_line to libft with minimal changes

### 3.1 The problem: `ft_strlen` is defined twice

Your `get_next_line_utils.c` (and `_utils_bonus.c`) has its own `ft_strlen`.
I copied the GNL files into libft and added them to libft's `SRC`. The archive
builds, but **linking any program that uses both fails**:

```
libft.a(get_next_line_utils.o): multiple definition of `ft_strlen';
libft.a(ft_strlen.o): first defined here
```

### 3.2 The fix (4 small steps)

1. **Copy only one version** into `libft/`: either the mandatory files
   (`get_next_line.c`, `get_next_line_utils.c`, `get_next_line.h`) or the bonus
   files. Not both, because they define the same functions. The bonus version
   handles several fds at once, so it's more useful later. If you choose it,
   you can keep the `_bonus` filenames; nothing requires renaming.

2. In `get_next_line_utils.c`, **delete the `ft_strlen` function** (the first
   10 lines after the `#include`). Libft already provides it.

3. In `get_next_line.h`, **delete the `ft_strlen` prototype** and include libft
   instead:

   ```c
   # include <unistd.h>
   # include <stdlib.h>
   # include "libft.h"
   ```

4. In `libft/Makefile`, add the two `.c` files to `SRC` and the header to
   `HEADER`:

   ```make
   HEADER = libft.h get_next_line.h

   SRC = ...\
   	  ft_lstmap.c\
   	  get_next_line.c\
   	  get_next_line_utils.c
   ```

After this, libft builds, norminette passes on all the GNL files, and a test
program that calls both `get_next_line` and `ft_strlen` links and runs
correctly.

### 3.3 Optional extras

- If you want `#include "libft.h"` alone to give you `get_next_line`, add
  `# include "get_next_line.h"` near the bottom of `libft.h`, **before** the
  final `#endif`. That's safe because both headers have include guards.
- `BUFFER_SIZE` falls back to 4096 through `#ifndef`, so libft needs no
  `-D BUFFER_SIZE` flag.
- GNL's helper functions (`find_char`, `char_join`, `extract_line`,
  `read_and_append`, `malloc_buff`) are global names. They don't clash with
  anything today. If a future project ever defines the same name, make them
  `static` or prefix them with `gnl_`.
- Don't add GNL to **this** project's libft unless you want it. ft_printf
  doesn't use it, and copying it in only adds files for the evaluator to read.
  Keep the GNL-enabled libft as your "master" copy for later projects.
