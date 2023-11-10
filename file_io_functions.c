#include "my_shell.h"

/**
 * get_shell_history_file - gets the history file for the shell
 * @info: parameter struct
 *
 * Return: allocated string containing history file path
 */
char *get_shell_history_file(info_t *info)
{
    char *directory, *filepath;

    directory = _getenv(info, "HOME=");
    if (!directory)
        return NULL;

    filepath = malloc(sizeof(char) * (_strlen(directory) + _strlen(SHELL_HIST_FILE) + 2));
    if (!filepath)
        return NULL;

    filepath[0] = 0;
    _strcpy(filepath, directory);
    _strcat(filepath, "/");
    _strcat(filepath, SHELL_HIST_FILE);

    return filepath;
}

/**
 * save_shell_history - creates a file or appends to an existing file to save shell history
 * @info: the parameter struct
 *
 * Return: 1 on success, -1 on failure
 */
int save_shell_history(info_t *info)
{
    ssize_t file_descriptor;
    char *filename = get_shell_history_file(info);
    list_t *node = NULL;

    if (!filename)
        return -1;

    file_descriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);

    if (file_descriptor == -1)
        return -1;

    for (node = info->shell_history; node; node = node->next)
    {
        _putsfd(node->str, file_descriptor);
        _putfd('\n', file_descriptor);
    }

    _putfd(BUF_FLUSH, file_descriptor);
    close(file_descriptor);

    return 1;
}

/**
 * load_shell_history - reads shell history from file
 * @info: the parameter struct
 *
 * Return: the number of history entries on success, 0 on failure
 */
int load_shell_history(info_t *info)
{
    int i, last = 0, entry_count = 0;
    ssize_t file_descriptor, read_length, file_size = 0;
    struct stat file_stats;
    char *buffer = NULL, *filename = get_shell_history_file(info);

    if (!filename)
        return 0;

    file_descriptor = open(filename, O_RDONLY);
    free(filename);

    if (file_descriptor == -1)
        return 0;

    if (!fstat(file_descriptor, &file_stats))
        file_size = file_stats.st_size;

    if (file_size < 2)
        return 0;

    buffer = malloc(sizeof(char) * (file_size + 1));
    if (!buffer)
        return 0;

    read_length = read(file_descriptor, buffer, file_size);
    buffer[file_size] = 0;

    if (read_length <= 0)
        return free(buffer), 0;

    close(file_descriptor);

    for (i = 0; i < file_size; i++)
        if (buffer[i] == '\n')
        {
            buffer[i] = 0;
            add_shell_history_entry(info, buffer + last, entry_count++);
            last = i + 1;
        }

    if (last != i)
        add_shell_history_entry(info, buffer + last, entry_count++);

    free(buffer);
    info->shell_history_count = entry_count;

    while (info->shell_history_count-- >= SHELL_HIST_MAX)
        delete_node_at_index(&(info->shell_history), 0);

    renumber_shell_history(info);

    return info->shell_history_count;
}

/**
 * add_shell_history_entry - adds an entry to the shell history linked list
 * @info: parameter struct
 * @buffer: buffer containing the history entry
 * @entry_count: the history entry count
 *
 * Return: Always 0
 */
int add_shell_history_entry(info_t *info, char *buffer, int entry_count)
{
    list_t *node = NULL;

    if (info->shell_history)
        node = info->shell_history;

    add_node_end(&node, buffer, entry_count);

    if (!info->shell_history)
        info->shell_history = node;

    return 0;
}

/**
 * renumber_shell_history - renumbers the shell history linked list after changes
 * @info: parameter struct
 *
 * Return: the new shell_history_count
 */
int renumber_shell_history(info_t *info)
{
    list_t *node = info->shell_history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }

    return info->shell_history_count = i;
}

