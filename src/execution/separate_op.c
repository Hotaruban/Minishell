/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:11 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/18 12:23:14 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function separator_op is used to execute a list of pipe command.
It returns the status of the last command executed.
*/

static void	waiting_pid(t_data *data, size_t len, pid_t *pid)
{
	size_t	i;

	(void)data;
	i = 0;
	while (i < len)
	{
		waitpid(pid[i++], &g_exit_status, WUNTRACED);
		if (WIFSIGNALED(g_exit_status))
			g_exit_status = WTERMSIG(g_exit_status) + 128;
		else
			g_exit_status = WEXITSTATUS(g_exit_status);
	}
	free(pid);
}

static void	capsule_pipe(t_data *data, char **env, int index)
{
	pid_t		*pid_array;
	pid_t		pid;

	pid = fork();
	if (pid == -1)
		error_system(FORK_ERROR);
	else if (pid == 0)
	{
		pid_array = fork_process(data, env, index);
		if (pid_array == NULL)
			exit(CMD_EXIT);
		waiting_pid(data, data->pipe_len, pid_array);
	}
	waitpid(pid, &g_exit_status, 0);
	pid = 0;
	g_exit_status = WEXITSTATUS(g_exit_status);
}

static int	pipe_op(t_data *data, char **env, int index)
{
	unsigned int	i;

	i = 0;
	open_heredoc(data);
	g_exit_status = check_acces_file(data, index);
	while (i < data->pipe_len)
	{
		if (data->cmd[index].cmd[0] != NULL)
			break ;
		i++;
		if (i == data->pipe_len)
			return (CMD_OK);
	}
	if (builtin_in_parent(data, env, index) == true)
		return (CMD_OK);
	else
		capsule_pipe(data, env, index);
	return (CMD_OK);
}

int	separator_op(t_data *data)
{
	char	**ev;

	g_exit_status = CMD_OK;
	ev = env_array(data->env);
	if (ev == NULL)
		return (CMD_ERROR);
	set_signal(data, IGNORE_SIGINT_PARENT);
	if (pipe_op(data, ev, 0) != CMD_OK)
		return (CMD_ERROR);
	free_2d_array(ev);
	set_signal(data, HANDLE_SIGINT_PARENT);
	return (g_exit_status);
}
