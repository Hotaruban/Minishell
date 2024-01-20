/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/14 23:20:14 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/20 23:40:37 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	redirection_heredoc(t_data *data, int index)
// {
// 	if (data->cmd[index].here_doc_in == true)
// 	{
// 		if (dup_files(data->cmd[index].here_doc_fd, STDIN_FILENO) != CMD_OK)
// 			return (CMD_ERROR);
// 		else
// 			return (close (data->cmd[index].here_doc_fd), CMD_OK);
// 	}
// 	return (CMD_OK);
// }

// int	redirection_pipes(t_data *data, int index)
// {
// 	if (data->cmd[index].pipe_out == true && data->cmd[index].file_out == false)
// 	{
// 		if (dup_files(data->pipefd[1], STDOUT_FILENO) != CMD_OK)
// 			return (CMD_ERROR);
// 	}
// 	close(data->pipefd[0]);
// 	close(data->pipefd[1]);
// 	return (CMD_OK);
// }
