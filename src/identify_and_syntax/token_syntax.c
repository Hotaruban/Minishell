/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 20:05:12 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 00:00:44 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*Make a error function*/

bool	order_error_check(t_tk_type type, t_tk_type pre)
{
	if ((pre == e_pipe && type == e_pipe) || (pre == e_rdrt && type == e_rdrt)
		|| (pre == e_rdrt && type == e_pipe))
		return (false);
	return (true);
}

bool	token_syntax(t_tokens *tokens)
{
	int	i;
	int	j;

	i = 0;
	j = tokens->token_count;
	if ((tokens->token_type[i] == e_pipe)
		|| (tokens->token_type[j - 1] == e_pipe)
		|| tokens->token_type[j - 1] == e_rdrt)
		return (false);
	while (i < j)
	{
		if (i == 0)
		{
			if (!(order_error_check(tokens->token_type[i], e_void)))
				return (false);
		}
		else
		{
			if (!(order_error_check(tokens->token_type[i], tokens->token_type[i - 1])))
				return (false);
		}
		i++;
	}
	return (true);
}
