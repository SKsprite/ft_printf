*This project has been created as part of the 42 curriculum by <login>*
## Description
## Instructions
## Resources
Notes:
	[Flag types]
	 -  left align
	 0  zero pad (for numbers)
	 #  adds 0x
	' ' add space infront of number
	 +  add sign for number

	[Width]
	    any numbers 0-9

	[Precision]
	 .  for %d, 0 pad to precision and %s no of char to print

	[Types]
	 %c char
	 %s string
	 %p pointer
	 %d decimal
	 %i integer
	 %u unsigned decimal
	 %x hex lower
	 %X hex upper
	 %% percentage sign

[logic]
my_printf
    print literal text
    parser(&curr)
        parse specification
        advance curr past specification
        count = formatter(...)
        return count
    total_count += returned count
    loop

[.gitignore]

.gitignore
libft/*
libft

