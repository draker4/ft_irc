# ***********************  FT_IRC MAKEFILE  ************************ #

.PHONY			:	all bonus clean fclean re reb bonus debug debug_bonus \
					run runs rund runv runb runbs runbd runbv
SHOWRULES		=	FALSE

# **************************  VARIABLES  *************************** #

# --------  Final Executable  -------- #

NAME			=	ircserv
BOTCALC			=	botCalc
DEBUG			=	${NAME}_debug
DEBUG_BOTCALC	=	${BOTCALC}_debug

# ----------  Directories  ----------- #

DIR_HEAD		=	./incl/
DIR_SRCS		=	./srcs/
DIR_SRCS_BC		=	./srcs/bot/
DIR_SRCS_BH		=	./srcs/bot/
DIR_SRCS_C		=	./srcs/command/
DIR_OBJS		=	.objs/
DIR_OBJS_BC		=	.objs/botCalc/
DIR_OBJS_BH		=	.objs/botHelp/
DIR_OBJS_C		=	.objs/command/
DIR_OBJS_D		=	.objs_debug/
DIR_OBJS_D_BC	=	.objs_debug/botCalc/
DIR_OBJS_D_BH	=	.objs_debug/botHelp/
DIR_OBJS_D_C	=	.objs_debug/command/

# -------------  Files  -------------- #
					#Calculator.hpp

HEAD			=	ABot.hpp		\
					botHelp.hpp		\
					Calc.hpp		\
					Channel.hpp 	\
					Client.hpp		\
					colors.hpp		\
					command.hpp		\
					errors.hpp		\
					Message.hpp		\
					replies.hpp		\
					Server.hpp

SRCS			=	Channel.cpp		\
					Client.cpp		\
					command.cpp 	\
					ircserv.cpp		\
					Message.cpp		\
					Server.cpp

SRCS_C			=	invite.cpp		\
					join.cpp		\
					kick.cpp		\
					kill.cpp		\
					list.cpp		\
					mode.cpp		\
					motd.cpp		\
					names.cpp		\
					nick.cpp		\
					notice.cpp		\
					oper.cpp		\
					part.cpp		\
					pass.cpp		\
					ping.cpp		\
					privmsg.cpp		\
					quit.cpp		\
					topic.cpp		\
					user.cpp		\
					wallops.cpp		\
					who.cpp			\
					whois.cpp

SRCS_BC			=	ABot.cpp		\
					botCalc.cpp		\
					Calc.cpp		\
					../Message.cpp
					# Calculator.cpp

SRCS_BH			=	botHelp.cpp

OBJS			=	${SRCS:%.cpp=${DIR_OBJS}%.o}
OBJS_BC			=	${SRCS_BC:%.cpp=${DIR_OBJS_BC}%.o}
OBJS_BH			=	${SRCS_BH:%.cpp=${DIR_OBJS_BH}%.o}
OBJS_C			=	${SRCS_C:%.cpp=${DIR_OBJS_C}%.o}
OBJS_D			=	${SRCS:%.cpp=${DIR_OBJS_D}%.o}
OBJS_D_BC		=	${SRCS_BC:%.cpp=${DIR_OBJS_D_BC}%.o}
OBJS_D_BH		=	${SRCS_BH:%.cpp=${DIR_OBJS_D_BH}%.o}
OBJS_D_C		=	${SRCS_C:%.cpp=${DIR_OBJS_D_C}%.o}
DEPS			=	${OBJS:.o=.d}
DEPS_BC			=	${OBJS_BC:.o=.d}
DEPS_BH			=	${OBJS_BH:.o=.d}
DEPS_C			=	${OBJS_C:.o=.d}
DEPS_D			=	${OBJS_D:.o=.d}
DEPS_D_BC		=	${OBJS_D_BC:.o=.d}
DEPS_D_BH		=	${OBJS_D_BH:.o=.d}
DEPS_D_C		=	${OBJS_D_C:.o=.d}

# ----------  Compilation  ----------- #

CC				=	c++
CFLAGS			= 	-Wall -Wextra -Werror
c98				=	-std=c++98
g3				=	-g3
FSANITIZE		=	-fsanitize=address
OPTI			=	-O3
MMD				=	-MMD
CLEAR			=	clear

# ------------  Commands  ------------ #

RM				=	rm -rf
MKDIR			=	mkdir -p
LLDB			=	lldb
CP				=	cp
VALGRIND		=	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all

ifeq ($(SHOWRULES),TRUE)
	HIDE = 
else
	HIDE = @
endif

# ****************************  RULES  ***************************** #

all					:	
						${HIDE} ${CLEAR}
						${HIDE}$(MAKE) -j ${NAME}

bonus					:	
						${HIDE} ${CLEAR}
						${HIDE}$(MAKE) -j ${BOTCALC}

# ---------  Compiled Rules  --------- #

${NAME}				:	${OBJS} ${OBJS_BH} ${OBJS_C}
						@echo "\033[0;33m"Building ircserv... "\033[0;0m"
						${HIDE} ${CC} ${CFLAGS} ${c98} ${OBJS} ${OBJS_BH} ${OBJS_C} -o ${NAME}
						@echo "\033[0;32m"ircserv Build Done!"\033[0;0m"

${DIR_OBJS}%.o		:	${DIR_SRCS}%.cpp Makefile | ${DIR_OBJS}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_BH}%.o	:	${DIR_SRCS_BH}%.cpp Makefile | ${DIR_OBJS}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_C}%.o	:	${DIR_SRCS_C}%.cpp Makefile | ${DIR_OBJS}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS}			:
						${HIDE} ${MKDIR} ${DIR_OBJS}
						${HIDE} ${MKDIR} ${DIR_OBJS_BH}
						${HIDE} ${MKDIR} ${DIR_OBJS_C}

-include ${DEPS}
-include ${DEPS_BH}
-include ${DEPS_C}

# ------  Compiled Rules Debug  ------ #

${DEBUG}			:	${OBJS_D} ${OBJS_D_BH} ${OBJS_D_C}
						@echo "\033[0;33m"Building ircserv_debug... "\033[0;0m"
						${HIDE} ${CC} ${CFLAGS} ${c98} ${OBJS_D} ${OBJS_D_BH} ${OBJS_D_C} ${g3} ${FSANITIZE} -o ${DEBUG}
						@echo "\033[0;32m"ircserv_debug build Done!"\033[0;0m"

${DIR_OBJS_D}%.o	:	${DIR_SRCS}%.cpp Makefile | ${DIR_OBJS_D}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} ${g3} ${FSANITIZE} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_D_BH}%.o	:	${DIR_SRCS_BH}%.cpp Makefile | ${DIR_OBJS_D}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} ${g3} ${FSANITIZE} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_D_C}%.o	:	${DIR_SRCS_C}%.cpp Makefile | ${DIR_OBJS_D}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} ${g3} ${FSANITIZE} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_D}		:
						${HIDE} ${MKDIR} ${DIR_OBJS_D}
						${HIDE} ${MKDIR} ${DIR_OBJS_D_BH}
						${HIDE} ${MKDIR} ${DIR_OBJS_D_C}

-include ${DEPS_D}
-include ${DEPS_D_BH}
-include ${DEPS_D_C}

# -----  Compiled Rules botCalc  ----- #

${BOTCALC}			:	${OBJS_BC}
						@echo "\033[0;33m"Building botCalc... "\033[0;0m"
						${HIDE} ${CC} ${CFLAGS} ${c98} ${OBJS_BC} -o ${BOTCALC}
						@echo "\033[0;32m"Build botCalc Done!"\033[0;0m"

${DIR_OBJS_BC}%.o	:	${DIR_SRCS_BC}%.cpp Makefile | ${DIR_OBJS_BC}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_BC}			:
						${HIDE} ${MKDIR} ${DIR_OBJS_BC}

-include ${DEPS_BC}

# --  Compiled Rules Debug botCalc  -- #

${DEBUG_BOTCALC}	:	${OBJS_D_BC} 
						@echo "\033[0;33m"Building botCalc_debug... "\033[0;0m"
						${HIDE} ${CC} ${CFLAGS} ${c98} ${OBJS_D_BC} ${g3} ${FSANITIZE} -o ${DEBUG}
						@echo "\033[0;32m"botCalc_debug build Done!"\033[0;0m"

${DIR_OBJS_D_BC}%.o	:	${DIR_SRCS}%.cpp Makefile | ${DIR_OBJS_D_BC}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} ${g3} ${FSANITIZE} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_D_BC}		:
						${HIDE} ${MKDIR} ${DIR_OBJS_D_BC}

-include ${DEPS_D_BC}

# ---------  Usual Commands  --------  #

clean				:	
						${HIDE} ${RM} ${DIR_OBJS}
						${HIDE} ${RM} ${DIR_OBJS_D}
						${HIDE} ${RM} ${DIR_DEBUG}
						@echo "\033[0;34m"Cleaning done!"\033[0;0m"

fclean				:	clean
						${HIDE} ${RM} ${NAME}
						${HIDE} ${RM} ${DEBUG}
						${HIDE} ${RM} ${BOTCALC}
						${HIDE} ${RM} ${BOTCALC_DEBUG}
						@echo "\033[0;32m"Full cleaning done!"\033[0;0m"

re					:
						${HIDE} $(MAKE) clean
						${HIDE} ${RM} ${NAME}
						${HIDE} $(MAKE) all

reb					:
						${HIDE} $(MAKE) clean
						${HIDE} ${RM} ${BOTCALC}
						${HIDE} $(MAKE) bonus

# ---------  Other Commands  --------- #

debug				:	
						$(MAKE) -j ${DEBUG}

debug_bonus				:	
						$(MAKE) -j ${DEBUG_BOTCALC}

# -----------  Run Commands  --------- #

run					:	all
						./${NAME} 4245 6262

runs				:	debug
						./${DEBUG} 4245 6262

rund				:	debug
						${LLDB} ./${DEBUG}

runv				:	all
						${VALGRIND} ./${NAME} 4245 6262

runb				:	bonus
						./${BOTCALC} localhost 4245 6262

runbs				:	debug_bonus
						./${BOTCALC_DEBUG} 4245 6262

runbd				:	debug
						${LLDB} ./${DEBUG} 4245 6262

runbv				:	bonus
						${VALGRIND} ./${BOTCALC} 4245 6262
