/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:04:25 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/16 17:31:26 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	creat_here_doc(t_data *data, int index, int i_file)
{
	char	*line;

	if (pipe(data->pipefd) == -1)
		return (-1);
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (ft_strncmp(line, data->cmd[index].limiters[i_file],
				ft_strlen(data->cmd[index].limiters[i_file])) == 0
			&& ((ft_strlen(line) - 1)
				== ft_strlen(data->cmd[index].limiters[i_file])))
			break ;
		write(data->pipefd[1], line, ft_strlen(line));
		free(line);
	}
	free(line);
	close (data->pipefd[1]);
	return (data->pipefd[0]);
}

static void	execute_heredoc(t_data *data)
{
	size_t	i;
	int		j;
	int		fd;

	i = 0;
	while (data->pipe_len > i)
	{
		j = 0;
		while (data->cmd[i].nb_heredocs > j)
		{
			fd = creat_here_doc(data, i, j);
			j++;
			if (data->cmd[i].here_doc_in == true
				&& data->cmd[i].nb_heredocs == j)
				data->cmd[i].here_doc_fd = dup(fd);
			close(fd);
		}
		i++;
	}
}

void	open_heredoc(t_data *data)
{
	int		status;
	pid_t	pid;
	
	status = CMD_OK;
	pid = fork();
	if (pid == -1)
		error_system("fork failed");
	else if (pid == 0)
	{
		data->sa_i.sa_handler = sigint_child_handler;
		sigaction(SIGINT, &data->sa_i, NULL);
		execute_heredoc(data);
	}
	waitpid(pid, &status, 0);
}
