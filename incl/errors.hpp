/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 15:25:35 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/11 15:47:03 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

# define ERR_NEEDMOREPARAMS(client, command) ( ":localhost 461 " + client + " " \
		+ command + ": Not Enough parameters\r\n") // 461 - PASS
# define ERR_ALREADYREGISTERED(client) ( ":localhost 462 " + client \
		+ ": You may not reregister\r\n") // 462 - PASS
# define ERR_PASSWDMISMATCH(client) ( ":localhost 464 " + client \
		+ ": Password incorrect\r\n") // 464 - PASS
		
#endif
