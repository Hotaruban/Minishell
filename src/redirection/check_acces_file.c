/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_acces_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:00:16 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/18 19:19:22 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_infile(char *infile)
{
	int	status;
	int	fd;

	status = g_exit_status;
	fd = access(infile, F_OK);
	if (fd == -1)
	{
		error_file_msg(infile, NO_FILE);
		return (CMD_ERROR);
	}
	fd = access(infile, R_OK);
	if (fd == -1)
	{
		error_file_msg(infile, F_DENIED);
		status = CMD_ERROR;
	}
	return (status);
}

static int	open_outfile(char *outfile, bool flag)
{
	int	status;
	int	fd;
	int	w_mode;

	w_mode = O_TRUNC;
	if (flag == true)
		w_mode = O_APPEND;
	status = g_exit_status;
	fd = access(outfile, W_OK);
	if (fd == -1)
	{
		fd = open(outfile, O_WRONLY | O_CREAT | w_mode, 0644);
		if (fd == -1)
		{
			error_file_msg(outfile, F_DENIED);
			return (CMD_ERROR);
		}
	}
	if (fd == 0)
		fd = open(outfile, O_WRONLY | O_CREAT | w_mode, 0644);
	close(fd);
	return (status);
}

static int	access_infile_outfile(t_data *data, int index)
{
	int	status;
	int	i;

	status = g_exit_status;
	i = 0;
	if (data->cmd[index].file_in == true
		&& data->cmd[index].here_doc_in == false)
	{
		while (data->cmd[index].infiles[i])
		{
			status = open_infile(data->cmd[index].infiles[i]);
			i++;
		}
		i = 0;
	}
	else
	{
		while (data->cmd[index].outfiles[i])
		{
			status = open_outfile(data->cmd[index].outfiles[i],
					data->cmd[index].append);
			i++;
		}
	}
	return (status);
}

int	check_acces_file(t_data *data, int index)
{
	int	status;
	int	i;

	i = 0;
	status = 0;
	while ((int)data->pipe_len > i)
	{
		status = access_infile_outfile(data, index);
		index++;
		i++;
	}
	return (status);
}
