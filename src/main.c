/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smargine <smargine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 14:16:14 by lgrimmei          #+#    #+#             */
/*   Updated: 2023/12/08 04:07:36 by smargine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

// /**
//  * Starts and initialises the program before entering a loop where each line 
//  * entered is handled
// */
// int	main(int ac, char **argv, char **env)
// {
// 	t_data	*data;

// 	data = malloc(sizeof(t_data));
// 	init_data(data, env);
// 	if (ac == 1 && argv)
// 	{
// 		while (1)
// 		{
// 			copy_env_paths(data);
// 			ft_signals(1, data);
// 			data->line = readline("minishell~$> ");
// 			if (!data->line)
// 				ft_signals(3, data);
// 			if (ft_lexer(data))
// 			{
// 				ft_prepare_data(data);
// 				ft_executor(data->tokens, STDIN_FILENO, data);
// 				ft_reset_data(data);
// 			}
// 			ft_free_str_arr(data->cmd_paths);
// 			data->cmd_paths = NULL;
// 			ft_clean_data(data, 0);
// 		}
// 		rl_clear_history();
// 	}
// 	else
// 		printf("./minishell doesn't accept arguments!\n");
// 	return (0);
// }

/**
 * Starts and initialises the program before entering a loop where each line 
 * entered is handled
*/
int	main(int ac, char **argv, char **env)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	init_data(data, env);
	if (ac == 1 && argv)
		ft_minihell(data);
	else
		printf("./minishell doesn't accept arguments!\n");
	return (0);
}

void	ft_minihell(t_data *data)
{
	while (1)
	{
		copy_env_paths(data);
		ft_signals(1, data);
		data->line = readline("minishell~$> ");
		if (!data->line)
			ft_signals(3, data);
		if (ft_lexer(data))
		{
			ft_prepare_data(data);
			ft_executor(data->tokens, STDIN_FILENO, data);
			ft_reset_data(data);
		}
		ft_free_str_arr(data->cmd_paths);
		data->cmd_paths = NULL;
		ft_clean_data(data, 0);
	}
	rl_clear_history();
}

/**
 * Prepares the data struct for each new line entered
*/
void	ft_prepare_data(t_data *data)
{
	data->exit_status = 0;
	data->stm_count = 0;
	data->created_files = malloc (sizeof(char *) * 1);
	data->created_files[0] = NULL;
}

/**
 * Handle last exit status, initiates freeing and adds line
 * to history
*/
void	ft_reset_data(t_data *data)
{
	if (g_received_signal)
	{
		data->exit_status = g_received_signal;
		g_received_signal = 0;
	}
	else
		data->exit_status = data->exit_status_final;
	ft_free_str_arr(data->created_files);
	ft_free_tokens(data->tokens);
	add_history(data->line);
	data->tokens = NULL;
	data->created_files = NULL;
}

/**
 * Initielises the data struct
*/
void	init_data(t_data *data, char **env)
{
	int	i;

	i = 0;
	data->orig_std_in = dup(STDIN_FILENO);
	data->orig_std_out = dup(STDOUT_FILENO);
	data->env = ft_copy_array(env);
	data->exit_status = 0;
	data->exit_status_final = 0;
	data->tokens = NULL;
	data->created_files = NULL;
	data->tok_is_red_str = NULL;
}
