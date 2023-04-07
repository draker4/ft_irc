# ***********************  FT_IRC MAKEFILE  ************************ #

.PHONY			:	all clean fclean re bonus debug run runl runs rund

# **************************  VARIABLES  *************************** #

# --------  Final Executable  -------- #

NAME			=	ircServ
DEBUG			=	${NAME}_debug

# ----------  Directories  ----------- #

DIR_HEAD		=	./incl/
DIR_SRCS		=	./srcs/
DIR_OBJS		=	.objs/
DIR_OBJS_D		=	.objs_debug/

# -------------  Files  -------------- #

HEAD			=	colors.hpp

SRCS			=	ircServ.cpp Server.cpp

OBJS			=	${SRCS:%.cpp=${DIR_OBJS}%.o}
OBJS_D			=	${SRCS:%.cpp=${DIR_OBJS_D}%.o}
DEPS			=	${OBJS:.o=.d}
DEPS_D			=	${OBJS_D:.o=.d}

# ----------  Compilation  ----------- #

CC				=	c++
CFLAGS			= 	-Wall -Wextra -Werror
c98				=	-std=c++98
g3				=	-g3
FSANITIZE		=	-fsanitize=address
OPTI			=	-O3
MMD				=	-MMD

# ------------  Commands  ------------ #

RM				=	rm -rf
MKDIR			=	mkdir -p
LEAKS			=	leaks -atExit --
LLDB			=	lldb
CP				=	cp
VALGRIND		=	valgrind

# ****************************  RULES  ***************************** #

all					:	
						$(MAKE) -j ${NAME}

# ---------  Compiled Rules  --------- #

${NAME}				:	${OBJS}
						${CC} ${CFLAGS} ${c98} ${OBJS} -o ${NAME}

${DIR_OBJS}%.o		:	${DIR_SRCS}%.cpp Makefile | ${DIR_OBJS}
						${CC} ${CFLAGS} ${c98} ${MMD} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS}			:
						${MKDIR} ${DIR_OBJS}

-include ${DEPS}

# ------  Compiled Rules Debug  ------ #

${DEBUG}			:	${OBJS_D}
						${CC} ${CFLAGS} ${c98} ${OBJS_D} ${g3} ${FSANITIZE} -o ${DEBUG}

${DIR_OBJS_D}%.o	:	${DIR_SRCS}%.cpp Makefile | ${DIR_OBJS_D}
						${CC} ${CFLAGS} ${c98} ${MMD} ${g3} ${FSANITIZE} -I ${DIR_HEAD} -c $< -o $@

${DIR_OBJS_D}		:
						${MKDIR} ${DIR_OBJS_D}

-include ${DEPS_D}

# ---------  Usual Commands  --------  #

clean				:	
						${RM} ${DIR_OBJS}
						${RM} ${DIR_OBJS_D}
						${RM} ${DIR_DEBUG}

fclean				:	clean
						${RM} ${NAME}
						${RM} ${DEBUG}

re					:
						$(MAKE) fclean
						$(MAKE) all

# ---------  Other Commands  --------- #

debug				:	
						$(MAKE) -j ${DEBUG}

# -----------  Run Commands  --------- #

run					:	all
						./${NAME}

runl				:	all
						${LEAKS} ./${NAME} 

runs				:	debug
						./${DEBUG}

rund				:	debug
						${LLDB} ./${DEBUG}

runv				:	all
						${VALGRIND} ./${NAME}
