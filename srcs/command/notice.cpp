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

// The NOTICE command is used to send notices between users, as well as to send notices
// to channels. The difference between NOTICE and PRIVMSG is that automatic replies must
// never be sent in response to a NOTICE message.
void notice(Client *client, const Message &message, Server *server)
{
	std::cout << BLUE << "NOTICE command called" << RESET << std::endl;
	(void)client;
	(void)message;
	(void)server;
}
