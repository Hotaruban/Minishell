/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 20:05:12 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 13:53:00 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	order_error_check(t_tk_type type, t_tk_type prev)
{
	if ((prev == e_pipe && type == e_pipe) || (prev == e_rdrt
		&& type == e_rdrt) || (prev == e_rdrt && type == e_pipe))
		return (false);
	return (true);
}

static bool	check_type_token(t_tokens *tokens, int nb_token)
{
	int	last_char;

	last_char = 0;
	if (tokens->token_type[0] == e_pipe)
	{
		error_input(SYNTAX_ERROR, tokens->tokens[0]);
		return (false);
	}
	else if ((tokens->token_type[nb_token] == e_pipe) 
			|| (tokens->token_type[nb_token] == e_rdrt))
	{
		last_char = ft_strlen(tokens->tokens[nb_token]) - 1;
		error_input(SYNTAX_ERROR, &tokens->tokens[nb_token][last_char]);
		return (false);
	}
	return (true);
}

bool	token_syntax(t_tokens *tokens)
{
	int	i;
	int	nb_token;

	i = 0;
	nb_token = tokens->token_count - 1;
	if (check_type_token(tokens, nb_token) == false)
		return (false);
	while (i <= nb_token)
	{
		if (i == 0)
		{
			if (!(order_error_check(tokens->token_type[i], e_void)))
				return (false);
		}
		else
		{
			if (!(order_error_check(tokens->token_type[i]
					, tokens->token_type[i - 1])))
				return (false);
		}
		i++;
	}
	return (true);
}
