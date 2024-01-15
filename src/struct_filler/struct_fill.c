/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct_fill.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 19:46:32 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 00:09:00 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_cmd(t_cmd *cmd, int j, t_tokens *tokens)
{
	if (j < (tokens->pipe_count) && tokens->pipe_count > 0)
		cmd->pipe_out = TRUE;
	else
		cmd->pipe_out = FALSE;
	if (j > 0)
		cmd->pipe_in = TRUE;
	else
		cmd->pipe_in = FALSE;
	if (tokens->heredoc_count[j] != 0 && tokens->heredoc_count[j] > 0
		&& tokens->infile_count[j] == 0)
		cmd->here_doc_in = TRUE;
	else
		cmd->here_doc_in = FALSE;
	if ((tokens->infile_count[j] != 0 && tokens->infile_count[j] > 0)
		|| cmd->here_doc_in == TRUE)
		cmd->file_in = TRUE;
	else
		cmd->file_in = FALSE;
	if (tokens->append_count[j] != 0 && tokens->append_count[j] > 0)						/*Check if its only if it appears last*/
		cmd->append = TRUE;
	else
		cmd->append = FALSE;
	if ((tokens->outfile_count[j] != 0 && tokens->outfile_count[j] > 0)
		|| cmd->append == TRUE)
		cmd->file_out = TRUE;
	else
		cmd->file_out = FALSE;
	cmd->here_doc_fd = -1;
	cmd->cmd = NULL;
	cmd->limiters = NULL;
	cmd->infiles = NULL;
	cmd->outfiles = NULL;
}

void	mallocer(t_cmd *cmd, t_tokens *tokens, int j)
{
	if (cmd->cmd == NULL)
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

void	sort_rdrt(t_cmd *cmd, char *token, char *next_token)
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

void	find_last_rdrt(t_cmd *cmd, t_tokens *tokens, int i)
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

void	identify_2(t_cmd *cmd, t_tokens *tokens, int j, int *i)
{
	int	x;

	x = 0;
	init_cmd(cmd, j, tokens);
	mallocer(cmd, tokens, j);
	while (*i < tokens->token_count && tokens->token_type[*i] != e_pipe)
	{
		if (tokens->token_type[*i] == e_rdrt)
		{
			sort_rdrt(cmd, tokens->tokens[*i], tokens->tokens[*i + 1]);
			*i += 2;
																/*Only works bc token_syntax makes sure no pipe or rdrt comes after*/
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

bool	struct_fill(t_tokens *tokens, t_data *data)						/*TOO MANY FUNCTION IN FILE*/
{
	int		i;
	int		j;
	t_cmd	*cmd_struct;

	cmd_struct = (t_cmd *)malloc(sizeof(t_cmd) * (tokens->pipe_count + 2));
	i = 0;
	j = 0;
	while (j < tokens->pipe_count + 1)
	{
		identify_2(&cmd_struct[j], tokens, j, &i);
		i += 1; 														/*So its not stuck on pipe*/
		j++;
	}
	data->cmd = cmd_struct;
	return (1);
}
