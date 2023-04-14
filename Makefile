# ***********************  FT_IRC MAKEFILE  ************************ #

.PHONY			:	all clean fclean re bonus debug run runl runs rund
SHOWRULES		=	FALSE

# **************************  VARIABLES  *************************** #

# --------  Final Executable  -------- #

NAME			=	ircserv
DEBUG			=	${NAME}_debug

# ----------  Directories  ----------- #

DIR_HEAD		=	./incl/
DIR_SRCS		=	./srcs/
DIR_SRCS_C		=	./srcs/command/
DIR_OBJS		=	.objs/
DIR_OBJS_C		=	.objs/command/
DIR_OBJS_D		=	.objs_debug/
DIR_OBJS_D_C	=	.objs_debug/command/

# -------------  Files  -------------- #

HEAD			=	Client.hpp	\
					Channel.hpp \
					colors.hpp	\
					command.hpp	\
					errors.hpp	\
					Message.hpp	\
					Server.hpp

SRCS			=	Client.cpp	\
					Channel.cpp \
					ircserv.cpp	\
					Message.cpp	\
					Server.cpp

SRCS_C			=	cap.cpp		\
					invite.cpp	\
					join.cpp	\
					kick.cpp	\
					kill.cpp	\
					list.cpp	\
					mode.cpp	\
					motd.cpp	\
					names.cpp	\
					nick.cpp	\
					notice.cpp	\
					oper.cpp	\
					part.cpp	\
					pass.cpp	\
					ping.cpp	\
					privmsg.cpp	\
					quit.cpp	\
					topic.cpp	\
					user.cpp

OBJS			=	${SRCS:%.cpp=${DIR_OBJS}%.o}
OBJS_C			=	${SRCS_C:%.cpp=${DIR_OBJS_C}%.o}
OBJS_D			=	${SRCS:%.cpp=${DIR_OBJS_D}%.o}
OBJS_D_C		=	${SRCS_C:%.cpp=${DIR_OBJS_D_C}%.o}
DEPS			=	${OBJS:.o=.d}
DEPS_C			=	${OBJS_C:.o=.d}
DEPS_D			=	${OBJS_D:.o=.d}
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
LEAKS			=	leaks -atExit --
LLDB			=	lldb
CP				=	cp
VALGRIND		=	valgrind --leak-check=full

ifeq ($(SHOWRULES),TRUE)
	HIDE = 
else
	HIDE = @
endif

# ****************************  RULES  ***************************** #

all					:	
						${HIDE} ${CLEAR}
						${HIDE}$(MAKE) -j ${NAME}

# ---------  Compiled Rules  --------- #

${NAME}				:	${OBJS} ${OBJS_C}
						@echo "\033[0;33m"Building... "\033[0;0m"
						${HIDE} ${CC} ${CFLAGS} ${c98} ${OBJS} ${OBJS_C} -o ${NAME}
						@echo "\033[0;32m"Build Done!"\033[0;0m"

${DIR_OBJS}%.o		:	${DIR_SRCS}%.cpp Makefile | ${DIR_OBJS}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_C}%.o	:	${DIR_SRCS_C}%.cpp Makefile | ${DIR_OBJS}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS}			:
						${HIDE} ${MKDIR} ${DIR_OBJS}
						${HIDE} ${MKDIR} ${DIR_OBJS_C}

-include ${DEPS}
-include ${DEPS_C}

# ------  Compiled Rules Debug  ------ #

${DEBUG}			:	${OBJS_D} ${OBJS_D_C}
						@echo "\033[0;33m"Building Debug... "\033[0;0m"
						${HIDE} ${CC} ${CFLAGS} ${c98} ${OBJS_D} ${OBJS_D_C} ${g3} ${FSANITIZE} -o ${DEBUG}
						@echo "\033[0;32m"Debug build Done!"\033[0;0m"

${DIR_OBJS_D}%.o	:	${DIR_SRCS}%.cpp Makefile | ${DIR_OBJS_D}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} ${g3} ${FSANITIZE} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_D_C}%.o	:	${DIR_SRCS_C}%.cpp Makefile | ${DIR_OBJS_D}
						${HIDE} ${CC} ${CFLAGS} ${c98} ${MMD} ${g3} ${FSANITIZE} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_D}		:
						${HIDE} ${MKDIR} ${DIR_OBJS_D}
						${HIDE} ${MKDIR} ${DIR_OBJS_D_C}

-include ${DEPS_D}
-include ${DEPS_D_C}

# ---------  Usual Commands  --------  #

clean				:	
						${HIDE} ${RM} ${DIR_OBJS}
						${HIDE} ${RM} ${DIR_OBJS_D}
						${HIDE} ${RM} ${DIR_DEBUG}
						@echo "\033[0;34m"Cleaning done!"\033[0;0m"

fclean				:	clean
						${HIDE} ${RM} ${NAME}
						${HIDE} ${RM} ${DEBUG}
						@echo "\033[0;32m"Full cleaning done!"\033[0;0m"

re					:
						${HIDE} $(MAKE) fclean
						${HIDE} $(MAKE) all

# ---------  Other Commands  --------- #

debug				:	
						$(MAKE) -j ${DEBUG}

# -----------  Run Commands  --------- #

run					:	all
						./${NAME} 4242 6262

runl				:	all
						${LEAKS} ./${NAME} 

runs				:	debug
						./${DEBUG} 4242 6262

rund				:	debug
						${LLDB} ./${DEBUG}

runv				:	all
						${VALGRIND} ./${NAME} 4242 6262
