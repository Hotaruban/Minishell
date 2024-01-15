/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:39:09 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 13:31:32 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_status;

static void init_data(t_data *data, t_env *env)
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

static void init_tokens(t_tokens *tokens)
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
	if (!(expandinator(tokens, data)))
		return(false);
	if (!(quote_trim(tokens)))
		return (false);
	if (!(struct_fill(tokens, data)))
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
	// signal(SIGINT, &sigint_handler);
	// signal(SIGQUIT, &sigint_handler);
	while (1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		if (*line)
			add_history(line);
		if (!(processor(line, &data, &tokens)))	/* TO FIX OR REMOVE */
		{
			// free_data_struct(&data);
		}
		free(line);
	}
	rl_clear_history();
	restore_termios(&data.term);
	return (0);
}