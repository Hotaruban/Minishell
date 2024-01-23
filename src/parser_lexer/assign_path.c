/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:43:10 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/23 23:57:51 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	check_cmd_accessible(t_data *data, int index)
{
	if (ft_strncmp(data->cmd[index].cmd[0], "../", 3) == 0
		|| ft_strncmp(data->cmd[index].cmd[0], "./", 2) == 0)
	{
		if (access(data->cmd[index].cmd[0], F_OK) == -1)
		{
			data->cmd[index].status = 127;
			data->cmd[index].error_str = ft_strdup(data->cmd[index].cmd[0]);
			data->cmd[index].msg_error = NO_FILE;
			return (false);
		}
		if (access(data->cmd[index].cmd[0], X_OK) == -1)
		{
			data->cmd[index].status = 126;
			data->cmd[index].error_str = ft_strdup(data->cmd[index].cmd[0]);
			data->cmd[index].msg_error = F_DENIED;
			return (false);
		}
		return (true);
	}
	return (false);
}

static char	**get_env(t_data *data, char **env, int index)
{
	char	**array;

	array = NULL;
	while (*env != NULL && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (*env == NULL)
	{
		data->cmd[index].status = 127;
		data->cmd[index].error_str = data->cmd[index].cmd[0];
		data->cmd[index].msg_error = NO_FILE;
		return (NULL);
	}
	if (ft_strncmp(*env, "PATH=", 5) == 0)
	{
		array = ft_split(*env + 5, ':');
		if (array == NULL)
		{
			error_system(MALLOC_ERROR);
			exit(CMD_ERROR);
		}
	}
	return (array);
}

static char	*check_path(t_data *data, char **env, int index)
{
	char	**path_array;
	char	*tmp_path;
	char	*path;
	int		i;

	path = NULL;
	path_array = get_env(data, env, index);
	if (path_array == NULL)
	{
		data->cmd[index].status = CMD_NOT_FOUND;
		data->cmd[index].error_str = ft_strdup(data->cmd[index].cmd[0]);
		data->cmd[index].msg_error = NO_FILE;
		return (path);
	}
	i = 0;
	while (path_array[i] != NULL)
	{
		tmp_path = ft_strjoin(path_array[i], "/");
		path = ft_strjoin(tmp_path, data->cmd[index].cmd[0]);
		if (tmp_path != NULL)
			free(tmp_path);
		if (access(path, F_OK) == 0)
			break ;
		if (path != NULL)
			free(path);
		path = NULL;
		i++;
	}
	if (path == NULL)
	{
		data->cmd[index].status = CMD_NOT_FOUND;
		data->cmd[index].error_str = ft_strdup(data->cmd[index].cmd[0]);
		data->cmd[index].msg_error = NO_CMD;
	}
	free_2d_array(path_array);
	return (path);
}

static char	*get_path(t_data *data, char **env, int index)
{
	char	*path;

	path = NULL;
	if (data->cmd[index].cmd[0][0] == '/')
	{
		path = ft_strdup(data->cmd[index].cmd[0]);
		if (access(path, F_OK) == -1)
		{
			data->cmd[index].status = CMD_NOT_FOUND;
			data->cmd[index].error_str = ft_strdup(path);
			data->cmd[index].msg_error = NO_FILE;
			return (path);
		}
	}
	else
		path = check_path(data, env, index);
	if (path == NULL)
		return (path);
	if (access(path, F_OK) == -1)
	{
		data->cmd[index].status = CMD_NOT_FOUND;
		data->cmd[index].error_str = ft_strdup(path);
		data->cmd[index].msg_error = NO_CMD;
	}
	return (path);
}

void	assign_path(t_data *data)
{
	char	**env;
	int		i;

	i = 0;
	data->ev_array = env_array(data->env);
	env = data->ev_array;
	while (i < (int)data->pipe_len)
	{
		if (data->cmd[i].cmd[0] != NULL && is_builtins(data, i) == false
			&& data->cmd[i].status == CMD_OK)
		{
			if (check_cmd_accessible(data, i) == true
				&& data->cmd[i].status == 0)
				data->cmd[i].path = ft_strdup(data->cmd[i].cmd[0]);
			else if (data->cmd[i].status == 0)
				data->cmd[i].path = get_path(data, env, i);
		}
		i++;
	}
}
