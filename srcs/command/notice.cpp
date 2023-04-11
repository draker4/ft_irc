/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:13:13 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/11 16:30:38 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

# include "command.hpp"

/**
 * @brief The NOTICE command is used to send notices between users, as well as to send notices to channels. 
 * <target> is interpreted the same way as it is for the PRIVMSG command.
 * 
 * The NOTICE message is used similarly to PRIVMSG. 
 * The difference between NOTICE and PRIVMSG is that automatic replies must never be sent in response to a NOTICE message. 
 * This rule also applies to servers – they must not send any error back to the client on receipt of a NOTICE command. 
   @param server
   @param client_fd User sending a msg
   @param cmd_infos Structure w/ prefix, command name and message
   Useful link : https://modern.ircdocs.horse/#notice-message
 * 
 */
void notice(Client *client, const Message &message, Server *server)
{
	std::cout << BLUE << "NOTICE command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}
