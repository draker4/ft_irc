/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 15:31:32 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/13 18:11:14 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include "colors.hpp"
# include <map>

# define DEBUG_CHANNEL false

class Client;

class	Channel
{
	public:
		// Types
		typedef std::map<Client *, std::string>				mapClients;
		typedef std::map<Client *, std::string>::iterator	itMapClients;
		
		Channel(void);
		Channel(const Channel &src);
		~Channel(void);

		Channel	&operator=(const Channel &rhs);
		
		// getter
		mapClients			getClients(void) const;

	private:
		std::string			_key;
		mapClients			_clients;
		// const unsigned int	_client_limit;
};

#endif
