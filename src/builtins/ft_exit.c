/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 13:15:55 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 11:02:50 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
The function ft_exit is used to exit the minishell.
It returns the exit status.
*/

static int	quit_and_clean(t_data *data, int status)
{
	rl_clear_history();
	free_env(data->env);
	free_data_struct(data);
	exit (status);
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
			error_exit_msg(NULL, TOO_MANY_ARG);
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

static int	get_exit_status(t_data *data, char *arg)
{
	int			status;

	status = 0;
	if (arg)
	{
		if (check_long_long(arg) == false || !ft_isnumber(arg))
		{
			error_exit_msg(arg, NUM_ARG);
			quit_and_clean(data, 255);
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
	int	status;

	status = check_input_exit(data, index);
	if (status != CMD_OK)
		return (status);
	status = get_exit_status(data, data->cmd[0].cmd[1]);
	if (data->cmd[index].pipe_out == true
		|| data->cmd[index].pipe_in == true)
		return (CMD_ERROR);
	error_exit_msg(NULL, NULL);
	quit_and_clean(data, status);
	return (status);
}
