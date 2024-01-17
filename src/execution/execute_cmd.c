/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/24 19:44:00 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 20:18:10 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	check_cmd_accessible(char **cmd, char **env)
{
	if (ft_strncmp(cmd[0], "../", 3) == 0 || ft_strncmp(cmd[0], "./", 2) == 0)
	{
		if (access(cmd[0], F_OK) == -1)
		{
			error_cmd(cmd[0], NO_FILE);
			free_2d_array(env);
			exit(CMD_NOT_FOUND);
		}
		if (access(cmd[0], X_OK) == -1)
		{
			error_cmd(cmd[0], F_DENIED);
			free_2d_array(env);
			exit(CMD_NOT_EXEC);
		}
		return (true);
	}
	return (false);
}

static char	**get_env(char **env)
{
	char	**array;

	array = NULL;
	while (*env != NULL && ft_strncmp(*env, "PATH=", 5) != 0)
		env++;
	if (*env == NULL)
	{
		error_cmd("PATH", NO_FILE);
		exit(CMD_NOT_FOUND);
	}
	if (ft_strncmp(*env, "PATH=", 5) == 0)
	{
		array = ft_split(*env + 5, ':');
		if (array == NULL)
		{
			error_system(MALLOC_ERROR);
			free_2d_array(env);
			exit(CMD_NOT_EXEC);
		}
	}
	return (array);
}

static char	*check_path(t_data *data, char *av, char **env)
{
	char	**path_array;
	char	*tmp_path;
	char	*path;
	int		i;

	path_array = get_env(env);
	i = -1;
	(void)data;
	while (path_array[++i] != NULL)
	{
		tmp_path = ft_strjoin(path_array[i], "/");
		path = ft_strjoin(tmp_path, av);
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

static char	*get_path(t_data *data, char **cmd, char **env)
{
	char	*path;

	path = NULL;
	if (cmd[0][0] == '/')
		path = ft_strdup(cmd[0]);
	else
		path = check_path(data, cmd[0], env);
	if (path == NULL)
	{
		error_cmd(cmd[0], NO_CMD);
		free_data_struct(data);
		free_2d_array(env);
		exit(CMD_NOT_FOUND);
	}
	if (access(path, F_OK) == -1)
	{
		free_data_struct(data);
		error_cmd(path, NO_FILE);
		free(path);
		free_2d_array(env);
		exit(CMD_NOT_FOUND);
	}
	return (path);
}

void	execute_cmd(t_data *data, char **cmd, char **env)
{
	char	*path;
	DIR		*dir;

	if (cmd == NULL || cmd[0][0] == '\0')
	{
		error_cmd(cmd[0], NO_CMD);
		exit(CMD_NOT_FOUND);
	}
	dir = opendir(cmd[0]);
	if (dir != NULL)
	{
		error_cmd(cmd[0], IS_DIR);
		closedir(dir);
		free_data_struct(data);
		free_2d_array(env);
		exit (126);
	}
	// free_data_struct(data);
	// 		exit (0);
	if (check_cmd_accessible(cmd, env) == true)
	{
		path = cmd[0];
	}
	else
	{
		path = get_path(data, cmd, env);
	}
	if (execve(path, cmd, env) == -1)
	{
		error_system("execve failed");
		exit (CMD_ERROR);
	}
}
