*This project has been created as part of the 42 curriculum by stkoh.*

## Description

`ft_printf` is a recreation of the C library function `printf()`. It is
built as a static library, `libftprintf.a`, that other projects can link.

```c
int	ft_printf(const char *format, ...);
```

`ft_printf` writes `format` to the standard output. Every conversion
specification (a `%` followed by optional flags, width and precision, then a
conversion character) is replaced by the next argument, converted as asked.
It returns the number of bytes written, or `-1` if a write fails.

Conversions:

| Conversion | Prints |
|---|---|
| `%c` | a single character |
| `%s` | a string (`(null)` for a `NULL` pointer) |
| `%p` | a `void *` pointer, in hexadecimal |
| `%d`, `%i` | a signed decimal integer |
| `%u` | an unsigned decimal integer |
| `%x`, `%X` | an unsigned integer in hexadecimal, lowercase / uppercase |
| `%%` | a percent sign |

Flags (bonus part):

| Flag | Effect |
|---|---|
| `-` | left-justify within the field width |
| `0` | pad numbers with zeros instead of spaces |
| `.` | precision: the minimum number of digits for numbers, the maximum number of characters for `%s` |
| `#` | add `0x` / `0X` in front of a non-zero `%x` / `%X` |
| ` ` (space) | put a space in front of a positive signed number |
| `+` | always print the sign of a signed number |
| width | a number giving the minimum field width (`%5d`) |

Unlike the real `printf`, `ft_printf` has no buffer: it writes its output as
it goes, with `write(1, ...)`.

## Instructions

Requirements: `cc` and `make`. The library includes my `libft`, which is
built automatically from the `libft/` folder.

```sh
make          # builds libftprintf.a (libft first, then ft_printf)
make bonus    # same library, with the flags
make clean    # removes the object files (here and in libft/)
make fclean   # also removes libftprintf.a and libft.a
make re       # fclean, then make
```

To use it in a program, include the header and link the library:

```c
#include "ft_printf.h"

int	main(void)
{
	ft_printf("%s is %d years old (%p)\n", "Ada", 36, (void *)0x2a);
	return (0);
}
```

```sh
cc -Wall -Wextra -Werror main.c -L. -lftprintf -o main
./main
```

## Algorithm and data structure

**Main loop.** `ft_printf` walks the format string once, from left to right.
Plain text up to the next `%` is written in one `write()` call
(`print_literal`). At a `%`, `print_format` reads the conversion
specification and calls the handler for its conversion character. Every
call adds its byte count to the total. If any `write()` fails, `ft_printf`
stops and returns `-1`.

**Parsing into `t_parsed`.** Each conversion specification is read into a
`t_parsed` struct, reset before every `%`:

```c
typedef struct s_parsed
{
	t_flags	flags;          /* bitmask of FLAG_HASH, FLAG_MINUS, ... */
	int		has_width;
	int		width;
	int		has_precision;
	int		precision;
}	t_parsed;
```

The flags are bits in one `t_flags` value (`FLAG_HASH = 1 << 0`, …). Setting
a flag twice (`%--5d`) changes nothing, and a handler tests a flag with one
`&`. `has_width` and `has_precision` are separate from the values, because
`%.0d` (a precision of 0) is not the same as `%d` (no precision). The
parser (`parse_format_spec`) reads flags only before any width or
precision, then the width, then a `.` and the precision.

**Dispatch table.** The conversion character selects its handler from a
table of `{character, function}` pairs (`t_dispatch_table`, filled in
`get_handler`):

```c
{'c', print_c}, {'s', print_s}, {'p', print_p}, {'d', print_d}, ...
```

Every handler has the same signature,
`int handler(va_list *args, t_parsed *parsed)`. It reads its own argument,
writes its output and returns the bytes written. The table replaces a long
`if`/`else` chain. Each conversion is one small function, and supporting a
new conversion means adding one row.

**`va_list` by pointer.** The handlers receive `va_list *` instead of a
`va_list` copy. Each `va_arg` then moves the one shared argument list
forward, so the next conversion reads the next argument. Passing a
`va_list` by value to a function and using it after the call is undefined.

**Why no buffer.** The subject forbids reproducing `printf`'s buffer
management. Writing each piece as soon as it is ready keeps the output in
the right order with any other `write(1, ...)` the program makes.

## Format specifier notes

```
%[flags][width][.precision]conversion

flags       -  left align
            0  zero pad (numbers)
            #  adds 0x / 0X (hex)
           ' ' space in front of a positive number
            +  sign in front of a number
width       any number: minimum field width
.precision  numbers: pad with 0 up to the precision
            %s: maximum number of characters printed
```

## Resources

- `man 3 printf` and `man 3 stdarg` (`va_start`, `va_arg`, `va_copy`,
  `va_end`)
- The C standard, section 7.21.6.1 (the `fprintf` function), for the exact
  meaning of each flag, width and precision
- [cppreference: printf](https://en.cppreference.com/w/c/io/fprintf)
- The 42 `ft_printf` subject

**Use of AI.** AI (Claude, through Claude Code) was used to:

- build a separate test suite, `ft_printf-Test`, that compares
  `ft_printf` with the real `printf` byte by byte. It is not part of this
  repository.
- explain concepts while debugging, such as the difference between
  `#include "..."` and `#include <...>`.
- write this README from my notes and code.
