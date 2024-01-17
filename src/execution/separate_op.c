/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:11 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 22:00:42 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function separator_op is used to execute a list of pipe command.
It returns the status of the last command executed.
*/

static int	waiting_pid(t_data *data, size_t len, pid_t *pid)
{
	int		status;
	size_t	i;

	(void)data;
	i = 0;
	while (i < len)
	{
		waitpid(pid[i++], &status, WUNTRACED);
		if (WIFSIGNALED(status))
			status = WTERMSIG(status) + 128;
		else
			status = WEXITSTATUS(status);
	}
	free(pid);
	exit (status);
}

static void	capsule_pipe(t_data *data, char **env, int index)
{
	int			status;
	pid_t		*pid_array;
	pid_t		pid;

	status = CMD_OK;
	pid = fork();
	if (pid == -1)
		error_system(FORK_ERROR);
	else if (pid == 0)
	{
		pid_array = fork_process(data, env, index);
		if (pid_array == NULL)
			exit(CMD_EXIT);
		status = waiting_pid(data, data->pipe_len, pid_array);
	}
	waitpid(pid, &status, 0);
	pid = 0;
	g_exit_status = WEXITSTATUS(status);
}

static int	pipe_op(t_data *data, char **env, int index)
{
	int				status;
	unsigned int	i;

	status = CMD_OK;
	i = 0;
	open_heredoc(data);
	status = check_acces_file(data, index);
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
