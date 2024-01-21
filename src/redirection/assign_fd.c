/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 15:17:27 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/21 16:18:58 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	open_infile(char *infile, t_cmd *cmd)
{
	// int	status;
	int	fd;

	// status = g_exit_status;
	fd = access(infile, F_OK);
	if (fd == -1)
	{
		cmd->status = CMD_ERROR;
		cmd->error_str = ft_strdup(infile);
		cmd->msg_error = NO_FILE; //NOT SURE IF MEMORY IS ALREADY ALLOCATED FOR MACRO
		// error_file_msg(infile, NO_FILE);
		// return (CMD_ERROR);
	}
	if (cmd->status != CMD_OK)
	{
		fd = access(infile, R_OK);
		if (fd == -1)
		{
			// error_file_msg(infile, F_DENIED);
			cmd->status = CMD_ERROR;
			cmd->error_str = ft_strdup(infile);
			cmd->msg_error = F_DENIED;//NOT SURE IF MEMORY IS ALREADY ALLOCATED FOR MACRO 
		}
	}
}

static void	open_outfile(char *outfile, t_rdrt_type type, t_cmd *cmd)
{
	int	fd;
	int	w_mode;

	w_mode = O_TRUNC;
	if (type == e_append)
		w_mode = O_APPEND;
	// status = g_exit_status;
	fd = access(outfile, W_OK);
	if (fd == -1)
	{
		fd = open(outfile, O_WRONLY | O_CREAT | w_mode, 0644);
		if (fd == -1)
		{
			cmd->status = CMD_ERROR;
			cmd->error_str = ft_strdup(outfile);
			cmd->msg_error = F_DENIED;//NOT SURE IF MEMORY IS ALREADY ALLOCATED FOR MACRO 
		}
	}
	// if (fd == 0)
	// 	fd = open(outfile, O_WRONLY | O_CREAT | w_mode, 0644); //MIGHT NEED LATER
	close(fd);
	// return (status);
}

static void	access_infile_outfile(t_cmd *cmd)
{
	int	i;

	// cmd->status = CMD_OK;
	i = 0;
		// while (data->cmd[index].infiles[i] && status != CMD_ERROR)
	while (cmd->files[i] && cmd->status != CMD_ERROR)
	{
		if (cmd->type[i] == e_infile)
			open_infile(cmd->files[i], cmd);
		else if(cmd->type[i] == e_outfile | cmd->type[i] == e_append)
			open_outfile(cmd->files[i], cmd->type[i], cmd);
		i++;
	}
	// 	i = 0;
	// }
	// else
	// {
	// 	while (data->cmd[index].outfiles[i] && status != CMD_ERROR)
	// 	{
	// 		status = open_outfile(data->cmd[index].outfiles[i],
	// 				data->cmd[index].append);
	// 		i++;
	// 	}
	// }
	// return (status);
}

void	find_last_infile_outfile(t_cmd *cmd)
{
	int	i;
	int	ptr_out;
	int	ptr_in;

	i = 0;
	ptr_out = 0;
	ptr_in = 0;
	while (cmd->files[i])
	{
		if (cmd->type[i] == e_infile || cmd->type[i] == e_heredoc)
			ptr_in = i;
		else if (cmd->type[i] == e_outfile || cmd->type[i] == e_append)
			ptr_out = i;
		i++;
	}
	if (ptr_in != 0)
	{
		if (cmd->type[ptr_in] == e_heredoc)
			cmd->fd_infile = cmd->here_doc_fd;
		cmd->fd_infile = open(cmd->files[ptr_in], O_RDONLY);
	}
	if (ptr_out != 0)
	{
		if (cmd->type[ptr_out] == e_append)
			cmd->fd_outfile = open(cmd->files[ptr_out], O_WRONLY | O_CREAT | O_APPEND, 0644);
		cmd->fd_outfile = open(cmd->files[ptr_out], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
}

void	assign_fd(t_data *data, int index)
{
	// int	status;
	// status = 0;
	while ((int)data->pipe_len > index)
	{
		access_infile_outfile(&data->cmd[index]);
		if (data->cmd[index].status == CMD_OK)
			find_last_infile_outfile(&data->cmd[index]);
		index++;
	}
	// return (status);
}
