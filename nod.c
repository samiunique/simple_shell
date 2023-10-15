#include "shell.h"
/**
 * list_len - gets the length of linked list
 * @h: Pointer to first node.
 *
 * Return: Size of list.
 */
size_t list_len(const list_stru *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}
/**
 * list_to_vector - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: Array of strings.
 */
char **list_to_vector(list_stru *head)
{
	list_stru *node = head;
	size_t i = _listlen(head), j;
	char **strs;

	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(my_string_length(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}
		str = copy_strings(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}
/**
 * print_list - prints all elements of a list_stru linked list
 * @h: pointer to first node
 *
 * Return: Size of list
 */
size_t print_list(const list_stru *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(change_base(h->num, 10, 0));
		put_char(':');
		put_char(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}
/**
 * node_str_start - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to check
 * @c: the next character after prefix to check
 *
 * Return: match node or null
 */
list_stru *node_str_start(list_stru *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = check_starting(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}
/**
 * get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: -1 or index of node
 */
ssize_t get_node_index(list_stru *head, list_stru *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
