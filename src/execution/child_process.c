/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 22:45:18 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/21 16:49:27 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	files_redirection(t_data *data, int index)
{
	if (data->cmd[index].fd_infile > 2)
	{
		dup_files(data->cmd[index].fd_infile, STDIN_FILENO);
		close(data->cmd[index].fd_infile);
	}
	if (data->cmd[index].fd_outfile > 2)
	{
		dup_files(data->cmd[index].fd_outfile, STDOUT_FILENO);
		close(data->cmd[index].fd_outfile);
	}
}


bool	is_builtins(t_data *data, int index)
{
	if (data->cmd[index].cmd[0] != NULL)
	{
		if (ft_strncmp(data->cmd[index].cmd[0], "echo", 5) == 0
			|| ft_strncmp(data->cmd[index].cmd[0], "cd", 3) == 0
			|| ft_strncmp(data->cmd[index].cmd[0], "pwd", 4) == 0
			|| ft_strncmp(data->cmd[index].cmd[0], "export", 7) == 0
			|| ft_strncmp(data->cmd[index].cmd[0], "unset", 6) == 0
			|| ft_strncmp(data->cmd[index].cmd[0], "env", 4) == 0
			|| ft_strncmp(data->cmd[index].cmd[0], "exit", 5) == 0)
			return (true);	
	}
	return (false);
}

static int	redirection_pipes(t_data *data, int index)
{
	if (data->cmd[index + 1].cmd != NULL && data->cmd[index].fd_outfile <= 2)
	{
		if (dup_files(data->pipefd[1], STDOUT_FILENO) != CMD_OK)
			return (CMD_ERROR);
	}
	close(data->pipefd[0]);
	close(data->pipefd[1]);
	return (CMD_OK);
}

void	child_process(t_data *data, char **env, int index)
{
	if (data->cmd[index].status != 0)
	{
		// To modify for files error.
		error_cmd(data->cmd[index].cmd[0], data->cmd[index].msg_error);
		exit (data->cmd[index].status);
	}
	else if (data->cmd[index].fd_infile > 2 || data->cmd[index].fd_outfile > 2)
		files_redirection(data, index);
	else if (data->cmd[index].status == 0)
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
