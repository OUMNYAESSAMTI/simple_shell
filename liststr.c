list_t *add_node(list_t **head, const char *str, int num)
{
    if (!head)
        return NULL;

    list_t *new_head = malloc(sizeof(list_t));
    if (!new_head)
        return NULL;

    _memset(new_head, 0, sizeof(list_t));
    new_head->num = num;

    if (str)
    {
        new_head->str = _strdup(str);
        if (!new_head->str)
        {
            free(new_head);
            return NULL;
        }
    }

    new_head->next = *head;
    *head = new_head;

    return new_head;
}

list_t *add_node_end(list_t **head, const char *str, int num)
{
    if (!head)
        return NULL;

    list_t *new_node = malloc(sizeof(list_t));
    if (!new_node)
        return NULL;

    _memset(new_node, 0, sizeof(list_t));
    new_node->num = num;

    if (str)
    {
        new_node->str = _strdup(str);
        if (!new_node->str)
        {
            free(new_node);
            return NULL;
        }
    }

    list_t *node = *head;
    if (node)
    {
        while (node->next)
            node = node->next;
        node->next = new_node;
    }
    else
    {
        *head = new_node;
    }

    return new_node;
}

int delete_node_at_index(list_t **head, unsigned int index)
{
    if (!head || !*head)
        return 0;

    if (index == 0)
    {
        list_t *node = *head;
        *head = (*head)->next;
        free(node->str);
        free(node);
        return 1;
    }

    list_t *current = *head;
    list_t *prev_node = NULL;

    for (unsigned int i = 0; current && i < index; i++)
    {
        prev_node = current;
        current = current->next;
    }

    if (!current)
        return 0;

    prev_node->next = current->next;
    free(current->str);
    free(current);

    return 1;
}
