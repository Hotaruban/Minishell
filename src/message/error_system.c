/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_system.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 00:57:02 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/15 13:38:37 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	error_system(char *msg)
{
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
