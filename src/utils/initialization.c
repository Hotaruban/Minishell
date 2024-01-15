/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:42:14 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/15 16:44:42 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void init_data(t_data *data, t_env *env)
{
	data->cmd = NULL;
	data->env = env;
	data->status = 0;
	data->pipe_len = 0;
	data->pipefd[1] = -1;
	if (!(set_termios(&data->term)))
		printf("Signal fail --- FAIL INIT DATA ---"); 		/*TOO REMOVE*/
	set_signal();                                           /*What is it? What that's doing in Initialization?*/
}

void init_tokens(t_tokens *tokens)
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

void    init_data_cmd(t_cmd *cmd)
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
}
