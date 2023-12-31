#include "shell.h"

/**
 * input_buf - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t input_buf(information_s *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*bfree((void **)info->sep_buff);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, handle_sigint);
#if USING_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = scanit(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->lc_flag = 1;
			/*handle_comments(*buf);*/
			update_history(info, *buf, info->hist_lines++);
			/* if (_strchr(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->sep_buff = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input(information_s *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	put_char(NEG_ONE);
	r = input_buf(info, &buf, &len); /*replce (info, &buf, &len);*/
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		j = i;		 /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		check_chain(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(info, buf, &j))
				break;
			j++;
		}
		i = j + 1;	  /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->sep_buff_kind = REG_F;
		}
		*buf_p = p;			 /* pass back pointer to current command position */
		return (my_string_length(p)); /* return length of current command */
	}
	*buf_p = buf; /* else not a chain, pass back buffer from _getline() */
	return (r);	  /* return length of buffer from _getline() */
}

/**
 * read_buf - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t read_buf(information_s *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->fd_read, buf, BUFFER_SIZE_R);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * scanit - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int scanit(information_s *info, char **ptr, size_t *length)
{
	static char buf[BUFFER_SIZE_R];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);
	c = char_strings(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = re_alloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);
	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * handle_sigint - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void handle_sigint(__attribute__((unused)) int sig_num)
{
	_puts("\n");
	_puts(":) ");
	put_char(-1);
}

