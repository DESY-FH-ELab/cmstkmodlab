LIBDIR        = ../lib

LIB           = TkModLabConrad

USEFAKEDEVICES=1
ARCHITECTURE=Darwin

MODULES       = ConradCommunication VConradController ConradController ConradControllerFake

CC            = gcc
CXX           = g++
CXXFLAGS      = -fPIC
LD            = g++
SOFLAGS       = -m64 -O2 -dynamiclib -single_module -undefined dynamic_lookup

ifeq ($(USEFAKEDEVICES),1)
CXXFLAGS     += -DUSE_FAKEIO
endif

ifeq ($(ARCHITECTURE),Darwin)
CXXFLAGS     += -I/opt/local/include
endif

ALLDEPEND = $(addsuffix .d,$(MODULES))

EXISTDEPEND = $(shell find . -name \*.d -type f -print)

all: depend lib

depend: $(ALLDEPEND)

lib: $(LIBDIR)/lib$(LIB).so

$(LIBDIR)/lib$(LIB).so: $(addsuffix .o,$(MODULES))
	@(test -e $(LIBDIR) || mkdir $(LIBDIR))
	@echo "Linking shared library $@"
	$(LD) $(SOFLAGS) $^ -o $@

%.d: %.cpp
	@echo Making dependency for file $< ...
	@set -e;\
	$(CXX) -M $(CPPFLAGS) $(CXXFLAGS)  $< |\
	sed 's!$*\.o!& $@!' >$@;\
	[ -s $@ ] || rm -f $@

%.o: %.cpp
	@echo "Compiling $<"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(LIBDIR)/lib$(LIB).so
	@rm -f $(addsuffix .o,$(MODULES))
	@rm -f *.d
	@rm -f *~

ifeq ($(findstring clean,$(MAKECMDGOALS)),)
ifneq ($(EXISTDEPEND),)
-include $(EXISTDEPEND)
endif
endif
