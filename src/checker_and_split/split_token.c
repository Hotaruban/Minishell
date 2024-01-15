/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 18:14:00 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 09:13:29 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	len_token(char *line)
{
	int		i;

	i = 0;
	if (ft_istoken(line[i]))
	{
		while ((ft_istoken(line[i]) && line[i]))
		{
			if (ft_isquote(line[i]))
				i += lenquote(&line[i]);
			else
				i++;
		}
	}
	else if (is_operator(line[i]))
		i += len_operator(&line[i]);
	return (i);
}

static int	token_count(char *line)
{
	int	tc;
	int	i;

	tc = 0;
	i = 0;
	if (!line)
		return (tc);
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (!(line[i]))
			return (tc);
		if (ft_istoken(line[i]))
			i += len_token(&line[i]);
		else if (is_operator(line[i]))
			i += len_operator(&line[i]);
		tc++;
	}
	return (tc);
}

static char	**token_split(char *line, int tc)
{
	int		i;
	int		j;
	int		k;
	char	**tokens;

	i = 0;
	j = 0;
	k = 0;
	tokens = (char **)malloc(sizeof(char *) * (tc + 1));
	while (j < tc)
	{
		while (ft_isspace(line[i]))
			i++;
		k = len_token(&line[i]);
		tokens[j] = (char *)malloc(sizeof(char) * (k + 1));
		ft_memcpy(tokens[j], &line[i], k);
		tokens[j][k] = '\0';
		i += k;
		j++;
	}
	tokens[tc] = NULL;
	return (tokens);
}

int	split_token(char *line, t_tokens *stuff)
{
	int		tc;
	char	**tokens;

	tc = token_count(line);
	tokens = NULL;
	if (tc == 0)
		return (false);
	tokens = token_split(line, tc);
	stuff->token_count = tc;
	stuff->tokens = tokens;
	return (true);
}
