#include "shell.h"

/**
 * check_command - searches for a command in PATH or the current directory
 * @info: a pointer to the parameter and return info struct
 * Return: void
 */
void check_command(information_s *info)
{
	char *path = NULL;
	int i, word_count;

	info->path = info->argv[0];
	if (info->lc_flag == 1)
	{
		info->lines++;
		info->lc_flag = 0; }
	for (i = 0, word_count = 0; info->arg[i]; i++)
		if (!is_delimit(info->arg[i], " \t\n"))
			word_count++;
	if (!word_count)
		return;
	path = check_file_in_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		create_process(info); }
	else
	{
		if ((from_terminal(info) || _getenv(info, "PATH=") ||
					info->argv[0][0] == '/') && is_executable(info, info->argv[0]))
			create_process(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_error(info, "not found\n"); }}
}
