/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:11 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/19 12:09:25 by jhurpy           ###   ########.fr       */
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
	int		status;

	(void)data;
	i = 0;
	status = 0;
	while (i < len)
	{
		waitpid(pid[i++], &status, WUNTRACED);
		if (WIFSIGNALED(status))
			g_exit_status = WTERMSIG(status) + 128;
		else
			g_exit_status = WEXITSTATUS(status);
	}
	free(pid);
	status = 0;
	exit (g_exit_status);
}

static void	capsule_pipe(t_data *data, char **env, int index)
{
	pid_t		*pid_array;
	pid_t		pid;
	int			status;

	status = 0;
	pid = fork();
	if (pid == -1)
		error_system(FORK_ERROR);
	else if (pid == 0)
	{
		pid_array = fork_process(data, env, index);
		if (pid_array == NULL)
			exit(CMD_ERROR);
		waiting_pid(data, data->pipe_len, pid_array);
	}
	waitpid(pid, &status, WUNTRACED);
	if (WIFSIGNALED(status))
		g_exit_status = WTERMSIG(status) + 128;
	else
		g_exit_status = WEXITSTATUS(status);
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
		index++;
		if (i == data->pipe_len)
			return (g_exit_status);
	}
	if (builtin_in_parent(data, env, index) == true)
		return (g_exit_status);
	else
		capsule_pipe(data, env, index);
	return (g_exit_status);
}

int	separator_op(t_data *data)
{
	char	**ev;

	g_exit_status = CMD_OK;
	set_signal(data, IGNORE_SIGINT_PARENT);
	ev = env_array(data->env);
	if (ev == NULL)
		return (CMD_ERROR);
	if (pipe_op(data, ev, 0) != CMD_OK)
	{
		free_2d_array(ev);
		return (g_exit_status);
	}
	free_2d_array(ev);
	return (g_exit_status);
}
