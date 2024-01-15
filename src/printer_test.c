
// void print_test(char **tokens, int tc)
// {
// 	int i = 0;
// 	while(i < tc && *tokens != NULL)
// 	{
// 			printf("tokens[%d] = %s \n", i, tokens[i]);
// 			i++;
// 	}
// }
// void print_test_struct(t_data *data, t_tokens *tokens)
// {
// 	int i = 0;

// 	while (i < tokens->pipe_count + 1)
// 	{
// 		printf("cmd_struct[%d] \n", i);
// 		print_test(data->cmd[i].cmd, tokens->arg_count[i]);
// 		print_test(data->cmd[i].limiters, tokens->heredoc_count[i]);
// 		print_test(data->cmd[i].infiles, tokens->infile_count[i]);
// 		print_test(data->cmd[i].outfiles, tokens->outfile_count[i] + tokens->append_count[i]);
// 		printf("pipe_out = %d\n", data->cmd[i].pipe_out);
// 		printf("pipe_in = %d\n", data->cmd[i].pipe_in);
// 		printf("heredoc in = %d\n", data->cmd[i].here_doc_in);
// 		printf("heredoc fd = %d\n", data->cmd[i].here_doc_fd);
// 		printf("heredoc nb = %d\n", data->cmd[i].nb_heredocs);
// 		printf("file_in = %d\n", data->cmd[i].file_in);
// 		printf("file_out = %d\n", data->cmd[i].file_out);
// 		printf("append = %d\n", data->cmd[i].append);
// 		i++;
// 	}
// }