/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:16:00 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/18 16:12:46 by jhurpy           ###   ########.fr       */
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

static void	set_var_env(t_env *env, char **var, int index)
{
	char	*var_tmp;

	while (env)
	{
		var_tmp = ft_substr(var[index], 0, len_variable(var[index]));
		if (ft_strncmp(env->name, var_tmp, len_variable(var[index])) == 0)
		{
			free(env->name);
			env->name = ft_strdup(var[index]);
			free (var_tmp);
			break ;
		}
		else if (env->next == NULL
			&& len_variable(var[index]) <= (int)ft_strlen(var[index]))
			add_variable(env, var[index]);
		if (var_tmp != NULL)
			free(var_tmp);
		env = env->next;
	}
}

static void	find_variable(t_env *env, char **var)
{
	t_env	*tmp_env;
	int		i;

	i = 0;
	while (var[i])
	{
		tmp_env = env;
		set_var_env(tmp_env, var, i);
		i++;
	}
}

int	ft_export(t_data *data, char **env, int index)
{
	int	status;
	int	i;

	status = CMD_OK;
	if (data->cmd[index].cmd[1] == NULL)
		return (print_env(env, 1), status);
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
			error_cmd_msg(data->cmd[index].cmd[0], data->cmd[index].cmd[i],
				NO_VALID_ID);
			status = CMD_ERROR;
		}
		i++;
	}
	find_variable(data->env, data->cmd[index].cmd + 1);
	return (status);
}
