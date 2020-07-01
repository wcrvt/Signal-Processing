##########################
## COMPILE/LINK OPTIONS ##
##########################

CC := gcc	# compiler
LD := gcc	# linker

CCFLAGS := -Wall -W -Weffc++ -O3		# warnings, optimization
LDFLAGS := -Wall -W -Weffc++ -O3	# warnings, optimization

CCLIBTAR :=				# target library:
LDLIBTAR := -lm -lstdc++ -lpthread -L/usr/local/lib -lfftw3# target library: libm.so,stdc++

CCOPTIONS := $(CCFLAGS) $(CDLIBTAR)
LDOPTIONS := $(LDFLAGS) $(LDLIBTAR)

###################
## TARGET FILES  ##
###################

TARGET := hilbert
SRCS := $(shell ls  *c)
OBJS := $(SRCS:%.c=%.o)

.SUFFIXES:
.SUFFIXES: .c .h .o

####################
##  TASK TARGETS  ##
####################

def: $(TARGET)

# build application
$(TARGET): $(OBJS) $(LIBS)
	@$(LD) $(LDOPTIONS) -o $@ $^

# generate object
.cc.o:
	@$(CC) $(CCOPTIONS) -c $< -o $@

# clean targets
.PHONY: clean
clean: 
	@rm -f $(OBJS) $(TARGET)

