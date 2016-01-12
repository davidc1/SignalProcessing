#
# This is an example GNUmakefile for my packages
#

# specific names for this package
DICT  = OpticalRecoTool_SignalProcessingDict
SHLIB = libOpticalRecoTool_SignalProcessing.so
SOURCES = $(filter-out $(DICT).cxx, $(wildcard *.cxx))
FMWK_HEADERS = LinkDef.h $(DICT).h
HEADERS = $(filter-out $(FMWK_HEADERS), $(wildcard *.h))
OBJECTS = $(SOURCES:.cxx=.o)

# include options for this package
INCFLAGS  = -I.                       #Include itself
INCFLAGS += $(shell larlite-config --includes)

# platform-specific options
OSNAME          = $(shell uname -s)
HOST            = $(shell uname -n)
OSNAMEMODE      = $(OSNAME)

# call kernel specific compiler setup
LDFLAGS += $(shell larlite-config --libs)
LDFLAGS += -L/usr/lib/x86_64-linux-gnu/ -lfftw3

# set compiler options for ROOT
#CXXFLAGS +=  `-lfftw3 -lm`
#CINTFLAGS +=  `-lfftw3 -lm`

include $(LARLITE_BASEDIR)/Makefile/Makefile.${OSNAME}

# call the common GNUmakefile
include $(LARLITE_BASEDIR)/Makefile/GNUmakefile.CORE
