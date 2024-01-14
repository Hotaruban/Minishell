/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/25 13:39:09 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/14 21:44:36 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_status;

void print_test(char **tokens, int tc)
{
	int i = 0;
	while(i < tc && *tokens != NULL)
	{
			printf("tokens[%d] = %s \n", i, tokens[i]);
			i++;
	}
}
void print_test_struct(t_data *data, t_tokens *tokens)
{
	int i = 0;

	while (i < tokens->pipe_count + 1)
	{
		printf("cmd_struct[%d] \n", i);
		print_test(data->cmd[i].cmd, tokens->arg_count[i]);
		print_test(data->cmd[i].limiters, tokens->heredoc_count[i]);
		print_test(data->cmd[i].infiles, tokens->infile_count[i]);
		print_test(data->cmd[i].outfiles, tokens->outfile_count[i] + tokens->append_count[i]);
		printf("pipe_out = %d\n", data->cmd[i].pipe_out);
		printf("pipe_in = %d\n", data->cmd[i].pipe_in);
		printf("heredoc in = %d\n", data->cmd[i].here_doc_in);
		printf("heredoc fd = %d\n", data->cmd[i].here_doc_fd);
		printf("heredoc nb = %d\n", data->cmd[i].nb_heredocs);
		printf("file_in = %d\n", data->cmd[i].file_in);
		printf("file_out = %d\n", data->cmd[i].file_out);
		printf("append = %d\n", data->cmd[i].append);
		i++;
	}
}

void init_data(t_data *data, t_env *env)
{
	data->cmd = NULL;
	
	data->env = env;
	data->status = 0;										/*Uncertain can come back to this later*/
	data->pipe_len = 0;
	data->pipefd[1] = -1; 									/*Uncertain as well*/
	if (!(set_termios(&data->term)))
		printf("FAIL INIT DATA"); 							/*TOO REMOVE*/
	set_signal();
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

bool	processor(char *line, t_data *data, t_tokens *tokens)
{
	add_history(line);
	if (!(checker(line)))
		return (false);
	if(!(split_token(line, tokens)))
		return (false);
	if (!(token_identify(tokens)))
		return (false);
	if (!(token_syntax(tokens)))
		return(false);
	if (!(expandinator(tokens, data)))
		return(false);
	if (!(quote_trim(tokens)))
		return (false);
	if (!(struct_fill(tokens, data)))
		return (false);
	data->pipe_len = tokens->pipe_count + 1;
	print_test_struct(data, tokens);
	if (tokens != NULL)
		free_tokens(tokens);
	if ((separator_op(data) != CMD_OK))
		return (false);
	if (data->cmd)
		free_cmd_struct(data->cmd);
	return (true);
}

int main(int ac, char** argv, char **ev)
{
	t_data		data;
	char		*line;
	t_tokens	tokens;
	t_env		*env;
	
	(void)argv;
	if (ac != 1)
		exit(1);
	env = set_env(ev);
	init_data(&data, env);
	init_tokens(&tokens);
	// signal(SIGINT, &sigint_handler);
	// signal(SIGQUIT, &sigint_handler);
	while (1)
	{
		line = readline("minishell : ");
		if (line == NULL)
			break;
		if (*line && !(processor(line, &data, &tokens)))	/* TO FIX OR REMOVE */
		{
			// break;
			// free_data_struct(&data);
		}
		free(line);
	}
	rl_clear_history();
	restore_termios(&data.term);
	return (0);
}