#-----------------------------------------------------------------
# makefile
#
# Tested on OSX 10.12
#
# Build Environment
# -----------------
# /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain
#    Xcode 8.3.2
#    clang 8.1.0
#
# /System/Library/Frameworks/OpenGL.framework
#    OpenGL 2.1 (bundle version 14.0.16)
#
# /System/Library/Frameworks/GLUT.framework
#    GLUT 3.6.9
# 
# Copyright (☯) 2017 Glenn Streiff
#-----------------------------------------------------------------
PROGRAMS  = hello-cube
PROGRAMS += hello-cone
PROGRAMS += hello-split
PROGRAMS += hello-text
PROGRAMS += hello-reshape
PROGRAMS += hello-split-ar
PROGRAMS += hello-bezier
PROGRAMS += hello-gpubez
PROGRAMS += hello-gpubez-1
PROGRAMS += hello-parabola
PROGRAMS += hello-parabolas
PROGRAMS += hello-transparency
PROGRAMS += hello-transparency-1
PROGRAMS += hello-transparency-2
INCDIR    = ..
SHARED    = $(INCDIR)/shared

#-----------------------------------------------------------------
# This program has build-time and run-time dependencies upon
# OpenGL and GLUT frameworks.
#
# NB: -framework references SDKs under: /System/Library/Frameworks
#-----------------------------------------------------------------
macosfw_CFLAGS  = -framework CoreServices 
macosfw_CFLAGS += -framework OpenGL
macosfw_CFLAGS += -framework GLUT

# For *.c programs.

CFLAGS    = $(macosfw_CFLAGS)
CFLAGS   += -Wno-deprecated-declarations
CFLAGS   += -I$(INCDIR)

# For *.cpp programs.

CXXFLAGS  = $(CFLAGS)

#-----------------------------------------------------------------
# Build targets, dependencies, and recipes.
#-----------------------------------------------------------------
all: $(PROGRAMS)

$(PROGRAMS): $(PROGRAMS:.c) $(PROGRAMS:.cpp)

#-----------------------------------------------------------------
# Build the supporting cast of dependencies that many of the
# demo programs require.
#-----------------------------------------------------------------
math3d.o: $(SHARED)/math3d.cpp
	$(COMPILE.cpp) -Wno-unused-command-line-argument $(OUTPUT_OPTION) $<

gltools.o: $(SHARED)/gltools.cpp
	$(COMPILE.cpp) -Wno-tautological-pointer-compare -Wno-unused-value -Wno-unused-command-line-argument $(OUTPUT_OPTION) $<

GLee.o: $(SHARED)/GLee.c
	$(CC) $(CPPFLAGS) $(TARGET_ARCH) -c $(OUTPUT_OPTION) $<

#-----------------------------------------------------------------
# Specify link-time dependencies for certain targets.
#-----------------------------------------------------------------
$(PROGRAMS): gltools.o math3d.o GLee.o


# Make forensics: print the implicit rules database.
debug:
	$(MAKE) -p -f/dev/null

CLEANME = $(PROGRAMS) $(PROGRAMS:=.o) math3d.o gltools.o GLee.o
clean:
	@for delMe in $(CLEANME) ;\
	do \
		[ -f "$${delMe}" ] && rm $${delMe} || true; \
	done
	@for delMe in $(CLEANME_DIRS) ;\
	do \
		[ -d "$${delMe}" ] && rm -rf $${delMe} || true; \
	done
