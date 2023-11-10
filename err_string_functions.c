#include "my_shell.h"

/**
 * print_error_string - prints an error string to stderr
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void print_error_string(char *str)
{
    int i = 0;

    if (!str)
        return;
    while (str[i] != '\0')
    {
        write_error_character(str[i]);
        i++;
    }
}

/**
 * write_error_character - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_error_character(char c)
{
    static int i;
    static char buf[WRITE_BUFFER_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUFFER_SIZE)
    {
        write(2, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return 1;
}

/**
 * write_to_file_descriptor - writes the character c to the given fd
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int write_to_file_descriptor(char c, int fd)
{
    static int i;
    static char buf[WRITE_BUFFER_SIZE];

    if (c == BUF_FLUSH || i >= WRITE_BUFFER_SIZE)
    {
        write(fd, buf, i);
        i = 0;
    }
    if (c != BUF_FLUSH)
        buf[i++] = c;
    return 1;
}

/**
 * print_to_file_descriptor - prints an input string to the given fd
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters written
 */
int print_to_file_descriptor(char *str, int fd)
{
    int i = 0;

    if (!str)
        return 0;
    while (*str)
    {
        i += write_to_file_descriptor(*str++, fd);
    }
    return i;
}

