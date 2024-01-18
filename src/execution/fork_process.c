/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:17:02 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/18 18:58:32 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	child_process(t_data *data, char **env, int index)
{
	if (data->cmd[index].here_doc_in == true)
	{
		if (redirection_heredoc(data, index) != CMD_OK)
			exit(CMD_ERROR);
	}
	if (data->cmd[index].cmd != NULL)
	{
		if (data->cmd[index].file_in == true
			&& data->cmd[index].here_doc_in == false)
			redir_infiles(data, index);
		if (data->cmd[index].file_out == true)
			redir_outfiles(data, index);
		if (redirection_pipes(data, index) != CMD_OK)
			exit(CMD_ERROR);
		if (is_builtins(data, index) == true)
			exit(execute_builtins(data, env, index));
		else
			execute_cmd((const t_data *)data,
				(const char **)data->cmd[index].cmd,
				(const char **)env);
	}
	exit (g_exit_status);
}

static void	parent_process(t_data *data, int index)
{
	close(data->pipefd[1]);
	if (data->cmd[index].file_in == false
		&& data->cmd[index].here_doc_in == false)
		dup_files(data->pipefd[0], STDIN_FILENO);
	close(data->pipefd[0]);
}

pid_t	*fork_process(t_data *data, char **env, int index)
{
	pid_t	*pid;
	size_t	i;

	pid = (pid_t *)malloc(sizeof(pid_t) * data->pipe_len);
	if (!pid)
		return (error_system(MALLOC_ERROR), NULL);
	i = 0;
	while (i < data->pipe_len)
	{
		if (pipe(data->pipefd) == -1)
			return (error_system(PIPE_ERROR), NULL);
		pid[i] = fork();
		if (pid[i] == -1)
			return (error_system(FORK_ERROR), NULL);
		else if (pid[i] == 0)
		{
			data->sa_i.sa_handler = sigint_child_handler;
			sigaction(SIGINT, &data->sa_i, NULL);
			child_process(data, env, index + i);
		}
		else if (pid[i] > 0)
			parent_process(data, index + i);
		i++;
	}
	return (pid);
}
