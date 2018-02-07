#-----------------------------------------------------------------
# makefile
#
# Tested on OSX with Xcode 8.3.2 (C++14).
#
# Copyright (☯) 2017 Glenn Streiff
#-----------------------------------------------------------------
# Headers picked up from:
# /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1
#-----------------------------------------------------------------

PROGRAMS  = parabola
PROGRAMS += parabola-bez
PROGRAMS += plane

CPPFLAGS = -std=c++14
#-Wno-c++11-extensions

all: $(PROGRAMS)

$(PROGRAMS): $(PROGRAMS:.cpp)

# Make forensics: print the implicit rules database.
debug:
	$(MAKE) -p -f/dev/null

CLEANME = $(PROGRAMS) $(PROGRAMS:=.o)
clean:
	@for delMe in $(CLEANME) ;\
	do \
		[ -f "$${delMe}" ] && rm $${delMe} || true; \
	done
