/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:42:14 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/22 01:14:01 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_data(t_data *data, char **ev)
{
	data->cmd = NULL;
	data->env = set_env(ev);
	data->ev_array = NULL;
	data->pipe_len = 0;
	data->pipefd[1] = -1;
	init_signal(data);
	set_echo_ctl(0);
}

void	init_tokens(t_tokens *tokens)
{
	tokens->tokens = NULL;
	tokens->token_type = NULL;
	tokens->pipe_count = 0;
	tokens->token_count = 0;
	tokens->cmd_count = 0;
	tokens->arg_count = NULL;
	tokens->heredoc_count = NULL;
	tokens->infile_count = NULL;
	tokens->outfile_count = NULL;
	tokens->append_count = NULL;
}

void	init_data_cmd(t_cmd *cmd)
{
	cmd->cmd = NULL;
	cmd->files = NULL;
	cmd->type = NULL;
	cmd->status = CMD_OK;
	cmd->error_str = NULL;
	cmd->msg_error = NULL;
	cmd->path = NULL;
	cmd->fd_infile = -2;
	cmd->fd_outfile = -2;
	cmd->pipe_in = FALSE;			// Check from here what is useless
	cmd->pipe_out = FALSE;
	cmd->here_doc_in = FALSE;
	cmd->here_doc_fd = -1;
	cmd->nb_heredocs = 0;
	cmd->limiters = NULL;
	cmd->file_in = FALSE;
	cmd->file_out = FALSE;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->append = FALSE;
}
