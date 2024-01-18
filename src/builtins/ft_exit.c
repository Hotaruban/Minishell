/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:55 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/18 18:19:23 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function ft_exit is used to exit the minishell.
It returns the exit status.
*/

static int	quit_and_clean(t_data *data)
{
	rl_clear_history();
	free_env(data->env);
	free_data_struct(data);
	exit (g_exit_status);
}

static int	check_input_exit(t_data *data, int index)
{
	if (data->cmd[index].file_in == true)
		return (CMD_ERROR);
	if (data->cmd[index].cmd[1] != NULL)
	{
		if (data->cmd[index].cmd[2] != NULL
			&& check_long_long(data->cmd[index].cmd[1]) == true)
		{
			error_exit_msg(NULL, TOO_MANY_ARG, 1);
			return (CMD_ERROR);
		}
	}
	return (CMD_OK);
}

static bool	ft_isnumber(char *arg)
{
	int	i;

	i = 0;
	if (arg[i] == '-' || arg[i] == '+')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (false);
		i++;
	}
	return (true);
}

static int	get_exit_status(t_data *data, char *arg, int flag)
{
	int			status;

	status = 0;
	if (arg)
	{
		if (check_long_long(arg) == false || !ft_isnumber(arg))
		{
			error_exit_msg(arg, NUM_ARG, flag);
			g_exit_status = 255;
			quit_and_clean(data);
		}
		else
			status = (ft_atoll(arg) % 256);
	}
	else
		status = CMD_OK;
	return (status);
}

int	ft_exit(t_data *data, int index)
{
	int	flag;

	flag = 0;
	if (data->pipe_len > 1)
		flag = 1;
	g_exit_status = check_input_exit(data, index);
	if (g_exit_status != CMD_OK)
		return (g_exit_status);
	g_exit_status = get_exit_status(data, data->cmd[0].cmd[1], flag);
	if (data->cmd[index].pipe_out == true
		|| data->cmd[index].pipe_in == true)
		return (CMD_ERROR);
	error_exit_msg(NULL, NULL, flag);
	quit_and_clean(data);
	return (g_exit_status);
}
