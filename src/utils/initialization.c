/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:42:14 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/20 20:03:45 by whendrik         ###   ########.fr       */
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
	cmd->pipe_out = FALSE;
	cmd->pipe_in = FALSE;
	cmd->here_doc_in = FALSE;
	cmd->file_in = FALSE;
	cmd->append = FALSE;
	cmd->file_out = FALSE;
	cmd->here_doc_fd = -1;
	cmd->cmd = NULL;
	cmd->limiters = NULL;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
	cmd->files = NULL;
	cmd->type = NULL;
	cmd->error_str = NULL;
	cmd->path = NULL;
	cmd->status = CMD_OK;
}
