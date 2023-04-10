/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:14:17 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/10 16:15:23 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <iostream>
# include "Server.hpp"
# include <sstream>

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
void pass (Server *server, std::string line, int clientSocket);
void ping (void);
void privmsg (void);
void quit (void);
void topic (void);
void user (void);

#endif
