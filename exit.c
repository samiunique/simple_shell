#include "shell.h"

/**
 * handle_exit - exits the shell
 * @info: contains simulated arguments for a function pointer,
 * allowing for a consistent function prototype
 * Return: exits with a given exit status
 * (0) if info.argv[0] != "exit"
 */

int handle_exit(information_s *info)
{
	int exitcheck;

	if (info->argv[1]) /* If there is an exit arguement */
	{
		exitcheck = err_num(info->argv[0]);/*1 to handle arg*/
		if (exitcheck == -1)
		{
			info->status = 2;
			print_error(info, "Illegal number: ");
			puts_err(info->argv[1]);
			putchar_err('\n');
			return (1);
		}
		info->error_code = err_num(info->argv[1]);
		return (-2);
	}
	info->error_code = -1;
	return (-2);
}
