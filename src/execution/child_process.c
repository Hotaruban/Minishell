/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 22:45:18 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/22 16:40:46 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_quit(int signum)
{
	if (signum == SIGQUIT)
		exit(131);
}

static void	open_quit_signal_on_valid_cmd(t_data *data, int index)
{
	if (!ft_strncmp(data->cmd[index].cmd[0], "cat", 3) || \
		!ft_strncmp(data->cmd[index].cmd[0], "/bin/cat", 8) || \
		(!ft_strncmp(data->cmd[index].cmd[0], "grep", 4) && \
		data->cmd[index].cmd[1] != NULL) || \
		(!ft_strncmp(data->cmd[index].cmd[0], "/usr/bin/grep", 13) && \
		data->cmd[index].cmd[1] != NULL) || \
		(!ft_strncmp(data->cmd[index].cmd[0], "awk", 3) && \
		data->cmd[index].cmd[1] != NULL) || \
		(!ft_strncmp(data->cmd[index].cmd[0], "/usr/bin/awk", 12) && \
		data->cmd[index].cmd[1] != NULL) || \
		(!ft_strncmp(data->cmd[index].cmd[0], "wc", 2) && \
		data->cmd[index].cmd[1] == NULL) || \
		(!ft_strncmp(data->cmd[index].cmd[0], "/usr/bin/wc", 11) && \
		data->cmd[index].cmd[1] == NULL))
	{
		data->sa_q.sa_handler = handle_quit;
		sigaction(SIGQUIT, &data->sa_q, NULL);
	}
}

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
	printf("we have reached here (child process start) \n");
	check_error_exit(data, index);
	printf("we have reached here (child process start2) \n");
	if (data->cmd[index].fd_infile > 2 || data->cmd[index].fd_outfile > 2)
		files_redirection(data, index);
	if (data->cmd[index].status == 0 && data->pipe_len > 1)
		redirection_pipes(data, index);
	printf("we have reached here (child process mid) \n");
	if (is_builtins(data, index) == true)
	{
		execute_builtins(data, env, index);
		exit (g_exit_status);
	}
	printf("we have reached here (child process mid 1) \n");
	open_quit_signal_on_valid_cmd(data, index);	
	int i = execve(data->cmd[index].path, data->cmd[index].cmd, env);
	// else if (execve(data->cmd[index].path, data->cmd[index].cmd, env) == -1)
	if (i == -1)
	{
		printf("we have reached here (child process mid2) \n");
		error_system("execve failed");
		exit (CMD_ERROR);
	}
	printf("we have reached here (child process end) \n");
}
