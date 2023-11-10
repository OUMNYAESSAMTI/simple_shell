#include "shell.h"

size_t list_len(const list_t *h)
{
    size_t i = 0;

    while (h)
    {
        h = h->next;
        i++;
    }
    return i;
}

char **list_to_strings(list_t *head)
{
    list_t *node = head;
    size_t i = list_len(head);
    char **strs;
    char *str;

    if (!head || !i)
        return NULL;

    strs = malloc(sizeof(char *) * (i + 1));
    if (!strs)
    {
        // Handle memory allocation failure
        return NULL;
    }

    for (i = 0; node; node = node->next, i++)
    {
        // Allocate memory for the string
        str = malloc(_strlen(node->str) + 1);
        if (!str)
        {
            // Handle memory allocation failure
            // Cleanup previously allocated memory
            for (size_t j = 0; j < i; j++)
                free(strs[j]);
            free(strs);
            return NULL;
        }

        // Copy the string
        _strcpy(str, node->str);
        strs[i] = str;
    }

    strs[i] = NULL;
    return strs;
}
