/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:39:09 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/23 14:49:28 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exit_ctrl_d(t_data *data)
{
	// printf("I AM HERE!\n");
	printf("\x1b[A\x1b[K%sexit\n", PROMPT_R);
	set_echo_ctl(1);
	rl_clear_history();
	free_env(data->env);
	exit(g_exit_status);
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
	if (!(check_line(line)))
		return (false);
	if (!(split_tokens(line, tokens)))
		return (false);
	if (!(token_identify(tokens, 0)))
		return (false);
	if (!(token_syntax(tokens)))
		return (free_tokens(tokens), false);
	if (!(variable_parser(tokens, data)))
		return (false);
	if (!(quote_trim(tokens)))
		return (false);
	if (!(assign_data_cmd(tokens, data)))
		return (false);
	data->pipe_len = tokens->pipe_count + 1;
	if (open_heredoc(data) && g_exit_status == 1) 
		return (true);
	// printf("exiting through heredoc\n");
	// if (g_exit_status == 1)
	 //Must re-evalute whether to keep this here with heredoc check
	assign_fd(data, 0);
	// return (printf("we returning out of the main \n"), true);
	assign_path(data);
	if (tokens != NULL)
		free_tokens(tokens);
	separator_op(data);
	return (true);
}

int	main(int ac, char **av, char **ev)
{
	t_data		data;
	t_tokens	tokens;
	char		*line;

	(void)av;
	g_exit_status = 0;
	if (ac != 1)
		exit(1);
	init_data(&data, ev);
	init_tokens(&tokens);
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			// printf("I AM HERE!\n");
			exit_ctrl_d(&data); // Signal PROBLEM quit with outfile 
		}
		if (line[0])
		{
			if (processor(line, &data, &tokens) && data.cmd)
				free_cmd_struct(data.cmd, data.pipe_len);
		}
		if (line)
			free(line);
	}
	return (g_exit_status);
}
