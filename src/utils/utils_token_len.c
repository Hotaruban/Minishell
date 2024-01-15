/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_len.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 09:46:46 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/15 09:52:59 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	len_var(char *line)
{
	int	len_var;

	len_var = 1;
	if (!ft_isalpha(line[len_var]) && !(line[len_var] == '_')
        && !(line[len_var] == '?'))
		return (0);
	len_var++;
	while (line[len_var] && (ft_isalnum(line[len_var]) || line[len_var] == '_'))
		len_var++;
	return (len_var);
}

int	len_operator(char *line)
{
	if (*line + 1 && (line[1] == '&' || line[1] == '|'
			|| line[1] == '<' || line[1] == '>'))
		return (2);
	return (1);
}

int	len_quote(char *line)
{
	char	quote;																/*WHAT MEAN THIS VARIABLE ?*/
	int		i;

	quote = line[0];
	i = 1;
	while (line[i] != '\0')
	{
		if (line[i] == quote)
			return (i + 1);
		i++;
	}
	if (quote == '\"')
		printf("TOO REMOVE -- lenquote -- \n");								/* TOO REMOVE */
																			// quote_error(2);
	if (quote == '\'')
		printf("TOO REMOVE -- lenquote -- \n");								/* TOO REMOVE */
																			// quote_error(1);
	return (0);
}
