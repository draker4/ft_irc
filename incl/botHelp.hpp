/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botHelp.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperriol <bperriol@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:14:17 by baptiste          #+#    #+#             */
/*   Updated: 2023/04/28 18:02:12 by bperriol         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOTHELP_HPP
# define BOTHELP_HPP

# include <iostream>

std::string	str_invite = " The INVITE command is used to invite a user to a channel. The parameter <nickname> is the nickname of the person to be invited to the target channel <channel>. Syntax : INVITE <nickname> <channel>";
std::string	str_join = " The JOIN command indicates that the client wants to join the given channel(s), each channel using the given key for it. Syntax: JOIN <channel>";
std::string	str_kick = " The KICK command can be used to request the forced removal of a user from a channel. It causes the <user> to be removed from the <channel> by force. Parameters: <channel> <user> *( \",\" <user> ) [<comment>]";
std::string	str_kill = " The KILL command is used to close the connection between a given client and the server they are connected to. KILL is a privileged command and is available only to IRC Operators. Syntax :KILL <nickname> <comment>";
std::string	str_list = " If the exact name of a channel is given, the only information about this channel is requested; otherwise, a list of all channels will be displayed. Syntax : LIST [<channel>]";
std::string	str_mode = " The MODE command is used to set or remove options (or modes) from a given target. Parameters: <target> [<modestring> [<mode arguments>...]]";
std::string	str_motd = " The MOTD command is used to get the “Message of the Day” of the given server. If <target> is not given, the MOTD of the server the client is connected to should be returned.";
std::string	str_names = " The NAMES command is used to view the nicknames joined to a channel. Syntax: NAMES <channel>{,<channel>}";
std::string	str_nick = " The NICK command is used to give the client a nickname or change the previous one. Syntax: NICK <nickname>";
std::string	str_notice = " The NOTICE command is used to send notices between users, as well as to send notices to channels. Parameters: <target> <text to be sent>";
std::string	str_oper = " The OPER command is used by a normal user to obtain IRC operator privileges. Both parameters are required for the command to be successful. Parameters: <name> <password>";
std::string	str_part = " The PART command removes the client from the given channel(s). Syntax: PART <channel>{,<channel>} [<reason>]";
std::string	str_pass = " The PASS command is used to set a 'connection password'. The password supplied must match the one defined in the server configuration. Syntax: PASS <password>";
std::string	str_ping = " The PING command is sent to check the other side of the connection is still connected, and/or to check for connection latency. Syntax: PING";
std::string	str_privmsg = " PRIVMSG is used to send private messages between users, as well as to send messages to channels.  <target> is usually the nickname of the recipient of the message, or a channel name. Parameters: <target> <text to be sent>";
std::string	str_quit = " The QUIT command is used to terminate a client's connection to the server. Parameters: [<reason>]";
std::string	str_topic = " The TOPIC command is used to change or view the topic of the given channel. Syntax : TOPIC <channel> [<topic>]";
std::string	str_user = " The USER command is used at the beginning of a connection to specify the username and realname of a new user. Syntax: USER <username> 0 * <realname>";
std::string	str_wallops = " The WALLOPS command is used to send a message to all currently connected users who have set the 'w' user mode for themselves. The <text> SHOULD be non-empty. Syntax: WALLOPS <text>";
std::string	str_who = " The WHO command is used to query a list of users who match the provided mask. Syntax : WHO <mask>";
std::string	str_whois = " The WHOIS command is used to query information about particular users. Syntax : WHOIS <nickname>";

#endif
