#include "my_shell.h"

/**
 * buffer_chained_commands - buffers chained commands
 * @info: parameter struct
 * @command_buffer: address of command buffer
 * @buffer_length: address of buffer length var
 *
 * Return: bytes read
 */
ssize_t buffer_chained_commands(info_t *info, char **command_buffer, size_t *buffer_length)
{
    ssize_t bytes_read = 0;
    size_t buffer_position = 0;

    if (!*buffer_length) /* if nothing left in the buffer, fill it */
    {
        free(*command_buffer);
        *command_buffer = NULL;

        signal(SIGINT, handle_interrupt);

        bytes_read = getline(command_buffer, &buffer_position, stdin);

        if (bytes_read > 0)
        {
            if ((*command_buffer)[bytes_read - 1] == '\n')
            {
                (*command_buffer)[bytes_read - 1] = '\0'; /* remove trailing newline */
                bytes_read--;
            }

            info->linecount_flag = 1;
            remove_comments(*command_buffer);
            build_history_list(info, *command_buffer, info->histcount++);

            *buffer_length = bytes_read;
            info->cmd_buffer = command_buffer;
        }
    }

    return bytes_read;
}

/**
 * get_input_line - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_input_line(info_t *info)
{
    static char *command_buffer; /* the ';' command chain buffer */
    static size_t buffer_position, buffer_length;
    ssize_t bytes_read = 0;
    char **buffer_pointer = &(info->args), *position;

    _putchar(BUF_FLUSH);

    bytes_read = buffer_chained_commands(info, &command_buffer, &buffer_length);

    if (bytes_read == -1) /* EOF */
        return -1;

    if (buffer_length) /* we have commands left in the chain buffer */
    {
        size_t j = buffer_position; /* init new iterator to current buf position */
        position = command_buffer + buffer_position; /* get pointer for return */

        check_chain(info, command_buffer, &j, buffer_position, buffer_length);

        while (j < buffer_length) /* iterate to semicolon or end */
        {
            if (is_chain(info, command_buffer, &j))
                break;

            j++;
        }

        buffer_position = j + 1; /* increment past nulled ';'' */

        if (buffer_position >= buffer_length) /* reached end of buffer? */
        {
            buffer_position = buffer_length = 0; /* reset position and length */
            info->cmd_buf_type = CMD_NORM;
        }

        *buffer_pointer = position; /* pass back pointer to current command position */
        return _strlen(position); /* return length of current command */
    }

    *buffer_pointer = command_buffer; /* else not a chain, pass back buffer from getline() */
    return bytes_read; /* return length of buffer from getline() */
}

/**
 * read_buffer - reads a buffer
 * @info: parameter struct
 * @buffer: buffer
 * @position: size
 *
 * Return: r
 */
ssize_t read_buffer(info_t *info, char *buffer, size_t *position)
{
    ssize_t bytes_read = 0;

    if (*position)
        return 0;

    bytes_read = read(info->read_fd, buffer, READ_BUF_SIZE);

    if (bytes_read >= 0)
        *position = bytes_read;

    return bytes_read;
}

/**
 * my_getline - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int my_getline(info_t *info, char **ptr, size_t *length)
{
    static char buffer[READ_BUF_SIZE];
    static size_t buffer_position, buffer_length;
    size_t k;
    ssize_t bytes_read = 0, buffer_size = 0;
    char *position = NULL, *new_position = NULL, *c;

    position = *ptr;

    if (position && length)
        buffer_size = *length;

    if (buffer_position == buffer_length)
        buffer_position = buffer_length = 0;

    bytes_read = read_buffer(info, buffer, &buffer_length);

    if (bytes_read == -1 || (bytes_read == 0 && buffer_length == 0))
        return -1;

    c = _strchr(buffer + buffer_position, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : buffer_length;

    new_position = _realloc(position, buffer_size, buffer_size ? buffer_size + k : k + 1);

    if (!new_position) /* MALLOC FAILURE! */
        return (position ? free(position), -1 : -1);

    if (buffer_size)
        _strncat(new_position, buffer + buffer_position, k - buffer_position);
    else
        _strncpy(new_position, buffer + buffer_position, k - buffer_position + 1);

    buffer_size += k - buffer_position;
    buffer_position = k;
    position = new_position;

    if (length)
        *length = buffer_size;

    *ptr = position;
    return buffer_size;
}

/**
 * handle_interrupt - blocks ctrl-C
 * @signal_number: the signal number
 *
 * Return: void
 */
void handle_interrupt(__attribute__((unused))int signal_number)
{
    _puts("\n");
}
