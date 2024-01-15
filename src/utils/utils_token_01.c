/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_01.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:55:53 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/15 09:12:52 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_special(int c)
{
	if (c == '$' || c == '#' || c == '@' || c == '-'
		|| c == '!' || c == '*')
		return (true);
	return (false);
}

int	len_operator(char *line)
{
	if (*line + 1 && (line[1] == '&' || line[1] == '|'
			|| line[1] == '<' || line[1] == '>'))
		return (2);
	return (1);
}


bool	is_operator(int c)
{
	if (c == '<' || c == '>' || c == '|')
		return (true);
	return (false);
}
