############################################
#                                          #
#  CFLIB - Flexible Configuration Library  #
#  (c) 1995-2008 Stefan Habermehl          #
#                                          #
#  Root makefile                           #
#                                          #
#  GNU Make Build Environment              #
#  based on Eclipse IDE Toolchains         #
#                                          #
#  @copyright (c) 2008 Stefan Habermehl    #
#  @license GNU GPL v3+ (see LICENSE.txt)  #
#                                          #
############################################
#
# @version SVN: $Id$
#
############################################

#### Setup

-include makefile.init

CFLIB_ROOT_REL := .

#### Project Root Makefile General Tasks

all: info_system info_usage

package: $(CFLIB_BUILDS)

#### Cleanup

clean: $(CFLIB_BUILDS:%=%_clean)
	@echo "Finished Cleaning Configurations: $(CFLIB_BUILDS)"
	@echo ' '

$(CFLIB_BUILDS:%=%_clean): %:
	@echo "Cleaning Configuration '$(@:%_clean=%)'"
	cd $(@:%_clean=%) && make clean
	-$(RM) $(@:%_clean=%)$(DIRSEP).update
	@echo "Finished cleaning configuration '$(@:%_clean=%)'"
	@echo ' '

#### Examples

$(CFLIB_EXAMPLES_EXEC_FILES): $(CFLIB_EXAMPLES_LIB_FILE)
$(CFLIB_EXAMPLES_EXEC_FILES): %$(EXE): $(CFLIB_EXAMPLES_DIR)/src/%.o
	@echo 'Building example target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -static -L"$(CFLIB_EXAMPLES_LIB_PATH)" -o"$@" $< -l$(CFLIB_EXAMPLES_LIB)
	@echo 'Finished building target: $@'
	@echo ' '

$(foreach target, $(CFLIB_EXAMPLES), examples/src/examples/$(target).o): examples/%.o: %.c
	make -C "$(CFLIB_EXAMPLES_PATH)" $<

#### Tasks common to Root Makefile and 
#### Build/Project Submakes in subdirectories (e.g. "lib", "examples")

-include makefile.targets

############################### end makefile