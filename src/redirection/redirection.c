/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:16:54 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/16 16:31:36 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	creat_outfile(char *outfile, int append)
{
	int	fd;
	int	w_mode;

	if (append == true)
		w_mode = O_APPEND;
	else
		w_mode = O_TRUNC;
	fd = open(outfile, O_WRONLY | O_CREAT | w_mode, 0644);
	if (fd == -1)
		return (error_system("open error"), CMD_ERROR);
	close(fd);
	return (CMD_OK);
}

static bool	access_outfile(t_data *data, int index, int i)
{
	while (data->cmd[index].outfiles[i] != NULL)
	{
		if (data->cmd[index].outfiles[i + 1] != NULL)
		{
			if (creat_outfile(data->cmd[index].outfiles[i],
					data->cmd[index].append) != CMD_OK)
				return (error_cmd(data->cmd[index].outfiles[i], F_DENIED),
					CMD_ERROR);
		}
		else if (access(data->cmd[index].outfiles[i], F_OK) == -1
			&& data->cmd[index].outfiles[i + 1] == NULL)
		{
			if (creat_outfile(data->cmd[index].outfiles[i],
					data->cmd[index].append) != CMD_OK)
				return (error_cmd(data->cmd[index].outfiles[i], F_DENIED),
					CMD_ERROR);
		}
		if (access(data->cmd[index].outfiles[i], W_OK) == -1)
			return (error_cmd(data->cmd[index].outfiles[i], F_DENIED),
				CMD_EXIT);
		i++;
	}
	return (CMD_OK);
}

int	check_access_files(t_data *data, int index, int i)
{
	while (data->cmd[index].infiles[i] != NULL)
	{
		if (access(data->cmd[index].infiles[i], F_OK) == -1
			&& data->cmd[index].infiles[i] != NULL)
			return (error_system("file not found"), CMD_EXIT);
		else if (access(data->cmd[index].infiles[i], R_OK) == -1
			&& data->cmd[index].infiles[i] != NULL)
			return (error_cmd(data->cmd[index].infiles[i], F_DENIED),
				CMD_EXIT);
		i++;
	}
	access_outfile(data, index, 0);
	return (CMD_OK);
}
