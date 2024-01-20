/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:16:06 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/21 01:17:20 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_pwd(t_data *data, int index)
{
	char	pwd[PATH_MAX];

	if (data->cmd[index].cmd[1] != NULL)
	{
		if (data->cmd[index].cmd[1][0] == '-')
		{
			error_cmd_msg("pwd", data->cmd[index].cmd[1], NO_OPTION);
			g_exit_status =  CMD_ERROR;
		}
	}
	else
	{
		getcwd(pwd, PATH_MAX);
		g_exit_status = CMD_OK;
	}

}
