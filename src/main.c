/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:39:09 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/16 23:27:50 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exit_ctrl_d(t_data *data)
{
	int	status;

	status = data->status;
	printf("\x1b[A\x1b[K%sexit\n", PROMPT);
	set_echo_ctl(1);
	rl_clear_history();
	free_env(data->env);
	exit(status);
}

static bool	assign_data_cmd(t_tokens *tokens, t_data *data)
{
	t_cmd	*cmd_struct;
	int		i;
	int		j;

	cmd_struct = (t_cmd *)malloc(sizeof(t_cmd) * (tokens->pipe_count + 1));
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
	if (*line)
		add_history(line);
	printf("HERE--\n");
	if (!(check_line(line)))
		return (printf("In checkline false\n"), false);
	printf("HERE--\n");
	if (!(split_tokens(line, tokens)))
		return (false);
	printf("HERE--\n");
	if (!(token_identify(tokens, 0)))
		return (false);
	printf("HERE--\n");
	if (!(token_syntax(tokens)))
		return (free_tokens(tokens), false);
	printf("HERE--\n");
	if (!(variable_parser(tokens, data)))
		return (false);
	printf("HERE--\n");
	if (!(quote_trim(tokens)))
		return (false);
	printf("HERE--\n");
	if (!(assign_data_cmd(tokens, data)))
		return (false);
	data->pipe_len = tokens->pipe_count + 1;
	if (tokens != NULL)
		free_tokens(tokens);
	if ((separator_op(data) != CMD_OK))
		return (false);
	return (true);
}

int	main(int ac, char **av, char **ev)
{
	t_data		data;
	t_tokens	tokens;
	char		*line;

	(void)av;
	if (ac != 1)
		exit(1);
	init_data(&data, ev);
	init_tokens(&tokens);
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			exit_ctrl_d(&data);
		}
		if (line[0])
			processor(line, &data, &tokens);
		if (data.cmd)
			free_cmd_struct(data.cmd, data.pipe_len);
		if (line)
			free(line);
	}
	return (0);
}
