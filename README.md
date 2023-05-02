# ft_irc
The purpose of this 42 school project is to create an IRC server following the IRC protocol. No server to server communication is handled here.
This server was tested on ubuntu with nc and hexchat. The following commands are supported : INVITE, JOIN, KICK, KILL, LIST, MODE, MOTD, NAMES, NICK, NOTICE, OPER, PART, PASS, PING, PRIVMSG, QUIT, TOPIC, USER, WALLOPS, WHO, WHOIS.
Server operators and channel operators are implemented, and modes like banned, voiced, invited, moderated, ...
The client can connect with the PASS, USER and NICK command (as every IRC connection), connect to any channel if he's status allows it and do some actions as speaking privately, changing some of his modes, being invisible to others, kicking someone, banning someone, changing the topic, ...
Any client can ask the "help bot" for help about commands, and send messages to the "calculator bot" to ask for operations (a channel is given for this conversation too).
Everything is explained in the message of the day, the server is launched with the "make run" command and the bot with the "make runb" command.
