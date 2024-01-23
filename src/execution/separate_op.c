/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separate_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:11 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/23 15:54:50 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function separator_op is used to execute a list of pipe command.
It returns the status of the last command executed.
*/

static void	waiting_pid(size_t len, pid_t *pid)
{
	size_t	i;
	int		status;

	i = 0;
	status = 0;
	while (i < len)
	{
		waitpid(pid[i], &status, WUNTRACED);
		if (WIFSIGNALED(status))
			g_exit_status = WTERMSIG(status);
		else
			g_exit_status = WEXITSTATUS(status);
		i++;
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
	int			i;

	status = 0;
	pid = fork();
	if (pid == -1)
		error_system(FORK_ERROR);
	else if (pid == 0)
	{
		pid_array = fork_process(data, env, index);
		if (pid_array == NULL)
			exit(CMD_ERROR);
		waiting_pid(data->pipe_len, pid_array);
	}
	waitpid(pid, &status, WUNTRACED);
	if (WIFSIGNALED(status))
	{
		g_exit_status = status - 10;
		i = WTERMSIG(status) + 128;
		// printf("status = %d \n", i);
	}
	else
		g_exit_status = WEXITSTATUS(status);
	// printf("status = %d \n", g_exit_status);
	// printf("i = %d \n", status);
	// status = 0;
}

void	separator_op(t_data *data)
{
	set_signal(data, IGNORE_SIGINT_PARENT);
	if (builtin_in_parent(data, 0) == true && data->pipe_len == 1)
		execute_builtins(data, data->ev_array, 0);
	else
		capsule_pipe(data, data->ev_array, 0);
	set_signal(data, HANDLE_SIGINT_PARENT);
	if (data->ev_array)
		free_2d_array(data->ev_array);
}
