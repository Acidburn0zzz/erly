# Copyright (c) 2017 Adriaan de Groot <groot@kde.org>
#
# This Makefile is just a stub: it invokes CMake, which in turn
# generates Makefiles, and then uses those to make the project. 
#
# Useful Make parameters at this level are:
#	PREFIX=/usr/local
#	PULLEY_BACKEND_DIR=/usr/local/share/steamworks/pulleyback/
#
# For anything else, do this:
#
#	make configure                 # Basic configure
#	( cd build ; ccmake )          # CMake GUI for build configuration
#	( cd build ; make install )    # Build and install
#
#
PREFIX ?= /usr/local
PULLEY_BACKEND_DIR ?= $(PREFIX)/share/steamworks/pulleyback/

# Set CMAKE_ARGS to influence the configuration. Here
# are some examples, with the default settings assigned.
# Take care that *_DIR variables must have a trailing / .
#
CMAKE_ARGS = -DCMAKE_INSTALL_PREFIX:PATH=$(PREFIX)
CMAKE_ARGS += -DPULLEY_BACKEND_DIR=$(PULLEY_BACKEND_DIR)

all: compile

build-dir:
	@mkdir -p build

configure: _configure build-dir build/CMakeCache.txt

_configure:
	@rm -f build/CMakeCache.txt

build/CMakeCache.txt:
	( cd build && cmake .. $(CMAKE_ARGS) )

compile: build-dir build/CMakeCache.txt
	( cd build && $(MAKE) )
	
install: build-dir
	( cd build && $(MAKE) install )
	
test: build-dir
	( cd build && $(MAKE) test )
	
uninstall: build-dir
	( cd build && $(MAKE) uninstall )

clean:
	rm -rf build/

package: compile
	( cd build && $(MAKE) package )

