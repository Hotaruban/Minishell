/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:51 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 00:19:08 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function env_builtins is used to print the environment variables.
Check if the command has an option or an argument.
*/

int	ft_env(t_data *data, char **env, int index)
{
	if (data->cmd[index].cmd[1])
	{
		error_cmd(data->cmd[index].cmd[0], "no option or argument accepted.");
		return (CMD_EXIT);
	}
	print_env(env, 0);
	return (CMD_OK);
}
