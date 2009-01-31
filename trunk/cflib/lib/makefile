############################################
#                                          #
#  CFLIB - Flexible Configuration Library  #
#  (c) 1994-2009 Stefan Habermehl          #
#                                          #
#  Default Library Makefile                #
#                                          #
#  @copyright (c) 2009 Stefan Habermehl    #
#  @license GNU GPL v3+ (see LICENSE.txt)  #
#                                          #
############################################
#
# @version SVN: $Id$
#
############################################

-include ../makefile.init
-include ../makefile.defs

LIBRARY_TARGET := $(CFLIB_PREF)$(CFLIB)$(CFLIB_EXT)
SOURCE_RELPATH := ..$(DIRSEP)src$(DIRSEP)
SOURCE_MODULES := cfmain cfio cfutil cfread cfwrite cferr cfdebug findfile

OBJS := $(foreach module, $(SOURCE_MODULES), $(module).o)

# Library Build Targets

all: $(LIBRARY_TARGET)

$(LIBRARY_TARGET): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking Archiver'
	ar -r $@ $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

%.o: $(SOURCE_RELPATH)%.c
	@echo 'Building file: $<'
	@echo 'Invoking C Compiler'
	$(CC) $(CFLIB_DEFINES) $(CFLIB_UNDEFINES) -O2 -Wall -Werror -c "$<" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets

clean:
	-$(RM) $(OBJS) $(LIBRARY_TARGET)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

############################### end makefile