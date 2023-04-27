/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 18:34:38 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/14 20:05:08 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The WHOIS command is used to query information about particular users. The server will
 * answer this command with several numeric messages with information about the nicks,
 * ending with RPL_ENDOFWHOIS.
 * Servers MUST end their reply to WHOIS messages with one of these numerics: 
 * 	ERR_NOSUCHNICK (401)
 * 	ERR_NONICKNAMEGIVEN (431)
 * 	RPL_ENDOFWHOIS (318)
 * 
 * The following numerics MAY be returned as part of the whois reply:
 * 	RPL_WHOISREGNICK (307)
 * 	RPL_WHOISUSER (311)
 * 	RPL_WHOISSERVER (312)
 * 	RPL_WHOISOPERATOR (313)
 * 	RPL_WHOISCHANNELS (319)
 * 	RPL_WHOISACCOUNT (330)
 * 	RPL_WHOISACTUALLY (338)
 * 	RPL_WHOISHOST (378)
 * 	RPL_WHOISMODES (379)
 * 
 * Servers typically send some of these numerics only to the client itself and to servers operators,
 * as they contain privacy-sensitive information that should not be revealed to other users.
 * 
 * Syntax : WHOIS <nickname>
 * 
 */
void whois(Client *client, const Message &message, Server *server)
{
	if (DEBUG_COMMAND)
		std::cout << BLUE << "INVITE command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}