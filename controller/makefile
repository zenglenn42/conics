#-----------------------------------------------------------------
# makefile
#
# This is a friendly make-frontend wrapper to a cmake build.  
# Obviously showing my bias for make.
#
# Tested on OSX 10.13
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
# 3rd party headers are generally under:
#    /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk/System/Library/Frameworks
#
# ToDo
# -----------------
# Update build so changes to PROGRAMS drives updates to 
# CMakeLists.txt
#
# Copyright (☯) 2018 Glenn Streiff
#-----------------------------------------------------------------
.SUFFIXES:

PROGRAMS  = $(BUILD_DIR)/hello-spinner
PROGRAMS += $(BUILD_DIR)/hello-checkbox
PROGRAMS += $(BUILD_DIR)/hello-motion
PROGRAMS += $(BUILD_DIR)/hello-ui
PROGRAMS += $(BUILD_DIR)/hello-ui-1
PROGRAMS += $(BUILD_DIR)/hello-conics
PROGRAMS += $(BUILD_DIR)/hello-conics-1
PROGRAMS += $(BUILD_DIR)/hello-conics-2
PROGRAMS += $(BUILD_DIR)/hello-conics-3
PROGRAMS += $(BUILD_DIR)/hello-conics-4
PROGRAMS += $(BUILD_DIR)/hello-conics-5
PROGRAMS += $(BUILD_DIR)/hello-conics-6
PROGRAMS += $(BUILD_DIR)/hello-conics-7

BUILD_DIR = build
GENERATED_MAKEFILE = $(BUILD_DIR)/Makefile
CMAKEFILE = CMakeLists.txt

debug_FLAGS = -print-search-dirs

#-----------------------------------------------------------------
# Tools isolation.
#-----------------------------------------------------------------
CMAKE = cmake

#-----------------------------------------------------------------
# Build targets, dependencies, and recipes.
#-----------------------------------------------------------------
all: $(GENERATED_MAKEFILE) $(PROGRAMS)

debug:
	c++ $(debugFLAGS) hello-spinner.cpp

$(PROGRAMS): $(BUILD_DIR)/% : %.cpp | $(GENERATED_MAKEFILE)
	@echo building $@ from $?
	cd $(BUILD_DIR) && make $(?:.cpp=)

PSEUDO_PROGRAMS = $(patsubst $(BUILD_DIR)/%, %, $(PROGRAMS))
$(PSEUDO_PROGRAMS):
	$(MAKE) $(BUILD_DIR)/$@
	@if [ -f "$(BUILD_DIR)/$@" ]; then \
		echo Built target $(BUILD_DIR)/$@ ;\
		echo ;\
	fi

$(GENERATED_MAKEFILE): $(CMAKEFILE) | $(BUILD_DIR)
	@echo building $@ ...
	@if which $(CMAKE); then \
		cd $(BUILD_DIR) && $(CMAKE) ..; \
	else \
		echo $(CMAKE) not found in search path; \
                false; \
	fi

$(BUILD_DIR):
	mkdir -p $@

#CLEANME = $(PROGRAMS)
clean:
	@if [ -f "$(GENERATED_MAKEFILE)" ]; then \
		cd $(BUILD_DIR) && make $@; \
	fi

CLEANME_DIRS = $(BUILD_DIR)
distclean:
	@for delMe in $(CLEANME) ;\
	do \
		[ -f "$${delMe}" ] && rm $${delMe} || true; \
	done
	@for delMe in $(CLEANME_DIRS) ;\
	do \
		[ -d "$${delMe}" ] && rm -rf $${delMe} || true; \
	done
