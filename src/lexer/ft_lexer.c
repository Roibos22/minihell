/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrimmei <lgrimmei@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 14:09:38 by lgrimmei          #+#    #+#             */
/*   Updated: 2023/12/08 14:03:24 by lgrimmei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * creates an array of tokens out of the entered line
*/
int	ft_lexer(t_data *data)
{
	t_lexer_help	*lh;

	if (!ft_no_open_quotes(data->line))
		return (ft_perror(OP_QUO, NULL, 1, data), 0);
	lh = ft_init_lexer_help(data);
	data->array = ft_lexer_split(lh, data);
	ft_tokenize(data);
	ft_free_lexer(lh);
	if (!data->no_tokens)
		return (0);
	return (1);
}

/**
 * Initializes the lexer_help struct
 * @param *line pointer to the line to be lexed
*/
t_lexer_help	*ft_init_lexer_help(t_data *data)
{
	t_lexer_help	*lexer_help;

	lexer_help = (t_lexer_help *)malloc(sizeof(t_lexer_help));
	lexer_help->i = 0;
	lexer_help->in_quotes = 0;
	lexer_help->quotes_type = 0;
	lexer_help->size = 0;
	lexer_help->start = 0;
	lexer_help->curr_t = 0;
	lexer_help->line = ft_strdup(data->line);
	lexer_help->result = NULL;
	lexer_help->temp = NULL;
	lexer_help->envp = malloc(sizeof(char **));
	*lexer_help->envp = data->env;
	return (lexer_help);
}

/**
 * Splits the line that was given to the function into an array of strings that 
 * can be converted to tokens afterward
 * @param line the line that was given to the program
*/
char	**ft_lexer_split(t_lexer_help *lh, t_data *data)
{
	ft_get_no_of_tokens(lh);
	lh->i = 0;
	lh->start = 0;
	ft_fill_tokens_array(lh, data);
	data->no_tokens = lh->size;
	ft_cleanup_tokens(lh);
	return (lh->result);
}

/**
 * Cleans up tokens from not used characters like quotes
 * @param *lh pointer to lexer_help struct
 */
void	ft_cleanup_tokens(t_lexer_help *lh)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (lh->result[j])
		j++;
	while (lh->result[i])
	{
		remove_quotes(lh->result[i], 0, 0);
		i++;
	}
}

/**
 * Removes inner double quotes within closed quotes from each token
 * @param *str pointer to current string
 */
void	remove_quotes(char *str, int i, int j)
{
	bool	in_quotes;
	char	quote_char;

	in_quotes = false;
	quote_char = '\0';
	while (str[i]) 
	{
		if ((str[i] == '\'' || str[i] == '\"') && !in_quotes) 
		{
			in_quotes = true;
			quote_char = str[i];
			i++;
		}
		else if (in_quotes && str[i] == quote_char) 
		{
			in_quotes = false;
			quote_char = '\0';
			i++;
		}
		else 
			str[j++] = str[i++];
	}
	str[j] = '\0';
}
