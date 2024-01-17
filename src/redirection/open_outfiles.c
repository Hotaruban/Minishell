/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_outfiles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:11:14 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 11:41:19 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	open_outfile(char *outfile, bool append)
{
	int	fd_out;
	int	w_mode;

	if (append == true)
		w_mode = O_APPEND;
	else
		w_mode = O_TRUNC;
	fd_out = open(outfile, O_WRONLY | O_CREAT | w_mode, 0644);
	dup_files(fd_out, STDOUT_FILENO);
	close(fd_out);
}

void	redir_outfiles(t_data *data, int index)
{
	int	i;

	i = 0;
	if (data->cmd[index].file_out == true)
	{
		while (data->cmd[index].outfiles[i + 1] != NULL)
			i++;
		open_outfile(data->cmd[index].outfiles[i], data->cmd[index].append);
	}
}
