BINDIR        = ../bin

TARGET 	      = ReadGMH3750

SOURCE        = ReadGMH3750

ARCHITECTURE := $(shell uname)

CXX           = g++
CXXFLAGS      = -Wall -fPIC -I../../devices/Greisinger/

LIBS          = ../../devices/Greisinger/*.o

LD            = g++
LDFLAGS       = -O2

ifeq ($(ARCHITECTURE),Darwin)
OSXVERSION    = $(shell sw_vers -productVersion | cut -d . -f 2)
OSXTARGET     = 10.$(OSXVERSION)
CXX           = MACOSX_DEPLOYMENT_TARGET=$(OSXTARGET) g++
LD            = MACOSX_DEPLOYMENT_TARGET=$(OSXTARGET) g++
LDFLAGS       = -O2
endif

#CXXFLAGS     += -DUSE_FAKEIO

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(addsuffix .o,$(SOURCE)) 
	@(test -e $(BINDIR) || mkdir $(BINDIR))
	@echo "Building binary $@"
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(TARGET) $< $(LIBS)

%.o: %.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BINDIR)/$(TARGET)
	rm -f $(addsuffix .o,$(SOURCE))
	rm -f *~
