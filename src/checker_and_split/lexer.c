/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/06 15:34:29 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/14 23:54:17 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_iscmd(int c)
{
	if (!(ft_isquote(c)) && !(ft_isoptr(c)))
		return (true);
	return (false);
}

bool	is_special(int c)
{
	if (c == '$' || c == '#' || c == '@' || c == '-'
		|| c == '!' || c == '*')
		return (true);
	return (false);
}

int	lenoptr(char *line)
{
	if (*line + 1 && (line[1] == '&' || line[1] == '|'
			|| line[1] == '<' || line[1] == '>'))
		return (2);
	return (1);
}

int	lencmd(char *line)
{
	int	i;

	i = 1;
	while (line[i] && ft_iscmd(line[i]))
		i++;
	return (i);
}

/*Check for unclosed quotes & Variables e.g. $$ -$ */
bool	checker(char *line)
{
	int	i;

	i = 0;
	if (!(*line))
		return (false);
	while (*line)
	{
		while (*line && ft_isspace(*line))
			line++;
		if (!(*line))
			break ;
		else if (ft_isquote(*line))
			i = lenquote(line);
		else if (ft_isoptr(*line))
			i = lenoptr(line);
		else if ((*line) == '$')
			i = lenvar(line);
		else if (ft_iscmd(*line))
			i = lencmd(line);
		if (i == 0)
			return (false);
		else
			line += i;
	}
	return (true);
}
