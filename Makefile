CXX				=	g++
CXXFLAGS		=	-O2 \
					-g \
					-Wall \
					-fmessage-length=0 \
					-Wno-unused-result

LDFLAGS			=	-lpthread

INC_DIR			=
LIB_DIR			=		

MNG_EXEC		=	cc_manager
MNG_SRCS_DIR	=	manager
MNG_HDRS		=	$(MNG_SRCS_DIR)/buffer.h \
					$(MNG_SRCS_DIR)/data.h \
					$(MNG_SRCS_DIR)/graph.h \
					$(MNG_SRCS_DIR)/locking.h \
					$(MNG_SRCS_DIR)/xaction_api.h
			
MNG_SRCS		=	$(MNG_SRCS_DIR)/binfiletest.cpp \
					$(MNG_SRCS_DIR)/buffer.cpp \
					$(MNG_SRCS_DIR)/data.cpp \
					$(MNG_SRCS_DIR)/graph.cpp \
					$(MNG_SRCS_DIR)/locking.cpp \
					$(MNG_SRCS_DIR)/server.cpp \
					$(MNG_SRCS_DIR)/trigger2.cpp \
					$(MNG_SRCS_DIR)/xaction_api.cpp
MNG_OBJS		=	$(MNG_SRCS:.cpp=.o)
#MNG_LIBS =		

CLT_EXEC		=	cc_client
CLT_SRCS_DIR	=	client
CLT_HDRS		=	$(CLT_SRCS_DIR)/xaction_api.h
			
CLT_SRCS		=	$(CLT_SRCS_DIR)/client_final.cpp \
					$(CLT_SRCS_DIR)/client.cpp \
					$(CLT_SRCS_DIR)/xaction_api.cpp
CLT_OBJS		=	$(CLT_SRCS:.cpp=.o)
#CLT_LIBS =

#.PHONY:		all manager client

all:		$(MNG_EXEC) \
			$(CLT_EXEC)

$(MNG_EXEC): $(MNG_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	
$(CLT_EXEC): $(CLT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(MNG_SRCS_DIR)/*.o cc_manager
	rm -f $(CLT_SRCS_DIR)/*.o cc_client