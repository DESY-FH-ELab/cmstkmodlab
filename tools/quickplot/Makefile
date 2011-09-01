BINDIR        = ../bin

TARGETS	      = quickgraph quickhist quickplot

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

all: $(addprefix $(BINDIR)/,$(TARGETS))

$(BINDIR)/quickgraph: quickgraph.C
	@(test -e $(BINDIR) || mkdir $(BINDIR))
	@echo "Building binary $@"
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/quickgraph quickgraph.C $(ROOTLIBS)

$(BINDIR)/quickhist: quickhist.C
	@(test -e $(BINDIR) || mkdir $(BINDIR))
	@echo "Building binary $@"
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/quickhist quickhist.C $(ROOTLIBS)

$(BINDIR)/quickplot: quickplot.C
	@(test -e $(BINDIR) || mkdir $(BINDIR))
	@echo "Building binary $@"
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/quickplot quickplot.C $(ROOTLIBS)

clean:
	rm -f $(addprefix $(BINDIR)/,$(TARGETS))
	rm -f *~
