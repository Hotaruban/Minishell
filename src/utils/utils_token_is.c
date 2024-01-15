/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_is.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/15 08:55:53 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/15 09:54:44 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// bool	is_special(int c) 								//// WHAT IS THIS FUNCTION ?
// {
// 	if (c == '$' || c == '#' || c == '@' || c == '-'
// 		|| c == '!' || c == '*')
// 		return (true);
// 	return (false);
// }

bool	is_operator(int c)
{
	if (c == '<' || c == '>' || c == '|')
		return (true);
	return (false);
}

bool	is_token(int c)
{
	if (!(is_operator(c)) && !(ft_isspace(c)))
		return (true);
	return (false);
}
