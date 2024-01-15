/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:34:29 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 09:13:12 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	ft_iscmd(int c)
{
	if (!(ft_isquote(c)) && !(is_operator(c)))
		return (true);
	return (false);
}

static int	len_cmd(char *line)
{
	int	i;

	i = 1;
	while (line[i] && ft_iscmd(line[i]))
		i++;
	return (i);
}

/*
The function check_line parse the input from the user to valid
if the input is correct.
Check for unclosed quotes & Variables e.g. $$ -$ 
*/

bool	check_line(char *line)
{
	int	i;

	i = 0;
	while (*line)
	{
		while (*line && ft_isspace(*line))
			line++;
		if (!(*line))
			break ;
		else if (ft_isquote(*line))
			i = lenquote(line);
		else if (is_operator(*line))
			i = len_operator(line);
		else if ((*line) == '$')
			i = lenvar(line);
		else if (ft_iscmd(*line))
			i = len_cmd(line);
		if (i == 0)
			return (false);
		else
			line += i;
	}
	return (true);
}
