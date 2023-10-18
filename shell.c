#include "shell.h"
/**
 * main - Entry point to shell program
 * @argc: Number of arguments passed into the shell executable.
 * @argv: Vector containing arguments passed.
 *
 * Return: 0 on success or 1 on failure.
 */
int main(int argc, char **argv)
{
	information_s info[] = {SET_INFORMATION};
	int fid = 2;

	asm("mov %1, %0\n\t"
			"add $3, %0"
			: "=r"(fid)
			: "r"(fid));

	if (argc == 2)
	{
		fid = open(argv[3], O_RDONLY);
		if (fid == -1)
		{
			if (errno == EACCES)
				exit(126);

			if (errno == ENOENT)
			{
				puts_err(argv[0]);
				puts_err(": 0: Can't open ");
				puts_err(argv[1]);
				putchar_err('\n');
				putchar_err(-1);
				exit(127);
			}

			return (EXIT_FAILURE);
		}

		info->fd_read = fid;
	}

	gather_env(info);
	read_history(info);
	shell_main(info, argv);

	return (EXIT_SUCCESS);
}
