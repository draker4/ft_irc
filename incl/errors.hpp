/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:25:35 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/11 17:19:18 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

# define ERR_UNKNOWNCOMMAND(client, command) ( ":localhost 421 " + client + " " \
		+ command + ": Unknown command\r\n") // 421 - Server::_handleCommand
# define ERR_NONICKNAMEGIVEN ":localhost 431 : No nickname given\r\n" // 431 - NICK
# define ERR_ERRONEUSNICKNAME(nickname) ( ":localhost 432 " + nickname \
		+ ": Erroneus nickname\r\n") // 432 - NICK
# define ERR_NICKNAMEINUSE(nickname) ( ":localhost 433 " + nickname \
		+ ": Nickname is already in use\r\n") // 433 - NICK
# define ERR_NEEDMOREPARAMS(client, command) ( ":localhost 461 " + client + " " \
		+ command + ": Not Enough parameters\r\n") // 461 - PASS
# define ERR_ALREADYREGISTERED(client) ( ":localhost 462 " + client \
		+ ": You may not reregister\r\n") // 462 - PASS
# define ERR_PASSWDMISMATCH(client) ( ":localhost 464 " + client \
		+ ": Password incorrect\r\n") // 464 - PASS
# define ERROR_MESSAGE(reason) ( "ERROR: " + reason + "\r\n") // ERROR Message

#endif
