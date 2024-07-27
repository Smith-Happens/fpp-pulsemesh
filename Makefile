SRCDIR ?= /opt/fpp/src
include $(SRCDIR)/makefiles/common/setup.mk
include $(SRCDIR)/makefiles/platform/*.mk

all: libfpp-pulsemesh.$(SHLIB_EXT)
debug: all


OBJECTS_fpp_PulseMesh_so += src/FPPPulseMesh.o
LIBS_fpp_PulseMesh_so += -L$(SRCDIR) -lfpp -ljsoncpp -lcurl
CXXFLAGS_src/FPPPulseMesh.o += -I$(SRCDIR)


%.o: %.cpp Makefile
	$(CCACHE) $(CC) $(CFLAGS) $(CXXFLAGS) $(CXXFLAGS_$@) -c $< -o $@

libfpp-PulseMesh.$(SHLIB_EXT): $(OBJECTS_fpp_PulseMesh_so) $(SRCDIR)/libfpp.$(SHLIB_EXT)
	$(CCACHE) $(CC) -shared $(CFLAGS_$@) $(OBJECTS_fpp_PulseMesh_so) $(LIBS_fpp_PulseMesh_so) $(LDFLAGS) -o $@

clean:
	rm -f libfpp-pulsemesh.$(SHLIB_EXT) $(OBJECTS_fpp_PulseMesh_so)
