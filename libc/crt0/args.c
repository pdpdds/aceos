/* $Id: args.c,v 1.1.1.1 2002/12/21 09:49:35 pavlovskii Exp $ */

#include <os\kernel32.h>
#include <stdlib.h>
#include <ctype.h>
#include <internal.h>
#include <string.h>

#define MAIN	main
#define CHAR	char
#define _istspace	isspace
#define _tcslen		strlen

CHAR *__crt0_argv[2];

int __crt0_parseargs(CHAR ***argv)
{
	CHAR *args, *ch;
	size_t size;
	int argc, i;
	CHAR **theargv;

	args = GetCommandLine();
	argc = 1;

	for (ch = args; *ch; ch++)
		if (*ch == ' ')
		{
			*ch = '\0';
			argc++;
			while (*ch && _istspace(*ch))
				ch++;
		}

	if (argc <= _countof(__crt0_argv))
		theargv = __crt0_argv;
	else
		theargv = malloc(sizeof(CHAR*) * argc);

	ch = args;
	for (i = 0; i < argc; i++)
	{
		theargv[i] = ch;
		ch += _tcslen(ch) + 1;
	}

	*argv = theargv;
	return argc;
}

