/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_system.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 00:57:02 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 11:16:03 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	error_system(char *msg)
{
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	error_cmd(char *cmd, char *msg)
{
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	error_input(char *msg, char *msg_type)
{
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd("`", STDERR_FILENO);
	ft_putstr_fd(msg_type, STDERR_FILENO);
	ft_putendl_fd("\'", STDERR_FILENO);
}

void	error_cmd_var(char *cmd, char *msg, char *str)
{
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd("`", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putstr_fd("\': ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	error_file_msg(char *file, char *msg)
{
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	error_cmd_msg(char *cmd1, char *cmd2, char *msg)
{
	ft_putstr_fd(PROMPT, STDERR_FILENO);
	ft_putstr_fd(cmd1, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd("`", STDERR_FILENO);
	ft_putstr_fd(cmd2, STDERR_FILENO);
	ft_putstr_fd("\': ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	error_exit_msg(char *arg, char *str)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (str != NULL)
	{
		ft_putstr_fd(PROMPT, STDERR_FILENO);
		ft_putstr_fd("exit: ", STDERR_FILENO);
		if (arg != NULL)
		{
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
		}
		ft_putendl_fd(str, STDERR_FILENO);
	}
}