/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: whendrik <whendrik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:39:09 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 21:49:23 by whendrik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static bool	assign_data_cmd(t_tokens *tokens, t_data *data)
{
	t_cmd	*cmd_struct;
	int		i;
	int		j;

	cmd_struct = (t_cmd *)malloc(sizeof(t_cmd) * (tokens->pipe_count + 2));
	if (cmd_struct == NULL)
		return (error_system(MALLOC_ERROR), false);
	i = 0;
	j = 0;
	while (j < tokens->pipe_count + 1)
	{
		init_data_cmd(&cmd_struct[j]);
		identify_cmd(&cmd_struct[j], tokens, j, &i);
		i += 1;
		j++;
	}
	data->cmd = cmd_struct;
	return (true);
}

static bool	processor(char *line, t_data *data, t_tokens *tokens)
{
	if (!(check_line(line)))
		return (false);
	if(!(split_tokens(line, tokens)))
		return (false);
	if (!(token_identify(tokens)))
		return (false);
	if (!(token_syntax(tokens)))
		return(free_tokens(tokens), false);
	if (!(variable_parser(tokens, data)))
		return(false);
	if (!(quote_trim(tokens)))
		return (false);
	if (!(assign_data_cmd(tokens, data)))
		return (false);
	data->pipe_len = tokens->pipe_count + 1;
	if (tokens != NULL)
		free_tokens(tokens);
	if ((separator_op(data) != CMD_OK))
		return (false);
	if (data->cmd)
		free_cmd_struct(data->cmd);
	return (true);
}

int main(int ac, char** av, char **ev)
{
	t_data		data;
	t_tokens	tokens;
	t_env		*env;
	char		*line;
	
	(void)av;
	if (ac != 1)
		exit(1);
	env = set_env(ev);
	init_data(&data, env);
	init_tokens(&tokens);
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("\x1b[A\x1b[K%sexit\n", PROMPT);
			break ;
		}
		if (*line)
			add_history(line);
		if (!(processor(line, &data, &tokens)))	/* TO FIX OR REMOVE */
		{
			// free_data_struct(&data);
		}
		free(line);
	}
	set_echo_ctl(1);
	rl_clear_history();
	return (0);
}