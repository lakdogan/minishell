/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 05:53:04 by almatsch          #+#    #+#             */
/*   Updated: 2025/08/24 16:28:30 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/core/minishell.h"
#include "../../../includes/parse/parse_types.h" // Make sure this header defines t_command, t_command_tree, N_CMD, etc.

// Forward declaration
t_command_tree *parse_command(t_tokens *tokens, int *pos, t_gc *gc);

// Modified: parse_command function to handle assignments
// t_command_tree *parse_command(t_tokens *tokens, int *pos, t_gc *gc)
// {
//     t_command_tree *node;
//     t_command *cmd;
//     int start = *pos, count = 0;

//     // Zähle aufeinanderfolgende Assignment-Tokens
//     while (*pos < tokens->count && tokens->arr[*pos].type == ASSIGNMENT)
//     {
//         count++;
//         (*pos)++;
//     }

//     if (count > 0)
//     {
//         // Erstelle ein Kommando mit nur Umgebungsvariablen (wie Bash)
//         cmd = gc_calloc(gc, 1, sizeof(t_command));
//         if (!cmd)
//             return (NULL);

//         cmd->env_vars = gc_calloc(gc, count + 1, sizeof(char *));
//         if (!cmd->env_vars)
//             return (NULL);

//         for (int i = 0; i < count; i++)
//             cmd->env_vars[i] = gc_strdup(gc, tokens->arr[start + i].value);
//         cmd->env_vars[count] = NULL;
//         cmd->argv = NULL;  // Keine Kommando-Argumente

//         node = gc_calloc(gc, 1, sizeof(t_command_tree));
//         if (!node)
//             return (NULL);
//         node->type = NODE_CMD;
//         node->cmd = cmd;
//         // Gib das Node immer zurück, auch wenn danach eine Pipe kommt!
//         return (node);
//     }

//     // Normale Kommando-Parsing-Logik
//     cmd = gc_calloc(gc, 1, sizeof(t_command));
//     if (!cmd)
//         return (NULL);

//     // Check for environment variable assignments at beginning.
//     int cmd_pos = *pos;
//     int has_assignments = 0;
//     while (cmd_pos < tokens->count && tokens->arr[cmd_pos].type == ASSIGNMENT)
//     {
//         has_assignments = 1;
//         cmd_pos++;
//     }

//     // If assignments found (not the only tokens), store them in the command.
//     if (has_assignments)
//     {
//         cmd->env_vars = gc_calloc(gc, cmd_pos - *pos + 1, sizeof(char *));
//         if (!cmd->env_vars)
//             return (NULL);
//         int env_idx = 0;
//         while (*pos < cmd_pos)
//         {
//             cmd->env_vars[env_idx++] = gc_strdup(gc, tokens->arr[*pos].value);
//             (*pos)++;
//         }
//         cmd->env_vars[env_idx] = NULL;
//     }

//     // Hier folgt dein bestehender Code für Argumente und Node-Erstellung...
//     node = gc_calloc(gc, 1, sizeof(t_command_tree));
//     if (!node)
//         return (NULL);
//     node->type = NODE_CMD;
//     node->cmd = cmd;
//     return (node);
// }

t_command_tree *parse_command(t_tokens *tokens, int *pos, t_gc *gc)
{
    t_command_tree *node;
    t_command *cmd;
    int start = *pos, count = 0;

    // Zähle aufeinanderfolgende Assignment-Tokens
    while (*pos < tokens->count && tokens->arr[*pos].type == ASSIGNMENT)
    {
        count++;
        (*pos)++;
    }

    // Nur Assignments → pending assignment node
    if (count > 0 && (*pos >= tokens->count || tokens->arr[*pos].type != WORD))
    {
        cmd = gc_calloc(gc, 1, sizeof(t_command));
        if (!cmd)
            return (NULL);
        cmd->env_vars = gc_calloc(gc, count + 1, sizeof(char *));
        if (!cmd->env_vars)
            return (NULL);
        for (int i = 0; i < count; i++)
            cmd->env_vars[i] = gc_strdup(gc, tokens->arr[start + i].value);
        cmd->env_vars[count] = NULL;
        cmd->argv = NULL;
        node = gc_calloc(gc, 1, sizeof(t_command_tree));
        if (!node)
            return (NULL);
        node->type = NODE_CMD;
        node->cmd = cmd;
        return (node);
    }

    // Assignments + Kommando: alles als Argumente ans Kommando
    *pos = start;
    cmd = gc_calloc(gc, 1, sizeof(t_command));
    if (!cmd)
        return (NULL);
    cmd->env_vars = NULL;

    // Argumente einsammeln (get_args akzeptiert sowohl ASSIGNMENT als auch WORD)
    t_arg_lst *args = NULL;
    args = get_args((t_exec*)cmd, tokens, pos, gc); // ggf. cast anpassen
    if (!args)
        return (NULL);
    cmd->argv = lst_to_argv(args, gc);

    node = gc_calloc(gc, 1, sizeof(t_command_tree));
    if (!node)
        return (NULL);
    node->type = NODE_CMD;
    node->cmd = cmd;
    return (node);
}

t_command_tree	*create_new_node(t_node_type type, t_gc *gc)
{
	t_command_tree	*node;

	node = gc_calloc(gc, 1, sizeof(t_command_tree));
	if (node)
		node->type = type;
	return (node);
}

int	handle_redir(t_exec	*exec, t_tokens *tokens, int *pos, t_gc *gc)
{
	t_token_type	type;
	int				if_fail;

	type = tokens->arr[(*pos)].type;
	(*pos)++;
	if ((*pos) >= tokens->count || tokens->arr[(*pos)].type != WORD)
	{
		return (0);
	}
	if_fail = 0;
	if (type == APPEND || type == OUTFILE)
		if_fail = add_outfile(exec, type, tokens->arr[(*pos)].value, gc);
	else if (type == HEREDOC || type == INFILE)
		if_fail = add_infile(exec, type, tokens->arr[(*pos)].value, gc);
	if (if_fail == 0)
		return (0);
	(*pos)++;
	return (1);
}

t_arg_lst	*get_args(t_exec *exec, t_tokens *tokens, int *pos, t_gc *gc)
{
    t_arg_lst	*args;

    args = NULL;
    while ((*pos) < tokens->count && (
        tokens->arr[(*pos)].type == WORD ||
        tokens->arr[(*pos)].type == ASSIGNMENT ||
        is_redir(tokens->arr[(*pos)].type)))
    {
        if (is_redir(tokens->arr[(*pos)].type))
        {
            if (!handle_redir(exec, tokens, pos, gc))
            {
                (*pos) = -1;
                return (NULL);
            }
        }
        else
        {
            arg_to_list(&args, tokens->arr[(*pos)].value,
                tokens->arr[(*pos)].no_expand, gc);
            (*pos)++;
        }
    }
    return (args);
}

// Make sure parse_cmd also handles assignments if they slip through
t_command_tree	*parse_cmd(t_tokens *tokens, int *pos, t_gc *gc)
{
	t_exec		*exec;
	t_arg_lst	*args;
	int			s_pos;

	// If we encounter assignment tokens, handle them
	if ((*pos) < tokens->count && tokens->arr[*pos].type == ASSIGNMENT)
		return (parse_command(tokens, pos, gc));

	exec = gc_calloc(gc, 1, sizeof(t_exec));
	if (!exec)
		return (NULL);
	s_pos = (*pos);
	args = get_args(exec, tokens, pos, gc);
	if ((*pos) == -1)
		return (NULL);
	if (s_pos == (*pos))
		return (NULL);
	return (create_exec_node(exec, args, gc));
}
