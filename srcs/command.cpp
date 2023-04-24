/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 15:42:02 by bperriol          #+#    #+#             */
/*   Updated: 2023/04/24 17:45:58 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "command.hpp"

Message::vecString	split(std::string str, std::string c)
{
	Message::vecString	vec;
	size_t		prev = 0;
	size_t		pos;

	while ((pos = str.find_first_of(c, prev)) != std::string::npos) {
		vec.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}
	if (str[prev])
		vec.push_back(str.substr(prev, str.length() - prev));
	return vec;
}
