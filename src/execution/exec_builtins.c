/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:16:21 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 21:59:39 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function is_builtins is used to check if the command is a builtin.
*/

bool	is_builtins(t_data *data, int index)
{
	if (ft_strncmp(data->cmd[index].cmd[0], "echo", 5) == 0 || \
		ft_strncmp(data->cmd[index].cmd[0], "cd", 3) == 0 || \
		ft_strncmp(data->cmd[index].cmd[0], "pwd", 4) == 0 || \
		ft_strncmp(data->cmd[index].cmd[0], "export", 7) == 0 || \
		ft_strncmp(data->cmd[index].cmd[0], "unset", 6) == 0 || \
		ft_strncmp(data->cmd[index].cmd[0], "env", 4) == 0 || \
		ft_strncmp(data->cmd[index].cmd[0], "exit", 5) == 0)
		return (true);
	return (false);
}

/*
The function execute_builtins is used to execute the builtin commands.
*/

int	execute_builtins(t_data *data, char **env, int index)
{
	if (ft_strncmp(data->cmd[index].cmd[0], "echo", 5) == 0)
		return (ft_echo(data, index));
	else if (ft_strncmp(data->cmd[index].cmd[0], "cd", 3) == 0)
		return (ft_cd(data, index));
	else if (ft_strncmp(data->cmd[index].cmd[0], "pwd", 4) == 0)
		return (ft_pwd(data, index));
	else if (ft_strncmp(data->cmd[index].cmd[0], "export", 7) == 0)
		return (ft_export(data, env, index));
	else if (ft_strncmp(data->cmd[index].cmd[0], "unset", 6) == 0)
		return (ft_unset(data, index));
	else if (ft_strncmp(data->cmd[index].cmd[0], "env", 4) == 0)
		return (ft_env(data, env, index));
	else if (ft_strncmp(data->cmd[index].cmd[0], "exit", 5) == 0)
		return (ft_exit(data, index));
	return (CMD_NOT_FOUND);
}

static bool	is_parent_builtin(t_data *data, int index)
{
	if (ft_strncmp(data->cmd[index].cmd[0], "exit", 5) == 0
		|| (ft_strncmp(data->cmd[index].cmd[0], "export", 7) == 0
			&& data->cmd[index].cmd[1] != NULL)
		|| ft_strncmp(data->cmd[index].cmd[0], "unset", 6) == 0
		|| ft_strncmp(data->cmd[index].cmd[0], "cd", 3) == 0)
		return (true);
	return (false);
}

static bool	builtin_no_pipe(t_data *data, char **env, int index)
{
	if (data->cmd[index].pipe_in == false
		&& data->cmd[index].pipe_out == false)
	{
		if (data->cmd[index].file_out == true
			|| data->cmd[index].file_in == true)
			return (true);
		if (ft_strncmp(data->cmd[index].cmd[0], "cd", 3) == 0)
			return (g_exit_status = execute_builtins(data, env, index), true);
	}
	return (false);
}

bool	builtin_in_parent(t_data *data, char **env, int index)
{
	int	status;

	status = CMD_OK;
	if (is_parent_builtin (data, index) == true)
	{
		if (data->cmd[index].pipe_in == false && \
			data->cmd[index].pipe_out == false)
		{
			if (is_parent_builtin (data, index) == true)
			{
				g_exit_status = execute_builtins(data, env, index) + status;
				return (true);
			}
		}
		if (builtin_no_pipe(data, env, index) == true)
			return (true);
	}
	return (false);
}
