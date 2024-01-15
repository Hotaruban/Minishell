/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 12:06:30 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 09:13:55 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_istoken(int c)
{
	if (!(is_operator(c)) && !(ft_isspace(c)))
		return (true);
	return (false);
}

int	lenquote(char *line)
{
	int		i;
	char	c;																/*WHAT MEAN THIS VARIABLE ?*/

	c = line[0];
	i = 1;
	while (line[i] != '\0')
	{
		if (line[i] == c)
			return (i + 1);
		i++;
	}
	if (c == '\"')
		printf("TOO REMOVE -- lenquote -- \n");								/* TOO REMOVE */
																			// quote_error(2);
	if (c == '\'')
		printf("TOO REMOVE -- lenquote -- \n");								/* TOO REMOVE */
																			// quote_error(1);
	return (0);
}

int	lenvar(char *line)
{
	int	i;

	i = 1;
	if (!ft_isalpha(line[i]) && !(line[i] == '_') && !(line[i] == '?'))
		return (0);
	i++;
	while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
		i++;
	return (i);
}
