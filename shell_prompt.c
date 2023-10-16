#include "shell.h"

/**
 * shell_main - main control for the shell program
 * @info: pointer to an information_s struct containing shell information
 * @av: array of strings containing arguments to the shell
 * Return: the status of the last executed built_in command
 */
int shell_main(information_s *info, char **av)
{
	ssize_t read_result = 0;
	int built_in_return_value = 0;

	while (read_result != -1 && built_in_return_value != -2)
	{
		clear_info(info);
		/* Display the shell prompt if in interactive mode */
		if (from_terminal(info))
			_puts(":) ");
		putchar_err(NEG_ONE);
		read_result = get_input(info);
		if (read_result != -1)
		{
			set_info(info, av);
			built_in_return_value = handle_built_in(info);
			if (built_in_return_value == -1)
				check_command(info); }
		else if (from_terminal(info))
			put_char('\n');
		free_info(info, 0); }
	create_history(info);
	free_info(info, 1);
	if (!from_terminal(info) && info->status)
		exit(info->status);
	if (built_in_return_value == -2)
	{
		if (info->error_code == -1)
			exit(info->status);
		exit(info->error_code); }
	return (built_in_return_value);
}
/**
 * handle_built_in - finds a built_in command
 * @info: the parameter & return info struct
 * Return: -1 if built_in not found,
 * 0 if built_in executed successfully,
 * 1 if built_in found but not successful,
 * 2 if built_in signals exit()
 */
int handle_built_in(information_s *info)
{
	int i, built_in_return_value = -1;

	builtin_commands built_ints[] = {
		/*{"cd", handle_cd},*/
		{"env", _printenv},
		{"exit", handle_exit},
		/*{"help", handle_help},*/
		/*{"alias", handle_alias},*/
		/*{"setenv", check_setenv},*/
		/*{"history", handle_history},*/
		/*{"unsetenv", check_unsetenv},*/
		{NULL, NULL}};
	for (i = 0; built_ints[i].type; i++)
		if (compare_strinds(info->argv[0], built_ints[i].type) == 0)
		{
			info->lines++;
			built_in_return_value = built_ints[i].func(info);
			break; }
	return (built_in_return_value);
}
