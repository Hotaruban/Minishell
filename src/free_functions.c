/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 00:21:25 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/17 21:28:49 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_2d_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	free_cmd_struct(t_cmd *cmd, int nb_cmd)
{
	int	i;

	i = 0;
	while (i < nb_cmd)
	{
		if (cmd[i].cmd != NULL)
			free_2d_array(cmd[i].cmd);
		if (cmd[i].limiters != NULL)
			free_2d_array(cmd[i].limiters);
		if (cmd[i].infiles != NULL)
			free_2d_array(cmd[i].infiles);
		if (cmd[i].outfiles != NULL)
			free_2d_array(cmd[i].outfiles);
		i++;
	}
	if (cmd)
		free(cmd);
}

void	free_tokens(t_tokens *tokens)
{
	if (tokens->tokens != NULL)
		free_2d_array (tokens->tokens);
	if (tokens->token_type != NULL)
		free(tokens->token_type);
	tokens->pipe_count = 0;
	tokens->token_count = 0;
	tokens->cmd_count = 0;
	if (tokens->arg_count != NULL)
		free(tokens->arg_count);
	if (tokens->heredoc_count != NULL)
		free(tokens->heredoc_count);
	if (tokens->infile_count != NULL)
		free(tokens->infile_count);
	if (tokens->outfile_count != NULL)
		free(tokens->outfile_count);
	if (tokens->append_count != NULL)
		free(tokens->append_count);
}

void	free_data_struct(t_data *data)
{
	data->status = 0;
	if (data->cmd)
		free_cmd_struct(data->cmd, data->pipe_len);
}
