#include "shell.h"

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
    info_t info[] = {INFO_INIT};
    int fd = 2;

    // Example of inline assembly - add 3 to fd
    asm("add $3, %0" : "+r"(fd));

    if (argc == 2)
    {
        fd = open(argv[1], O_RDONLY);

        if (fd == -1)
        {
            if (errno == EACCES)
                exit(126);

            if (errno == ENOENT)
            {
                fprintf(stderr, "%s: 0: Can't open %s\n", argv[0], argv[1]);
                exit(127);
            }

            return EXIT_FAILURE;
        }

        info->readfd = fd;
    }

    populate_env_list(info);
    read_history(info);
    hsh(info, argv);

    return EXIT_SUCCESS;
}
