/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_builtins.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jhurpy <jhurpy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 13:27:10 by jhurpy            #+#    #+#             */
/*   Updated: 2024/01/18 15:43:25 by jhurpy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	len_variable(char *var)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	return (i + 1);
}

bool	check_variable(char *var)
{
	int	i;

	i = 0;
	if (ft_isdigit(var[i]) || var[i] == '_')
		return (false);
	if (!ft_isalpha(var[i]))
		return (false);
	i++;
	while (var[i] != '=' && var[i] != '\0')
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

void	print_env(char **env, int flag)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (flag == 1)
			printf("declare -x ");
		printf("%s\n", env[i]);
		i++;
	}
}

void	add_variable(t_env *tmp_env, char *var)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (new == NULL)
		error_system(MALLOC_ERROR);
	new->name = ft_strdup(var);
	new->next = NULL;
	tmp_env->next = new;
}

bool	check_long_long(char *str)
{
	char	*tmp;

	if (str[0] == 45 && ft_strlen(str) <= 20)
	{
		if (ft_strlen(str) == 20 && str[1] == 57)
		{
			tmp = ft_substr(str, 2, 19);
			if (ft_atoll(tmp) > 223372036854775808)
				return (false);
		}
		return (true);
	}
	else if (ft_strlen(str) <= 19)
	{
		if (ft_strlen(str) == 19 && str[0] == 57)
		{
			tmp = ft_substr(str, 1, 18);
			if (ft_atoll(tmp) > 223372036854775807)
				return (false);
		}
		return (true);
	}
	return (false);
}
