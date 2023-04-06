/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baptiste <baptiste@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 10:56:15 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/06 16:09:34 by baptiste         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* -----------------------------  Constructors  ----------------------------- */

Piece::Piece( void ) : _type ( 0 ), _rotation( 0 ), _color( 0 ),
	_posX ( ORIGIN_X ), _posY ( ORIGIN_Y )
{
	// std::cout << GREEN_T << "Piece Default Constructor called "
	// 	<< RESET << std::endl;
	return ;
}

Piece::Piece( int type ) : _type ( type ), _rotation( 0 ),
	_posX ( ORIGIN_X ), _posY ( ORIGIN_Y )
{
	// std::cout << GREEN_T << "Piece Special Constructor called "
	// 	<< RESET << std::endl;
	switch( type )
	{
		case I:
			this->_color = BLUE;
			break;
		case J:
			this->_color = YELLOW;
			break;
		case Z:
			this->_color = MAGENTA;
			break;
		case L:
			this->_color = GREEN;
			break;
		case O:
			this->_color = CYAN;
			break;
		case S:
		case T:
			this->_color = RED;
			break;
		default:
			break;
	}
	return ;
}

Piece::Piece( Piece const &src )
{
	// std::cout << GREEN_T << "Piece Copy Constructor called "
	// 	<< RESET << std::endl;
	*this = src;
	return ;
}

/* -----------------------------  Destructors  ------------------------------ */

Piece::~Piece( void )
{
	// std::cout << GREEN_T << "Piece Default Destructor called "
	// 	<< RESET << std::endl;
	return ;
}

/* -------------------------  Assignment Operator  -------------------------- */

Piece &Piece::operator=( Piece const &src )
{
	// std::cout << "Piece assignment operator called" << std::endl;
	if ( this != &src ) {
		this->_type = src._type;
		this->_rotation = src._rotation;
		this->_color = src._color;
		this->_posX = src._posX;
		this->_posY = src._posY;
	}
	return ( *this );
}

/* --------------------------------  Getter  -------------------------------- */

int	Piece::getType()
{
	return ( this->_type );
}

int	Piece::getRotation()
{
	return ( this->_rotation );
}

int	Piece::getColor()
{
	return ( this->_color );
}

int	Piece::getPosX()
{
	return ( this->_posX );
}

int	Piece::getPosY()
{
	return ( this->_posY );
}
		
/* --------------------------------  Setter  -------------------------------- */

void	Piece::setRotation( int r )
{
	this->_rotation = r;
	return ;
}

void	Piece::setColor( int c )
{
	this->_color = c;
	return ;
}

void	Piece::setPosX( int x )
{
	this->_posX = x;
	return ;
}

void	Piece::setPosY( int y )
{
	this->_posY = y;
	return ;
}

/* -----------------------  Public member functions  ------------------------ */
