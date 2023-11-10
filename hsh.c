void fork_cmd(info_t *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error during fork");
        return;
    }

    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, get_environ(info)) == -1)
        {
            free_info(info, 1);
            perror("execve failed");
            if (errno == EACCES)
                exit(EXIT_PERM_DENIED);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == EXIT_PERM_DENIED)
                print_error(info, "Permission denied\n");
        }
    }
}
