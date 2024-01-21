/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 22:45:18 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/22 01:09:54 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	check_error_exit(t_data *data, int index)
{
	DIR		*dir;

	if (data->cmd[index].status != 0 || (data->cmd[index].status == 0
			&& data->cmd[index].cmd[0] == NULL))
	{
		if (data->cmd[index].error_str != NULL
			|| data->cmd[index].msg_error != NULL)
			error_cmd(data->cmd[index].error_str, data->cmd[index].msg_error);
		exit (data->cmd[index].status);
	}
	dir = opendir(data->cmd[index].cmd[0]);
	if (dir != NULL)
	{
		error_cmd(data->cmd[index].cmd[0], IS_DIR);
		closedir(dir);
		exit (CMD_NOT_EXEC);
	}
}

static void	files_redirection(t_data *data, int index)
{
	if (data->cmd[index].fd_infile > 2)
	{
		dup2(data->cmd[index].fd_infile, STDIN_FILENO);
		close(data->cmd[index].fd_infile);
	}
	if (data->cmd[index].fd_outfile > 2)
	{
		dup2(data->cmd[index].fd_outfile, STDOUT_FILENO);
		close(data->cmd[index].fd_outfile);
	}
}

static int	redirection_pipes(t_data *data, int index)
{
	if ((int)data->pipe_len > (index + 1) && data->cmd[index].fd_outfile < 2)
		dup2(data->pipefd[1], STDOUT_FILENO);
	close(data->pipefd[0]);
	close(data->pipefd[1]);
	return (CMD_OK);
}

void	child_process(t_data *data, char **env, int index)
{
	check_error_exit(data, index);
	if (data->cmd[index].fd_infile > 2 || data->cmd[index].fd_outfile > 2)
		files_redirection(data, index);
	if (data->cmd[index].status == 0 && data->pipe_len > 1)
		redirection_pipes(data, index);
	if (is_builtins(data, index) == true)
	{
		execute_builtins(data, env, index);
		exit (g_exit_status);
	}
	else if (execve(data->cmd[index].path, data->cmd[index].cmd, env) == -1)
	{
		error_system("execve failed");
		exit (CMD_ERROR);
	}
}
