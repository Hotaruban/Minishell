/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_infiles.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 14:11:22 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 15:02:49 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	open_infile(char *infile)
{
	int	fd_in;
	int	tmpfd[2];

	fd_in = open(infile, O_RDONLY);
	if (fd_in == -1)
	{
		if (pipe(tmpfd) == -1)
			error_system(PIPE_ERROR);
		dup_files(STDIN_FILENO, tmpfd[0]);
	}
	else
	{
		dup_files(fd_in, STDIN_FILENO);
		close(fd_in);
	}
}

void	redir_infiles(t_data *data, int index)
{
	int	i;

	i = 0;
	if (data->cmd[index].file_in == true)
	{
		while (data->cmd[index].infiles[i + 1] != NULL)
			i++;
		open_infile(data->cmd[index].infiles[i]);
	}
}
