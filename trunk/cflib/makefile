############################################
#                                          #
#  CFLIB - Flexible Configuration Library  #
#  (c) 1995-2012 Stefan Habermehl          #
#                                          #
#  Root makefile                           #
#                                          #
#  GNU Make Build Environment              #
#  based on Eclipse IDE Toolchains         #
#                                          #
#  @copyright (c) 2012 Stefan Habermehl    #
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

	
#### Archives

dist: archive/cflib-20.0.1.tgz archive/cflib-20.0.1-src.tgz

DISTFILES := makefile* *.txt *.doxyfile \
	include src lib liblin libmgw libmgw_debug \
	doc/*.html doc/cflib.* doc/include doc/man doc/html \
	doc/latex/Makefile doc/latex/*.sty doc/latex/*.ttf doc/latex/*.tex \
	doc/latex/*.dvi doc/latex/*.ps \
	examples/src examples/linux/cf_test examples/mswindows

DISTFILES_SRC := makefile* *.txt *.doxyfile \
	include src lib liblin libmgw libmgw_debug \
	doc/*.html doc/include doc/cflib.man doc/html \
	examples/makefile examples/*.mk examples/src examples/utilities

archive/cflib-20.0.2.tgz: $(DISTFILES)
	tar czvf $@ --exclude=\*.d --exclude=\*.o --exclude=\*.svn\* $(DISTFILES)

archive/cflib-20.0.2-src.tgz: $(DISTFILES_SRC)
	tar czvf $@ --exclude=\*.hh\* --exclude=\*.a --exclude=\*.exe --exclude=\*.d --exclude=\*.o --exclude=\*.svn\* $(DISTFILES_SRC)

#### Tasks common to Root Makefile and 
#### Build/Project Submakes in subdirectories (e.g. "lib", "examples")

-include makefile.targets

############################### end makefile
