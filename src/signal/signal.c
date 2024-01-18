/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 15:30:25 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/18 19:04:42 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigint_parent_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 1;
	}
}

void	sigint_child_handler(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		exit(130);
	}
}

void	set_signal(t_data *data, int type)
{
	if (type == HANDLE_SIGINT_PARENT)
	{
		data->sa_i.sa_handler = sigint_parent_handler;
		sigaction(SIGINT, &data->sa_i, NULL);
	}
	else if (type == IGNORE_SIGINT_PARENT)
	{
		data->sa_i.sa_handler = SIG_IGN;
		sigaction(SIGINT, &data->sa_i, NULL);
	}
	else if (type == IGNORE_SIGQUIT)
	{
		data->sa_q.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &data->sa_q, NULL);
	}
}

void	init_signal(t_data *data)
{
	ft_memset(&data->sa_i, 0, sizeof(struct sigaction));
	data->sa_i.sa_flags = SA_RESTART;
	set_signal(data, HANDLE_SIGINT_PARENT);
	ft_memset(&data->sa_q, 0, sizeof(struct sigaction));
	data->sa_q.sa_flags = SA_RESTART;
	set_signal(data, IGNORE_SIGQUIT);
}

void	set_echo_ctl(int enable)
{
	struct termios	terminal;

	ft_memset(&terminal, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, &terminal);
	if (enable)
		terminal.c_lflag |= ECHOCTL;
	else
		terminal.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminal);
}
