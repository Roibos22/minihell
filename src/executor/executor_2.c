/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgrimmei <lgrimmei@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 05:47:24 by smargine          #+#    #+#             */
/*   Updated: 2024/02/07 00:22:18 by lgrimmei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * finds and executes the builtin, while also phandling fds
*/
int	ft_execute_builtin(t_statment *stm, t_data *data)
{
	int	exit;

	exit = 0;
	ft_handle_io_redirection(stm);
	if (stm->stm_has_cmd)
	{
		if (ft_strncmp(stm->cmd_args[0], "echo", 5) == 0)
			exit = ft_echo(stm);
		if (ft_strncmp(stm->cmd_args[0], "cd", 3) == 0)
			exit = ft_cd(stm, data);
		else if (ft_strncmp(stm->cmd_args[0], "pwd", 4) == 0)
			ft_pwd();
		else if (ft_strncmp(stm->cmd_args[0], "export", 7) == 0)
			exit = ft_export(stm, data);
		else if (ft_strncmp(stm->cmd_args[0], "unset", 6) == 0)
			exit = ft_unset(stm, data);
		else if (ft_strncmp(stm->cmd_args[0], "env", 4) == 0)
			ft_env(stm, data);
		else if (ft_strncmp(stm->cmd_args[0], "exit", 5) == 0)
			exit = ft_exit(stm, data);
	}
	dup2(data->orig_std_in, STDIN_FILENO);
	dup2(data->orig_std_out, STDOUT_FILENO);
	ft_exec_next_stm(stm->start_token, stm, data);
	return (exit);
}

/**
 * helper function, handles I/O for redirections
*/
void	ft_handle_io_redirection(t_statment *stm)
{
	if (stm->fd_in != STDIN_FILENO)
	{
		dup2(stm->fd_in, STDIN_FILENO);
		close(stm->fd_in);
	}
	if (stm->fd_out != STDOUT_FILENO)
	{
		dup2(stm->fd_out, STDOUT_FILENO);
		close(stm->fd_out);
	}
}

/**
 * prepares fds if a redirection was found
*/
void	ft_prepare_fds(t_statment *stm)
{
	if (stm->fd_in != STDIN_FILENO) 
	{
		dup2(stm->fd_in, STDIN_FILENO);
		close(stm->fd_in);
	}
	close(stm->pipe[0]);
	if (stm->fd_out != STDOUT_FILENO) 
	{
		dup2(stm->fd_out, STDOUT_FILENO);
		close(stm->fd_out);
	}
}
