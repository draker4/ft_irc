/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 10:02:29 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/06 16:11:15 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include "colors.hpp"

#define BOARD_WIDTH 10
#define	ORIGIN_X 5
#define	ORIGIN_Y 0
#define PIVOT_X 2
#define PIVOT_Y 1

class Piece
{
	private:
		
		// Constructors
		
		// Private functions
		
	public:
		// Constructors
		Piece( void );
		Piece( Piece const &src );
		
		// Destructors
		~Piece( void );
	
		// Assignment Operator
		Piece	&operator=( Piece const &src );
		
		// Getter
		
		// Setter

		// Public member functions
};

#endif
