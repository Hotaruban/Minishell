/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:40 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/19 20:18:13 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function cd_builtins is used to change the current working directory
to the directory specified in the path and update the environment.
If the path is not valid, the function returns 1.
If the path is valid, the function returns 0.
*/

static void	set_oldpwd(t_env *env, char *oldpwd)
{
	t_env	*tmp_env;

	tmp_env = env;
	while (tmp_env)
	{
		if (ft_strncmp(tmp_env->name, "OLDPWD=", 7) == 0)
		{
			free(tmp_env->name);
			tmp_env->name = ft_strdup(oldpwd);
			break ;
		}
		else if (tmp_env->next == NULL)
		{
			add_variable(tmp_env, oldpwd);
			break ;
		}
		tmp_env = tmp_env->next;
	}
	free(oldpwd);
}

static void	set_variable_pwd(t_env *env)
{
	t_env	*tmp_env;
	char	*tmp_pwd;
	char	*cwd;

	tmp_env = env;
	while (tmp_env)
	{
		if (ft_strncmp(tmp_env->name, "PWD=", 4) == 0)
		{
			tmp_pwd = ft_strjoin("OLDPWD=", tmp_env->name + 4);
			free(tmp_env->name);
			cwd = getcwd(NULL, 0);
			tmp_env->name = ft_strjoin("PWD=", cwd);
			free(cwd);
		}
		tmp_env = tmp_env->next;
	}
	set_oldpwd(env, tmp_pwd);
}

static int	change_directory(t_data *data, char *path, t_env *env)
{
	(void)data;
	if (chdir(path) == -1)
	{
		error_cmd_msg("cd ", path, NO_FILE);
		return (CMD_ERROR);
	}
	set_variable_pwd(env);
	if (path != NULL)
		free(path);
	return (CMD_OK);
}

int	ft_cd(t_data *data, int index)
{
	char	*path;

	path = NULL;
	if (!data->cmd[index].cmd || !data->cmd[index].cmd[1]
		|| ft_isspace(data->cmd[index].cmd[1][0])
		|| data->cmd[index].cmd[1][0] == '\0'
		|| ft_strncmp(data->cmd[index].cmd[1], "--", 3) == 0)
	{
		path = get_env_value("HOME", &data->env, 4, g_exit_status);
		return (change_directory(data, path, data->env));
	}
	if (data->cmd[index].cmd[2])
		return (error_cmd(data->cmd[index].cmd[0],
				TOO_MANY_ARG), CMD_ERROR);
	if (ft_strncmp(data->cmd[index].cmd[1], "-", 2) == 0)
	{
		path = get_env_value("OLDPWD", &data->env, 6, 0);
		printf("%s\n", path);
		if (!path)
			return (error_cmd(data->cmd[index].cmd[0],
					"OLDPWD not set"), CMD_ERROR);
		return (change_directory(data, path, data->env));
	}
	path = ft_strdup(data->cmd[index].cmd[1]);
	return (change_directory(data, path, data->env));
}
