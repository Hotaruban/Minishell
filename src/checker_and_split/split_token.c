/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 18:14:00 by whendrik          #+#    #+#             */
/*   Updated: 2024/01/15 10:08:26 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	len_token(char *line)
{
	int		i;

	i = 0;
	if (is_token(line[i]))
	{
		while ((is_token(line[i]) && line[i]))
		{
			if (ft_isquote(line[i]))
				i += len_quote(&line[i]);
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
	int	nb_tokens;
	int	i;

	nb_tokens = 0;
	i = 0;
	if (!line)
		return (nb_tokens);
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (!(line[i]))
			return (nb_tokens);
		if (is_token(line[i]))
			i += len_token(&line[i]);
		else if (is_operator(line[i]))
			i += len_operator(&line[i]);
		nb_tokens++;
	}
	return (nb_tokens);
}

static char	**token_split(char *line, int nb_tokens)
{
	int		i;
	int		j;
	int		k;
	char	**tokens;

	i = 0;
	j = 0;
	k = 0;
	tokens = (char **)malloc(sizeof(char *) * (nb_tokens + 1));
	while (j < nb_tokens)
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
	tokens[nb_tokens] = NULL;
	return (tokens);
}

int	split_token(char *line, t_tokens *stuff)
{
	char	**tokens;
	int		nb_tokens;

	nb_tokens = token_count(line);
	tokens = NULL;
	if (nb_tokens == 0)
		return (false);
	tokens = token_split(line, nb_tokens);
	stuff->token_count = nb_tokens;
	stuff->tokens = tokens;
	return (true);
}
