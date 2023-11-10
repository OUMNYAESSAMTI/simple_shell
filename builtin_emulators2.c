#include "my_shell.h"

/**
 * display_history - displays the history list, one command by line, preceded
 *                   with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * Return: Always 0
 */
int display_history(info_t *info)
{
    print_list(info->command_history);
    return 0;
}

/**
 * remove_alias - removes an alias by name
 * @info: parameter struct
 * @name: the name of the alias to be removed
 *
 * Return: Always 0 on success, 1 on error
 */
int remove_alias(info_t *info, char *name)
{
    char *p, c;
    int ret;

    p = _strchr(name, '=');
    if (!p)
        return 1;

    c = *p;
    *p = 0;
    ret = delete_node_at_index(&(info->aliases),
                                get_node_index(info->aliases, node_starts_with(info->aliases, name, -1)));
    *p = c;

    return ret;
}

/**
 * set_alias - sets an alias to a string
 * @info: parameter struct
 * @alias_str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(info_t *info, char *alias_str)
{
    char *p;

    p = _strchr(alias_str, '=');
    if (!p)
        return 1;

    if (!*++p)
        return remove_alias(info, alias_str);

    remove_alias(info, alias_str);
    return (add_node_end(&(info->aliases), alias_str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(list_t *node)
{
    char *p = NULL, *a = NULL;

    if (node)
    {
        p = _strchr(node->str, '=');
        for (a = node->str; a <= p; a++)
            _putchar(*a);
        _putchar('\'');
        _puts(p + 1);
        _puts("'\n");
        return 0;
    }
    return 1;
}

/**
 * manage_alias - manages aliases similar to the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int manage_alias(info_t *info)
{
    int i = 0;
    char *p = NULL;
    list_t *node = NULL;

    if (info->argc == 1)
    {
        node = info->aliases;
        while (node)
        {
            print_alias(node);
            node = node->next;
        }
        return 0;
    }

    for (i = 1; info->argv[i]; i++)
    {
        p = _strchr(info->argv[i], '=');
        if (p)
            set_alias(info, info->argv[i]);
        else
            print_alias(node_starts_with(info->aliases, info->argv[i], '='));
    }

    return 0;
}

