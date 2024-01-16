/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:16:00 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 00:13:51 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function export_builtins is used to add or change the value of
environment variables.
If export is called without arguments, it prints the environment.
If export is called with arguments, it adds or changes the value of
environment variables.
*/

static void	find_variable(t_env *env, char **var)
{
	t_env	*tmp_env;
	char	*var_tmp;
	int		i;

	i = 0;
	while (var[i])
	{
		tmp_env = env;
		while (tmp_env)
		{
			var_tmp = ft_substr(var[i], 0, len_variable(var[i]));
			if (ft_strncmp(tmp_env->name, var_tmp, len_variable(var[i])) == 0)
			{
				free(tmp_env->name);
				tmp_env->name = ft_strdup(var[i]);
				free (var_tmp);
				break ;
			}
			else if (tmp_env->next == NULL
				&& len_variable(var[i]) < (int)ft_strlen(var[i]))
				add_variable(tmp_env, var[i]);
			tmp_env = tmp_env->next;
		}
		i++;
	}
}

int	ft_export(t_data *data, char **env, int index)
{
	int	status;
	int	i;

	status = CMD_OK;
	if (data->cmd[index].cmd[1] == NULL)
		return (print_env(env), status);
	if (data->cmd[index].cmd[1][0] == '-')
	{
		error_cmd(data->cmd[index].cmd[0], NO_OPTION);
		return (CMD_EXIT);
	}
	i = 1;
	while (data->cmd[index].cmd[i])
	{
		if (check_variable(data->cmd[index].cmd[i]) == false)
		{
			printf("%s%s: `%s\': %s\n", PROMPT, data->cmd[index].cmd[0],
				data->cmd[index].cmd[i], NO_VALID_ID);
			status = CMD_ERROR;
		}
		i++;
	}
	find_variable(data->env, data->cmd[index].cmd + 1);
	return (status);
}
