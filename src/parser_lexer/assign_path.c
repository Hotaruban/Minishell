/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 19:43:10 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/20 20:51:26 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	check_cmd_accessible(t_data *data, int index)
{
	if (ft_strncmp(data->cmd[index].cmd[0], "../", 3) == 0 || ft_strncmp(data->cmd[index].cmd[0], "./", 2) == 0)
	{
		if (access(data->cmd[index].cmd[0], F_OK) == -1)
		{
			error_cmd(data->cmd[index].cmd[0], NO_FILE); // "THis will dissapear" - JErermy
			exit(CMD_NOT_FOUND);
		}
		if (access(data->cmd[index].cmd[0], X_OK) == -1)
		{
			error_cmd(data->cmd[index].cmd[0], F_DENIED);// "THis will dissapear" - JErermy
			exit(CMD_NOT_EXEC);
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
		error_cmd(data->cmd[index].cmd[0], NO_FILE); //"THis will be changed also" - Sick jeremy
		exit(CMD_NOT_FOUND);
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

	path_array = get_env(data, env, index);
	i = -1;
	while (path_array[++i] != NULL)
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
	}
	free_2d_array(path_array);
	return (path);
}

static char	*get_path(t_data *data, char **env, int index)
{
	char	*path;

	path = NULL;
	if (data->cmd[index].cmd[0][0] == '/')
		path = ft_strdup(data->cmd[index].cmd[0]);
	else
		path = check_path(data, env, index);
	if (path == NULL)
	{
		error_cmd(data->cmd[index].cmd[0], NO_CMD);// "THIsi wll be rmeovd also" - Jeremy in french
		exit(CMD_NOT_FOUND);
	}
	if (access(path, F_OK) == -1)// "THis too"
	{
		error_cmd(path, NO_FILE);
		free(path);
		exit(CMD_NOT_FOUND);
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
		if (is_builtins(data, i) == false)
		{
			if (check_cmd_accessible(data, i) == true)
			data->cmd[i].path = (char *)data->cmd[i].cmd[0];
			else
				data->cmd[i].path = get_path(data, env, i);
			if (data->cmd[i].path == NULL)
			{
				data->cmd[i].status = CMD_NOT_FOUND;
				data->cmd[i].error_str = ft_strdup(data->cmd[i].cmd[0]);	
			}
		}
		i++;
	}
}
