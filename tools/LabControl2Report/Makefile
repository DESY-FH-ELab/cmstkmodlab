BINDIR        = ../bin

TARGET 	      = LabControl2Report

SOURCE        = LabControl2Report.cpp

ARCHITECTURE := $(shell uname)

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
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(TARGET) $(SOURCE)

clean:
	rm -f $(BINDIR)/$(TARGET)
	rm -f *~
