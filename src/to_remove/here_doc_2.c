/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 01:04:02 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/23 01:26:47 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	creat_here_doc(t_data *data, int index, int i_file)
{
	char	*line;

	// if (index == data->pipe_len)
	// 	tmp_pipe = data->pipefd;
	// else
	// {
	// 	if (pipe(tmp_pipe[2]->pipefd) == -1)
	// 		return (-1);
	// }
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

static void	execute_heredoc(t_data *data, int i)
{
	// size_t	i;
	int		j;
	int		fd;

	// i = 0;
	// while (data->pipe_len > i)
	// {
		j = 0;
		while (data->cmd[i].nb_heredocs > j)
		{
			fd = creat_here_doc(data, i, j);
			j++;
			if (data->cmd[i].here_doc_in == true
				&& data->cmd[i].nb_heredocs == j)
				data->cmd[i].here_doc_fd = fd;
			close(fd);
		}
		printf("data->cmd[%d].here_doc_fd: %d\n", i, data->cmd[i].here_doc_fd);
		// i++;
	// }
	exit (g_exit_status);
}

static void	get_exit_status(void)
{
	if (g_exit_status == 33280)
		g_exit_status = 1;
	else
		g_exit_status = WEXITSTATUS(g_exit_status);
}

static void	fork_heredoc(t_data *data)
{
	pid_t	*pid;
	size_t	i;
	
	pid = (pid_t *)malloc(sizeof(pid_t) * data->pipe_len);
	// if (!pid)
	// 	return (error_system(MALLOC_ERROR), NULL);
	i = 0;
	while (i < data->pipe_len)
	{
		pipe(data->pipefd);
			// return (error_system(PIPE_ERROR), NULL);
		pid[i] = fork();
		// if (pid[i] == -1)
			// error_system(FORK_ERROR);
		if (pid == 0)
		{
			data->sa_i.sa_handler = sigint_child_handler;
			sigaction(SIGINT, &data->sa_i, NULL);
			execute_heredoc(data, i);
		}
		else if (pid[i] > 0)
		{
			while (i < data->pipe_len)
			waitpid(pid[i], &g_exit_status, WUNTRACED);
		}
		i++;
	}
	close(data->pipefd[0]);
	close(data->pipefd[1]);
	printf("fork_heredoc data->cmd[0].here_doc_fd: %d\n", data->cmd[0].here_doc_fd);
	get_exit_status();
}

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
	if (flag == true)
		fork_heredoc(data);
	set_signal(data, HANDLE_SIGINT_PARENT);
	return (flag);
}