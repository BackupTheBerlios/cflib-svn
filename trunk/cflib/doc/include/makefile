############################################
#                                          #
#  CFLIB - Flexible Configuration Library  #
#  (c) 1995-2009 Stefan Habermehl          #
#                                          #
#  Make specific doxygen tagfiles          #
#  from library documentation tagfiles     #
#                                          #
#  @copyright (c) 2009 Stefan Habermehl    #
#  @license GNU GPL v3+ (see LICENSE.txt)  #
#                                          #
############################################
#
# @version SVN: $Id: $
#
############################################

XML := cflib_tagfile.xml
XSL := tagfile_filter.xsl
XSLT := Xalan

COMPOUNDS := page struct group file
MEMBERS   := function define variable typedef

all: $(foreach kind, $(COMPOUNDS), cflib_$(kind).xml) $(foreach kind, $(MEMBERS), libcf_$(kind).xml)

cflib_%.xml: %
	$(XSLT) -p compound "'$<'" $(XML) $(XSL) > $@

libcf_%.xml: %
	$(XSLT) -p compound "''" -p target "'$<'" $(XML) $(XSL) > $@

$(MEMBERS) $(COMPOUNDS):: % : cflib_tagfile.xml
	@echo "Specific Tagfile for:" $@

.PHONY: $(MEMBERS) $(COMPOUNDS)

############################### end makefile