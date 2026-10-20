# ft_printf: your design vs. your code, and a DRY rewrite

This document has three parts:

1. how the current code differs from the logic in your README,
2. the bugs that come out of that difference (all confirmed by running the
   code),
3. a rewrite that follows your logic exactly: per-conversion differences in
   **one table**, and every flag rule in **one function**.

I built the rewrite in a scratch folder and ran it through your
`ft_printf-Test`. Results are in §6. None of your files were changed.

---

## 1. Your idea

```
my_printf
    print literal text
    parser(&curr)
        parse specification
        advance curr
            pass specification
            count = formatter(...)
        return count
    total_count += returned count
loop
```

The idea is a **pipeline** with three separate steps:

```
"%-08.3x"  ──parse──▶  spec {flags, width, precision, conv}  ──format──▶  output
```

- **Parse** reads the whole specification in one go and moves `curr` past it.
  It doesn't print anything.
- **Format** receives a finished spec and prints. It doesn't read the format
  string.
- The **loop** only alternates between literal text and conversions, and adds
  up the counts.

## 2. What the code does instead

### 2.1 The top loop: matches the idea

`ft_printf` and `print_literal` already follow the design: literal chunk or
conversion, add the count, stop on `-1`. Keep them.

### 2.2 The parser: mixed in with dispatch, one character at a time

`print_format` ([ft_printf.c](ft_printf.c)) does this:

```c
while (str[*i] && !func)
{
    func = get_handler(str[*i]);      // is this char a conversion?
    if (!func)
    {
        parse_format_spec(str, args, inpt, i);   // no: feed ONE char to the parser
        (*i)++;
    }
}
```

So there's no "parse the specification" step. The dispatcher drives the
parser one character at a time. `parse_format_spec` only ever sees one
character, so it has to work out where it is from flags (`has_width`,
`has_precision`). Moving `curr` forward is split between three places:
`print_format`, `parse_format_spec` (which does `(*i)++` on `.`), and the
`(*i)++` after the loop.

That causes most of the bugs below:

- **The `.` never reaches the precision branch.** When `has_precision` is 0,
  every character that isn't a flag goes into the `!parsed->has_precision`
  (width) branch, `.` included. The `else if (c[*i] == '.')` branch can only
  run once `has_precision` is already 1, so precision is never set.
- **Width digits are never turned into a number.** Only `*` sets `width`.
  For `%5d`, the parser sets `has_width = 1` and leaves `width = 0`.
- **Unknown characters are skipped silently**, and the loop keeps going until
  it finds *any* conversion character. In `"%y %d"`, the `y` and the space get
  swallowed, the second `%` is printed as a `%%` conversion, and then `d`
  comes out as plain text.
- `int j` in `parse_format_spec` is set but never used. gcc's `-Wall` rejects
  that (`-Wunused-but-set-variable`); your clang 12 doesn't yet.

### 2.3 The formatters: flags parsed but never used, logic copied 4 times

| Problem | Where |
|---|---|
| `print_d` and `print_i` are identical, line for line | [handle_signed.c](handle_signed.c) |
| `print_u` is a third copy of the same code | [handle_unsigned.c](handle_unsigned.c) |
| No handler looks at `width`, `-`, `0`, `+`, space or `#` | all handlers |
| `if (!num) write "0"` skips precision, so `%.0d` with 0 prints `0` (should print nothing) | d, i, u |
| `print_c`'s `if (!num)` branch does the same as the `else` | `print_c` |
| `%u` reads `unsigned long` but receives `unsigned int`; `ft_itoa` takes `int`, so 4294967295 prints `-1` | `print_u` |
| `print_s` with a precision larger than the string calls `write` past its end | `print_s` |
| `num_with_precision`: `<=` writes one zero too many; with `-42`, `len--` makes `write(itoaed, len)` print `-4` (the `-` lands after the zeros and the last digit is lost) | [format_spec.c](format_spec.c) |
| `print_x`, `print_u_x` are stubs; `print_p` doesn't compile | handle_unsigned.c |
| `ft_itoa` + `free` on every number: a `malloc` that can fail, just to print digits | d, i, u |

### 2.4 What running it shows

I compiled the current code on a scratch copy, with `print_p` stubbed out so
it builds, and compared it with `printf`:

| Call | printf | yours |
|---|---|---|
| `"%5d", 42` | `   42` | `42` |
| `"%-5d\|", 42` | `42   \|` | `42\|` |
| `"%.5d", 42` | `00042` | `42` |
| `"%05d", 42` | `00042` | `42` |
| `"%+d", 42` | `+42` | `42` |
| `"%.0d", 0` | *(nothing)* | `0` |
| `"%.2s", "hello"` | `he` | `hello` |
| `"%u", 4294967295u` | `4294967295` | `-1` |
| `"%x", 255` | `ff` | *(nothing)* |
| `"%y %d", 7` | `%y 7` | `%d` |

The header is fine. The structure is what keeps width and precision from
working.

---

## 3. The redesign

### 3.1 Two principles

**DRY** means every piece of knowledge lives in exactly one place. For
ft_printf, "knowledge" is two things:

1. **What differs between conversions**: the argument type, the base, `0x`
   vs `0X`, whether `+` applies. That's *data*, so it goes in **one table**.
   Conversion letters appear nowhere else in the code.
2. **Each printf rule**, such as "`-` beats `0`" or "`+` beats space". Each
   rule is written **once**, in the function that uses it, as the **order of an
   if/else chain**. The rule that wins is the one checked first.

**Modular** means each stage has one job and passes a finished struct to the
next. No stage reaches back into an earlier one or changes the earlier
stage's struct.

```
 format string ─▶ parse_spec ─▶ t_spec ─▶ fetch_arg ─▶ t_arg ─▶ build_field ─▶ t_field ─▶ print_field ─▶ stdout
                  (what was     (what's    (va_arg,     (the     (sign, 0x,     (what to    (width
                   written)      written)   once)        value)   digits,        print)      padding)
                                                                  precision)
```

| Stage | Its only job | Knows about |
|---|---|---|
| `parse_spec` | read `%[flags][width][.prec]conv` and move `curr` | the format string |
| `find_conv` | look up the conversion's row | the table |
| `fetch_arg` | call `va_arg` with the type from the row | `va_list` |
| `build_field` | turn the value into sign, `0x`, digits, precision zeros | flags `+ space #`, precision |
| `print_field` | add width padding and write | flags `- 0`, width |

### 3.2 Why "clear the flag" was the wrong way (what changed from my first draft)

My first draft did this in the parser:

```c
if (spec->flags & FLAG_MINUS)
	spec->flags &= ~FLAG_ZERO;          /* '-' beats '0' */
if (spec->flags & FLAG_PLUS)
	spec->flags &= ~FLAG_SPACE;         /* '+' beats ' ' */
```

plus `spec->flags &= ~FLAG_ZERO` again in `put_number` (precision cancels
`0`) and in `put_text` (`0` doesn't pad strings). It gave the right output,
but you were right that it wasn't DRY:

- **One rule was split across three files.** "When does `0` pad?" was
  partly in the parser, partly in `put_number` and partly in `put_text`. To
  understand zero padding you had to read all three.
- **The parser was making formatting decisions.** It deleted information
  ("the user wrote `0`") that only the printer should interpret.
- **Later stages changed their input.** `put_number` edited the spec it was
  given, so the same spec could print differently depending on which function
  saw it first.
- **The sign chain existed twice** (`conv_signed` and `conv_ptr`). So did
  the `0x` logic (`conv_unsigned` and `conv_ptr`). Conversion letters were
  tested in three files (`'X'`, `'u'`, `'%'`).

The C standard states these rules as **priorities**. For example: "if the
`0` and `-` flags both appear, the `0` flag is ignored." A priority is
exactly what an if/else chain expresses:

```c
if (s->flags & FLAG_MINUS)   /* checked first → '-' wins */
	return ('-');
if (s->flags & FLAG_ZERO)
	return ('0');
return (' ');
```

Nothing gets cleared and nothing gets changed. The parser just records what
was written, and the rule lives in exactly one place.

### 3.3 The data model (ft_printf.h)

```c
/* Bit n of flags is the n-th character of FLAG_CHARS. */
# define FLAG_CHARS "#-+0 "

typedef enum e_flags
{
	FLAG_HASH = 1 << 0,     /* '#' */
	FLAG_MINUS = 1 << 1,    /* '-' */
	FLAG_PLUS = 1 << 2,     /* '+' */
	FLAG_ZERO = 1 << 3,     /* '0' */
	FLAG_SPACE = 1 << 4     /* ' ' */
}	t_flags;

/* Which va_arg type a conversion reads. */
typedef enum e_argtype
{
	ARG_NONE,
	ARG_CHAR,
	ARG_STR,
	ARG_PTR,
	ARG_INT,
	ARG_UINT
}	t_argtype;

/* What a conversion allows. */
typedef enum e_props
{
	P_SIGN = 1 << 0,        /* '+' and ' ' apply */
	P_ALT_ALWAYS = 1 << 1,  /* the 0x prefix is always printed (p) */
	P_NOPAD = 1 << 2        /* width is ignored (%%) */
}	t_props;

/* One row of the conversion table: everything that differs per letter. */
typedef struct s_conv
{
	char		c;
	t_argtype	type;
	const char	*base;      /* digits; NULL = a text conversion */
	const char	*alt;       /* what '#' adds: "0x", "0X" or "" */
	int			props;
}	t_conv;

/* The parsed specification: exactly what was written, nothing decided. */
typedef struct s_spec
{
	int		flags;
	int		width;          /* 0 = none */
	int		precision;      /* -1 = none */
	t_conv	conv;
}	t_spec;

/* The argument, fetched: a number (str == NULL) or text. */
typedef struct s_arg
{
	t_ul		num;        /* absolute value */
	int			neg;
	const char	*str;
	int			len;
	char		chr;        /* storage for %c and %% */
}	t_arg;

/* What will be printed, in order, before width padding. */
typedef struct s_field
{
	const char	*sign;      /* "", "-", "+", " " */
	const char	*alt;       /* "", "0x", "0X" */
	int			zeros;      /* from precision */
	const char	*body;
	int			len;
	int			numeric;
	char		digits[DIGITS_MAX];
}	t_field;
```

Each struct is the *output* of one stage and the *input* of the next.

### 3.4 The table (conv_table.c): the only place that knows the letters

```c
static void	init_table(t_conv *t)
{
	t[0] = (t_conv){'c', ARG_CHAR, NULL, "", 0};
	t[1] = (t_conv){'s', ARG_STR, NULL, "", 0};
	t[2] = (t_conv){'%', ARG_NONE, NULL, "", P_NOPAD};
	t[3] = (t_conv){'p', ARG_PTR, HEX_LO, "0x", P_SIGN | P_ALT_ALWAYS};
	t[4] = (t_conv){'d', ARG_INT, DEC, "", P_SIGN};
	t[5] = (t_conv){'i', ARG_INT, DEC, "", P_SIGN};
	t[6] = (t_conv){'u', ARG_UINT, DEC, "", 0};
	t[7] = (t_conv){'x', ARG_UINT, HEX_LO, "0x", 0};
	t[8] = (t_conv){'X', ARG_UINT, HEX_UP, "0X", 0};
	t[9] = (t_conv){0, ARG_NONE, NULL, "", 0};
}

int	find_conv(char c, t_conv *out)
{
	t_conv	table[10];
	int		i;

	init_table(table);
	i = 0;
	while (table[i].c && table[i].c != c)
		i++;
	*out = table[i];
	return (table[i].c != 0);
}
```

- **One table row replaces a whole handler.** The differences between `d`,
  `u`, `x` and `X` are now four values in a row, not four functions.
- **`find_conv` copies the row into the spec**, because `table` lives on the
  stack and disappears when the function returns. That's still without
  globals, as you wanted.
- **Adding a conversion means adding a row.** I tested this:
  `{'b', ARG_UINT, "01", "0b", 0}` (and making the array one bigger) gives a
  working binary `%b`. `ft_printf("[%#10b] [%-6.4b] [%08b]", 5, 5, 5)` prints
  `[     0b101] [0101  ] [00000101]`. Width, `#`, `-`, precision and `0` work
  with no other code changes.
- `P_SIGN` on `p` and `P_NOPAD` on `%` copy glibc behaviour that your tester
  checks: `%+p` prints `+0x…`, and `%5%` prints just `%`.

### 3.5 The loop (ft_printf.c): your logic, line for line

`ft_printf` and `print_literal` are the same as in the first draft (and close
to yours). `print_conversion` is your `parser(&curr)`:

```c
static int	print_conversion(const char *fmt, int *i, va_list *ap)
{
	t_spec	spec;
	t_arg	arg;
	t_field	field;
	int		start;

	(*i)++;
	start = *i;
	if (!parse_spec(fmt, i, &spec))        /* parse specification, advance curr */
	{
		if (!fmt[*i])
			return (-1);                   /* '%' at the very end */
		*i = start;
		return (write(1, "%", 1));         /* unknown: print '%', rest is literal */
	}
	fetch_arg(ap, &spec, &arg);            /* pass specification ... */
	build_field(&spec, &arg, &field);
	return (print_field(&spec, &field));   /* ... count = formatter(...) */
}
```

### 3.6 The parser (parse.c): records, never decides

```c
static int	set_flag(char c, t_spec *spec)
{
	const char	*found;

	found = ft_strchr(FLAG_CHARS, c);
	if (!c || !found)
		return (0);
	spec->flags |= 1 << (found - FLAG_CHARS);
	return (1);
}

static int	parse_number(const char *fmt, int *i)
{
	int	n;

	n = 0;
	while (ft_isdigit(fmt[*i]))
	{
		n = n * 10 + (fmt[*i] - '0');
		(*i)++;
	}
	return (n);
}

int	parse_spec(const char *fmt, int *i, t_spec *spec)
{
	spec->flags = 0;
	while (set_flag(fmt[*i], spec))
		(*i)++;
	spec->width = parse_number(fmt, i);
	spec->precision = -1;
	if (fmt[*i] == '.')
	{
		(*i)++;
		spec->precision = parse_number(fmt, i);
	}
	if (!find_conv(fmt[*i], &spec->conv))
		return (0);
	(*i)++;
	return (1);
}
```

- **`set_flag` is now a lookup, not five `if`s.** The flag's position in
  `"#-+0 "` *is* its bit, so the character-to-flag mapping is written once,
  next to the enum in the header. (`!c` is needed because
  `ft_strchr(s, '\0')` finds the string's terminator.)
- **`parse_spec` has no rule logic at all.** It's the grammar
  `%[flags][width][.precision]conv`, read in that order.

### 3.7 Fetching (fetch.c): the only `va_arg` calls

```c
static void	fetch_text(va_list *ap, const t_spec *s, t_arg *a)
{
	if (s->conv.type == ARG_STR)
	{
		a->str = va_arg(*ap, const char *);
		if (!a->str && s->precision >= 0 && s->precision < 6)
			a->str = "";                    /* glibc: "%.3s" of NULL */
		else if (!a->str)
			a->str = "(null)";
		a->len = 0;
		while (a->str[a->len] && (s->precision < 0 || a->len < s->precision))
			a->len++;
		return ;
	}
	a->chr = s->conv.c;                     /* %% prints its own letter */
	if (s->conv.type == ARG_CHAR)
		a->chr = (char)va_arg(*ap, int);    /* char is promoted to int */
	a->str = &a->chr;
	a->len = 1;
}

void	fetch_arg(va_list *ap, const t_spec *s, t_arg *a)
{
	long	n;

	a->num = 0;
	a->neg = 0;
	a->str = NULL;
	if (!s->conv.base)
		fetch_text(ap, s, a);
	else if (s->conv.type == ARG_PTR)
		a->num = (t_ul)va_arg(*ap, void *);
	else if (s->conv.type == ARG_UINT)
		a->num = va_arg(*ap, t_uint);
	else
	{
		n = va_arg(*ap, int);
		a->neg = (n < 0);
		if (n < 0)
			n = -n;                         /* long: safe for INT_MIN */
		a->num = n;
	}
	if (s->conv.type == ARG_PTR && a->num == 0)
	{
		a->str = "(nil)";                   /* NULL %p prints as text */
		a->len = 5;
	}
}
```

- **`va_arg` needs a type known at compile time,** so a chain of branches is
  unavoidable somewhere. Here it's the only one, and it branches on the
  *type from the table*, not on letters.
- **The string length stops at the precision.** The standard says that with
  a precision, a `%s` argument doesn't even need a `'\0'`. That also replaces
  your `write(1, string, precision)`, which could read past the end.
- **After this stage, a value is either a number or text.** `(nil)` becomes
  text, so nothing later has to know about pointers.

### 3.8 Building the field (field.c): sign, `#` and precision, each once

```c
/* The sign rule: '-' beats '+', '+' beats ' '. The first match wins. */
static const char	*sign_of(const t_spec *s, const t_arg *a)
{
	if (a->neg)
		return ("-");
	if (!(s->conv.props & P_SIGN))
		return ("");
	if (s->flags & FLAG_PLUS)
		return ("+");
	if (s->flags & FLAG_SPACE)
		return (" ");
	return ("");
}

/* The '#' rule: 0x / 0X, never for 0 (always for p). */
static const char	*alt_of(const t_spec *s, const t_arg *a)
{
	if (s->conv.props & P_ALT_ALWAYS)
		return (s->conv.alt);
	if ((s->flags & FLAG_HASH) && a->num != 0)
		return (s->conv.alt);
	return ("");
}

static void	make_digits(t_field *f, t_ul n, const char *base)
{
	t_ul	radix;
	int		i;

	radix = ft_strlen(base);
	i = DIGITS_MAX;
	f->digits[--i] = base[n % radix];
	while (n >= radix)
	{
		n /= radix;
		f->digits[--i] = base[n % radix];
	}
	f->body = f->digits + i;
	f->len = DIGITS_MAX - i;
}

/* The precision rule for numbers: at least that many digits; .0 of 0 is
** no digits at all. */
void	build_field(const t_spec *s, const t_arg *a, t_field *f)
{
	f->sign = "";
	f->alt = "";
	f->zeros = 0;
	f->numeric = (a->str == NULL);
	if (!f->numeric)
	{
		f->body = a->str;
		f->len = a->len;
		return ;
	}
	f->sign = sign_of(s, a);
	f->alt = alt_of(s, a);
	make_digits(f, a->num, s->conv.base);
	if (s->precision == 0 && a->num == 0)
		f->len = 0;
	if (s->precision > f->len)
		f->zeros = s->precision - f->len;
}
```

- **"`+` beats space" is the order of two `if`s in `sign_of`.** Nothing is
  cleared.
- **`#` for `d` or `u` does nothing on its own,** because their `alt` in the
  table is `""`. No special case is needed.
- **The sign and the `0x` are separate fields.** That's how `%+p` gives
  `+0x…` and `%08x` with `#` gives `0x0000ff`: the order is fixed in one
  place, `print_field`.
- **`make_digits` is the only digit converter,** for every base. It uses a
  stack buffer, so there's no `ft_itoa`, no `malloc` and no `free`.

### 3.9 Printing (print.c): width and `-`/`0`, once

```c
static void	emit(int *count, const char *s, int n)
{
	if (*count < 0 || n <= 0)
		return ;
	if (write(1, s, n) != n)
		*count = -1;
	else
		*count += n;
}

static void	emit_repeat(int *count, char c, int n)
{
	while (n > 0)
	{
		emit(count, &c, 1);
		n--;
	}
}

/*
** The padding rule, in order of priority (the first match wins):
** no width at all for P_NOPAD; '-' pads on the right; '0' pads with zeros,
** but only numbers without a precision; otherwise spaces on the left.
*/
static char	pad_style(const t_spec *s, const t_field *f)
{
	if (s->conv.props & P_NOPAD)
		return (0);
	if (s->flags & FLAG_MINUS)
		return ('-');
	if ((s->flags & FLAG_ZERO) && f->numeric && s->precision < 0)
		return ('0');
	return (' ');
}

/* [spaces] sign alt [zeros: '0' pad] [zeros: precision] body [spaces] */
int	print_field(const t_spec *s, const t_field *f)
{
	int		count;
	int		pad;
	char	style;

	style = pad_style(s, f);
	pad = s->width - f->zeros - f->len;
	pad -= ft_strlen(f->sign) + ft_strlen(f->alt);
	if (!style)
		pad = 0;
	count = 0;
	if (style == ' ')
		emit_repeat(&count, ' ', pad);
	emit(&count, f->sign, ft_strlen(f->sign));
	emit(&count, f->alt, ft_strlen(f->alt));
	if (style == '0')
		emit_repeat(&count, '0', pad);
	emit_repeat(&count, '0', f->zeros);
	emit(&count, f->body, f->len);
	if (style == '-')
		emit_repeat(&count, ' ', pad);
	return (count);
}
```

- **Every zero-padding rule is in `pad_style`, in priority order:** `-`
  beats `0`, precision cancels `0`, and `0` only applies to numbers. This is
  the code you asked about. It's four lines, and it's the only place that
  answers "how do we pad?".
- **`emit` handles `write` errors in one place.** After one failure, `count`
  stays `-1`.

### 3.10 Where every rule lives

| Rule | The one place |
|---|---|
| which characters are flags, and which bit each sets | `FLAG_CHARS` + `t_flags` (header) |
| everything that differs per conversion | `init_table` |
| the `%[flags][width][.prec]conv` grammar | `parse_spec` |
| which `va_arg` type to read | `fetch_arg` (driven by `t_argtype`) |
| precision limits `%s`; `(null)`, `(nil)` | `fetch_text`, `fetch_arg` |
| `-` sign, then `+`, then space | `sign_of` |
| `#` gives `0x`/`0X`, not for 0 | `alt_of` |
| precision = minimum digits, `.0` of 0 = nothing | `build_field` |
| `-` beats `0`, precision cancels `0`, `0` only for numbers, `%%` has no width | `pad_style` |
| output order and width | `print_field` |
| `write` error handling | `emit` |

---

## 4. File layout (Norm-compliant)

| File | Functions | Stage |
|---|---|---|
| `ft_printf.c` | `ft_printf`, `print_literal`, `print_conversion` | the loop |
| `parse.c` | `set_flag`, `parse_number`, `parse_spec` | text → `t_spec` |
| `conv_table.c` | `init_table`, `find_conv` | letter → `t_conv` |
| `fetch.c` | `fetch_text`, `fetch_arg` | `va_list` → `t_arg` |
| `field.c` | `sign_of`, `alt_of`, `make_digits`, `build_field` | `t_arg` → `t_field` |
| `print.c` | `emit`, `emit_repeat`, `pad_style`, `print_field` | `t_field` → output |

Every function is `static` except the six declared in the header:
`ft_printf`, `find_conv`, `parse_spec`, `fetch_arg`, `build_field` and
`print_field`. That's one public entry point per stage.

It passes norminette 3.3.59 on all seven files. Each file compiles on its
own with `cc` and `gcc -Wall -Wextra -Werror`, with no `-I`.

## 5. Migrating from your current files

1. **Header:** add `FLAG_CHARS`, `t_argtype`, `t_props`, `t_conv`, `t_spec`,
   `t_arg` and `t_field`. Remove `t_parsed`, `t_types`, `t_dispatch_table`,
   `t_handler` and the `print_*` prototypes.
2. **conv_table.c:** your `init_table`/`get_handler` from `handler.c`, with
   `t_conv` rows instead of function pointers.
3. **parse.c:** replaces `format_spec.c`. Your `set_flag` becomes the
   `FLAG_CHARS` lookup.
4. **fetch.c, field.c, print.c:** new. They replace `handle_signed.c`,
   `handle_unsigned.c` and `num_with_precision`.
5. **ft_printf.c:** replace `print_format` with `print_conversion`, and
   remove `reset_parsed`.
6. **Makefile:**
   `SRC = ft_printf.c parse.c conv_table.c fetch.c field.c print.c`.

## 6. Test results

Using your `ft_printf-Test` on a scratch copy with exactly this code:

| Suite | Result |
|---|---|
| mandatory | **37/38**. The one failure is `%z`, which glibc treats as a length modifier (undefined in C) |
| bonus (every flag × width × precision sweep for `c s p d i u x X %`) | **20/21**. The one failure is "flags in the wrong order" (`%.-5d`, `%-5.k`): undefined |
| valgrind (mandatory + bonus) | no memory errors, and no `malloc` anywhere |

These are the same results as my first draft. The output didn't change, only
where each rule lives.

## 7. One thing outside ft_printf: `read` in libftprintf.a

Your `libft/` now includes get_next_line, so `libftprintf.a` contains
`get_next_line.o`, and that calls `read`. The tester reports:

```
[KO] undefined in libftprintf.a: read (allowed: malloc free write)
```

ft_printf never calls `get_next_line`, but the function and its `read` are
inside the archive. Evaluators who run `nm` will see it. For this project,
use a libft without get_next_line (your `master` branch). Alternatively,
after `cp $(LIBFT) $(NAME)`, remove those two objects from the archive with
`ar d $(NAME) get_next_line.o get_next_line_utils.o`.
