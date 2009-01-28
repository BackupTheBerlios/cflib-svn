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

-include makefile.init

#-include makefile.defs
CFLIB_ROOT_REL := .

all: info_system

package: $(CFLIB_BUILDS)

clean: $(CFLIB_BUILDS:%=%_clean)
	@echo "Finished Cleaning Configurations: $(CFLIB_BUILDS)"
	@echo ' '

$(CFLIB_BUILDS:%=%_clean): %:
	@echo "Cleaning Configuration '$(@:%_clean=%)'"
	cd $(@:%_clean=%) && make clean
	-rm -rf $(@:%_clean=%)$(DIRSEP).update
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

#$(filter-out $(CFLIB_EXAMPLES_DIR)$(DIRSEP)dbm%,$(CFLIB_EXAMPLES_EXEC_FILES)): %$(EXE): $(CFLIB_EXAMPLES_DIR)/src/%.o
#$(filter $(CFLIB_EXAMPLES_DIR)$(DIRSEP)dbm%,$(CFLIB_EXAMPLES_EXEC_FILES)): $(CFLIB_EXAMPLES_DBMLIB_FILE)
#$(filter $(CFLIB_EXAMPLES_DIR)$(DIRSEP)dbm%,$(CFLIB_EXAMPLES_EXEC_FILES)): %$(EXE): $(CFLIB_EXAMPLES_DIR)/src/%.o
#	@echo 'Building dbm example target: $@'
#	@echo 'Invoking: GCC C Linker'
#	gcc -static -L"$(CFLIB_EXAMPLES_DBMLIB_PATH)" -o"$@" $< -l$(CFLIB_EXAMPLES_DBMLIB)
#	@echo 'Finished building target: $@'
#	@echo ' '

-include makefile.targets

############################### end makefile