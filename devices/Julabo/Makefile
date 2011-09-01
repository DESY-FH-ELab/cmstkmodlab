LIBDIR        = ../lib

LIB           = TkModLabJulabo

ARCHITECTURE := $(shell uname)
KERNELMAJOR  := $(shell uname -r | cut -d . -f 1)
KERNELMINOR  := $(shell uname -r | cut -d . -f 2)

MODULES       = FP50ComHandler \
		VJulaboFP50 \
                JulaboFP50Fake \
                JulaboFP50

CC            = gcc
CXX           = g++
CXXFLAGS      = -fPIC
LD            = g++
SOFLAGS       = -shared

ifeq ($(ARCHITECTURE),Darwin)
CXXFLAGS     += -DUSE_FAKEIO
endif
ifeq ($(ARCHITECTURE),Linux)
ifeq ($(shell test $(KERNELMINOR) -lt 6 && echo true),true)
CXXFLAGS     += -DUSE_FAKEIO
endif
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
