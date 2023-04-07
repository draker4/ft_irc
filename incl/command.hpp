/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:14:17 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/07 15:55:31 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>

void invite (void);
void join (void);
void kick (void);
void kill (void);
void list (void);
void mode (void);
void motd (void);
void names (void);
void nick (void);
void notice (void);
void oper (void);
void part (void);
void pass (void);
void ping (void);
void privmsg (void);
void quit (void);
void topic (void);
void user (void);

#endif