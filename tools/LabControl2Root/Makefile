BINDIR        = ../bin

TARGET 	      = LabControl2Root

SOURCE        = LabControl2Root.cpp

ARCHITECTURE := $(shell uname)

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

CXX           = g++
CXXFLAGS      = -Wall -fPIC

LD            = g++
LDFLAGS       = -O2

ifeq ($(ARCHITECTURE),Darwin)
OSXVERSION    = $(shell sw_vers -productVersion | cut -d . -f 2)
OSXTARGET     = 10.$(OSXVERSION)
CXX           = MACOSX_DEPLOYMENT_TARGET=$(OSXTARGET) g++
LD            = MACOSX_DEPLOYMENT_TARGET=$(OSXTARGET) g++
LDFLAGS       = -O2
endif

CXXFLAGS     += $(ROOTCFLAGS)

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(SOURCE)
	@(test -e $(BINDIR) || mkdir $(BINDIR))
	@echo "Building binary $@"
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(TARGET) $(SOURCE) $(ROOTLIBS)

clean:
	rm -f $(BINDIR)/$(TARGET)
	rm -f *~
