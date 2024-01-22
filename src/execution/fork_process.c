/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:17:02 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/22 14:32:22 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parent_process(t_data *data, int index)
{
	close(data->pipefd[1]);
	if (data->pipe_len > 1)
	{
		if (data->cmd[index + 1].cmd[0] != NULL)
		{
			if (data->cmd[index].fd_infile < 2)
				dup2(data->pipefd[0], STDIN_FILENO);
		}
	}
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
			// exit (0);
			child_process(data, env, index + i);
		}
		else if (pid[i] > 0)
			parent_process(data, index + i);
		i++;
	}
	return (pid);
}
