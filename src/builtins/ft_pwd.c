/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:16:06 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/19 12:08:45 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_pwd(t_data *data, int index)
{
	char	pwd[PATH_MAX];
	char	*tmp;

	tmp = NULL;
	if (data->cmd[index].cmd[1] != NULL)
	{
		if (data->cmd[index].cmd[1][0] == '-')
		{
			printf("%s%s: %s: %s\n", PROMPT, data->cmd[index].cmd[0],
				data->cmd[index].cmd[1], NO_OPTION);
			return (CMD_ERROR);
		}
	}
	getcwd(pwd, PATH_MAX);
	printf("%s\n", pwd);
	return (CMD_OK);
}
