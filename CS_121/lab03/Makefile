#  Makefile
#    a makefile for the stack example.
#

#            SHELL = 	/bin/sh

#              TOP = .
#      CURRENT_DIR = .

#              CPP = /lib/cpp $(STD_CPP_DEFINES)
              CXX  = g++

#        CCOPTIONS =
#           CFLAGS = $(CDEBUGFLAGS) $(CCOPTIONS) $(ALLDEFINES)
#           RM_CMD = $(RM) *.o core

SRCS=	testQueue.cpp	\
	queueL.cpp	\
	listQ.cpp

OBJS=	testQueue.o	\
	queueL.o	\
	listQ.o

testQueue.o:	queueL.h

queueL.o: 	queueL.h

listQ.o:	listQ.h

PROGRAMS = testQueue

all::  $(PROGRAMS)


testQueue: $(OBJS)
	$(CXX) -o $@ $(OBJS)

#$(CXX) -o $@ $(OBJS) $(LDOPTIONS) $(LOCAL_LIBRARIES) $(LDLIBS)  $(EXTRA_LOAD_FLAGS)

clean::
	$(RM) testQueue

###########################################################################
# common rules for all Makefiles - do not edit

emptyrule::

clean::
	rm *.o

