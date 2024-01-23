/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:39:09 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/24 02:09:56 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	exit_ctrl_d(t_data *data)
{
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
	printf("tokens->pipe_count + 1 = %d\n", tokens->pipe_count + 1); // HERE CAN BE THE SEGMENTATION FAULT (after execute command line 70 of main first step len = 2 then for 1 commande len = 2)
	while (j < tokens->pipe_count + 1)
	{
		printf("Number of j = %d\n", j);
		init_data_cmd(&cmd_struct[j]);
		printf("HERE WE QUIT 1!\n");
		identify_cmd(&cmd_struct[j], tokens, j, &i); // SEGFAULT IN THIS FUNCTION
		printf("HERE WE QUIT 2!\n");
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
	printf("PRINTF BEFORE - PROCESSOR LINE 50\n");
	if (!(check_line(line)))
		return (false);
	printf("PRINTF BEFORE - PROCESSOR LINE 53\n");
	if (!(split_tokens(line, tokens)))
		return (false);
	printf("PRINTF BEFORE - PROCESSOR LINE 56\n");
	if (!(token_identify(tokens, 0)))
		return (false);
	printf("PRINTF BEFORE - PROCESSOR LINE 59\n");
	if (!(token_syntax(tokens)))
		return (free_tokens(tokens), false);
	printf("PRINTF BEFORE - PROCESSOR LINE 62\n");
	if (!(variable_parser(tokens, data)))
		return (false);
	printf("PRINTF BEFORE - PROCESSOR LINE 65\n");
	if (!(quote_trim(tokens)))
		return (false);
	printf("PRINTF BEFORE - PROCESSOR LINE 68\n");
	if (!(assign_data_cmd(tokens, data))) // SEGMENTATION FAULT IN THE FUNCTION (<<p <<l | << k => press 'p' enter, then ctrl + C, then call commande ls / pwd ...)
		return (false);
	printf("PRINTF BEFORE - PROCESSOR LINE 71\n");
	data->pipe_len = tokens->pipe_count + 1;
	printf("data->pipe_len %zu\n", data->pipe_len);
	if (open_heredoc(data) && g_exit_status == 1)
	{
		
		printf("PRINTF BEFORE - SEPARATOR\n");
		return (true);
	}
	printf("PRINTF AFTER - SEPARATOR\n");
	assign_fd(data, 0);
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
			printf("PRINTF BEFORE - EXIT_CTRL_D\n");
			exit_ctrl_d(&data);
		}
		if (line[0])
		{
			printf("PRINTF BEFORE - PROCESSOR\n");
			printf("LINE %s\n", line);
			if (processor(line, &data, &tokens) && data.cmd)
				free_cmd_struct(data.cmd, data.pipe_len);
		}
		if (line)
			free(line);
	}
	return (g_exit_status);
}
