/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:10:30 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 15:02:15 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	dup_files(int fd_target, int fd_origin)
{
	if (dup2(fd_target, fd_origin) == -1)
	{
		error_system(DUP_ERROR);
		if (fd_origin != STDIN_FILENO && fd_origin != STDOUT_FILENO
			&& fd_origin != STDERR_FILENO)
			close(fd_origin);
		if (fd_target != STDIN_FILENO && fd_target != STDOUT_FILENO
			&& fd_target != STDERR_FILENO)
			close(fd_target);
		return (CMD_ERROR);
	}
	return (CMD_OK);
}
