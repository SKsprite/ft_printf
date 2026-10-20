# Dispatch table without globals — notes

Checked with norminette 3.3.59 and `cc -Wall -Wextra -Werror`.

## 1. Why the current `is_type` fails norminette

```c
t_types	types[] = {
	TYPE_C, TYPE_S, ...
};
```

```
Error: DECL_ASSIGN_LINE (line: 17): Declaration and assignation on a single line
```

The Norm forbids declaring **and** initialising a local variable in one
statement. It makes an exception only for `static` and `const` variables.

The same function also has a hidden compiler problem:
`i < sizeof(types) / sizeof(types[0])` compares an `int` with a `size_t`,
which `-Wextra` reports as `-Wsign-compare`, and `-Werror` turns that into a
build failure.

## 2. Why you can't simply loop over `t_types`

```c
TYPE_C = 'c', TYPE_S = 's', TYPE_P = 'p', ... TYPE_PERC = '%'
```

The enum values are ASCII codes, so they are **not contiguous**. A loop such as
`for (t = TYPE_C; t <= TYPE_U_X; t++)` would run through every character in
between, and `'%'` (37) is lower than `'X'` (88). In C, an enum gives you no
way to iterate over its members. You always loop over an **array** that lists
them, so the real question is how to build that array without a global.

There are two norm-friendly ways to do it.

---

## Option A: fill the table in a helper (no globals, no statics) — recommended

The table lives on the caller's stack. A small function fills it one entry at
a time, and the last entry is a `{0, 0}` sentinel that ends the loop.

```c
static void	init_table(t_dispatch_table *table)
{
	table[0] = (t_dispatch_table){TYPE_C, print_c};
	table[1] = (t_dispatch_table){TYPE_S, print_s};
	table[2] = (t_dispatch_table){TYPE_P, print_p};
	table[3] = (t_dispatch_table){TYPE_D, print_d};
	table[4] = (t_dispatch_table){TYPE_I, print_d};
	table[5] = (t_dispatch_table){TYPE_U, print_u};
	table[6] = (t_dispatch_table){TYPE_X, print_x};
	table[7] = (t_dispatch_table){TYPE_U_X, print_x};
	table[8] = (t_dispatch_table){TYPE_PERC, print_perc};
	table[9] = (t_dispatch_table){0, 0};
}

static t_handler	get_handler(char c)
{
	t_dispatch_table	table[10];
	int					i;

	init_table(table);
	i = 0;
	while (table[i].type)
	{
		if ((char)table[i].type == c)
			return (table[i].func);
		i++;
	}
	return (0);
}
```

- `table[0] = (t_dispatch_table){...};` is an **assignment**, not an
  initialisation, so `DECL_ASSIGN_LINE` doesn't trigger. That's why it passes.
- If you prefer to avoid compound literals, assigning one field at a time also
  passes: `table[0].type = TYPE_C; table[0].func = print_c;`. That makes 20
  lines, which still fits under the 25-line limit.
- The sentinel means you don't need `sizeof` or a count, so the
  `int`/`size_t` warning goes away.
- The array is rebuilt on every `%` conversion. That costs about 10 stores,
  which is negligible.

## Option B: `static const` table inside the function

```c
static t_handler	get_handler(char c)
{
	static const t_dispatch_table	table[] = {
	{TYPE_C, print_c},
	{TYPE_S, print_s},
	/* ... */
	{0, 0}
	};
	int								i;

	i = 0;
	while (table[i].type)
	{
		if ((char)table[i].type == c)
			return (table[i].func);
		i++;
	}
	return (0);
}
```

This passes norminette because `static` and `const` are exempt from
`DECL_ASSIGN_LINE`. However, a `static` local has the same lifetime as a
global, and some evaluators treat it as one under "global variables are
forbidden". **Option A avoids that argument entirely.**

---

## 3. `is_type` becomes free

Once you have `get_handler`, a separate list of types isn't needed:

```c
if (get_handler(c))   /* c is a valid conversion */
```

If you still want a standalone check, loop over a string literal. Assigning a
pointer on its own line is fine for the Norm:

```c
static int	is_type(char c)
{
	const char	*types;
	int			i;

	types = "cspdiuxX%";
	i = 0;
	while (types[i])
	{
		if (types[i] == c)
			return (1);
		i++;
	}
	return (0);
}
```

## 4. Using it in `print_format`

```c
handler = get_handler(str[*i]);
if (!handler)
	return (-1);              /* or print the raw text, your choice */
inpt->type = (t_types)str[*i];
(*i)++;
return (handler(args, inpt));
```

Every handler must match `t_handler`, i.e.
`int print_x(va_list *args, t_parsed *inpt)`. Handlers that don't use `inpt`
yet should `(void)inpt;` so `-Wunused-parameter` doesn't fail the build.

## 5. Other norm gotchas nearby

- Make `is_type` and `print_format` `static` if only `ft_printf.c` uses them.
  Otherwise their prototypes belong in the header.
- Function-pointer typedefs such as `t_handler` are allowed. Keep the
  `t_`/`s_`/`e_` prefixes as they are.
- Each function can have at most 4 parameters and 5 local variables. Each file
  can have at most 5 functions. A file with all nine handlers will need to be
  split.
