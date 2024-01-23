/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 21:04:25 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/23 10:14:30 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	creat_here_doc(t_data *data, int index, int i_file)
{
	char	*line;
	int		fd;

	if (pipe(data->pipefd) == -1)
		return (-1);
	printf("data->pipefd[0] => %d// data->pipefd[1] = %d\n", data->pipefd[0], data->pipefd[1]);
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
	fd = dup(data->pipefd[0]);
	close (data->pipefd[0]);
	printf("fd => %d\n", fd);
	return (fd);
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
				{
					data->cmd[i].here_doc_fd = fd;
					break ;
				}
			close(fd);
		}
		i++;
	}
	// exit (g_exit_status);
}

// static void	get_exit_status(void)
// {
// 	if (g_exit_status == 33280)
// 		g_exit_status = 1;
// 	else
// 		g_exit_status = WEXITSTATUS(g_exit_status);
// }

// static void	fork_heredoc(t_data *data)
// {
// 	pid_t	pid;
	
// 	pid = fork();
// 	if (pid == -1)
// 		error_system(FORK_ERROR);
// 	else if (pid == 0)
// 	{
// 		data->sa_i.sa_handler = sigint_child_handler;
// 		sigaction(SIGINT, &data->sa_i, NULL);
// 		execute_heredoc(data);
// 	}
// 	waitpid(pid, &g_exit_status, WUNTRACED);
// 	get_exit_status();
// }

bool	open_heredoc(t_data *data)
{
	size_t	i;
	bool	flag;

	i = 0;
	flag = false;
	set_signal(data, IGNORE_SIGINT_PARENT);
	while (data->pipe_len > i)
	{
		if (data->cmd[i++].here_doc_in == true)
			flag = true;
	}
	if (flag == true)						///// ADD SIGNAL FOR CTRL + C in parent process in heredoc
	{
		data->sa_i.sa_handler = sigint_child_handler;
		sigaction(SIGINT, &data->sa_i, NULL);
		execute_heredoc(data);
	}
		// fork_heredoc(data);
	set_signal(data, HANDLE_SIGINT_PARENT);
	return (flag);
}
