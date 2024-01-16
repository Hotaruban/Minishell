/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 16:25:44 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/16 11:43:47 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	init_cmd(t_cmd *cmd, int j, t_tokens *tokens)
{
	if (j < (tokens->pipe_count) && tokens->pipe_count > 0)
		cmd->pipe_out = TRUE;
	if (j > 0)
		cmd->pipe_in = TRUE;
	if (tokens->heredoc_count[j] != 0 && tokens->heredoc_count[j] > 0
		&& tokens->infile_count[j] == 0)
		cmd->here_doc_in = TRUE;
	if ((tokens->infile_count[j] != 0 && tokens->infile_count[j] > 0)
		|| cmd->here_doc_in == TRUE)
		cmd->file_in = TRUE;
	if (tokens->append_count[j] != 0 && tokens->append_count[j] > 0)
		cmd->append = TRUE;
	if ((tokens->outfile_count[j] != 0 && tokens->outfile_count[j] > 0)
		|| cmd->append == TRUE)
		cmd->file_out = TRUE;
}

static void	malloc_cmd(t_cmd *cmd, t_tokens *tokens, int j)
{
	cmd->cmd = (char **)ft_calloc(sizeof(char *),
			((tokens->arg_count[j]) + 1));
	cmd->limiters = (char **)ft_calloc(sizeof(char *),
			(tokens->heredoc_count[j] + 1));
	cmd->nb_heredocs = tokens->heredoc_count[j];
	cmd->infiles = (char **)ft_calloc(sizeof(char *),
			(tokens->infile_count[j] + 1));
	cmd->outfiles = (char **)ft_calloc(sizeof(char *),
			(tokens->outfile_count[j] + tokens->append_count[j] + 1));
}

static void	sort_rdrt(t_cmd *cmd, char *token, char *next_token)
{
	int	i;

	i = 0;
	if (ft_strncmp("<<", token, 2) == 0)
	{
		while ((cmd->limiters[i]))
			i++;
		cmd->limiters[i] = ft_strdup(next_token);
	}
	else if (*token == '<')
	{
		while ((cmd->infiles[i]))
			i++;
		cmd->infiles[i] = ft_strdup(next_token);
	}
	else
	{
		while ((cmd->outfiles[i]))
			i++;
		cmd->outfiles[i] = ft_strdup(next_token);
	}
}

static void	find_last_rdrt(t_cmd *cmd, t_tokens *tokens, int i)
{
	while (tokens->token_type[i] != e_pipe && i != 0)
	{
		if (!(ft_strncmp(tokens->tokens[i], "<<", 2)))
		{
			cmd->here_doc_in = TRUE;
			break ;
		}
		else if (!(ft_strncmp(tokens->tokens[i], "<", 1)))
		{
			cmd->file_in = TRUE;
			break ;
		}
		i--;
	}
}

void	identify_cmd(t_cmd *cmd, t_tokens *tokens, int j, int *i)
{
	int	x;

	x = 0;
	init_cmd(cmd, j, tokens);
	malloc_cmd(cmd, tokens, j);
	while (*i < tokens->token_count && tokens->token_type[*i] != e_pipe)
	{
		if (tokens->token_type[*i] == e_rdrt)
		{
			sort_rdrt(cmd, tokens->tokens[*i], tokens->tokens[*i + 1]);
			*i += 2;
		}
		else if (tokens->token_type[*i] == e_argument)
		{
			cmd->cmd[x] = ft_strdup(tokens->tokens[*i]);
			*i += 1;
			x++;
		}
	}
	find_last_rdrt(cmd, tokens, *i - 1);
}
