/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_acces_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/17 09:00:16 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 10:12:44 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_infile(char *infile)
{
	int	status;
	int	fd;

	status = CMD_OK;
	fd = access(infile, F_OK);
	if (fd == -1)
	{
		printf("%s%s: %s\n", PROMPT, infile, NO_FILE);
		return (CMD_ERROR);
	}
	fd = access(infile, R_OK);
	if (fd == -1)
	{
		printf("%s%s: %s\n", PROMPT, infile, F_DENIED);
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
	status = CMD_OK;
	fd = access(outfile, W_OK);
	if (fd == -1)
	{
		fd = open(outfile, O_WRONLY | O_CREAT | w_mode, 0644);
		if (fd == -1)
		{
			printf("%s%s: %s\n", PROMPT, outfile, F_DENIED);
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

	status = CMD_OK;
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
	while ((data->cmd[index].file_in == true
			&& (data->cmd[index].here_doc_in == false))
		|| data->cmd[index].file_out == true)
	{
		status = access_infile_outfile(data, index);
		index++;
	}
	return (status);
}
