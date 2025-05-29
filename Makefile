CCX = g++

CCFLAGS		=  -ansi -pedantic -std=c++11
#LDFLAGS		= -lrt
LDFLAGS	=

PROGS		= lab4
OBJS		= lab4.o Queue.o SharedMem.o
HDRS		= Queue.h SharedMem.h

.cc.o:
	@echo "Compiling " $<
	$(CCX) -c $(CCFLAGS) $<
	@echo $@ "done"

all:	$(PROGS)


lab4:	$(OBJS)
	@echo "Linking " $@
	$(CCX)  -o $@ $(OBJS) $(LDFLAGS)
	@echo $@ "done"

clean:
	$(RM) $(PROGS) $(OBJS) core *~




