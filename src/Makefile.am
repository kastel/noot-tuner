# these are the headers for your project
noinst_HEADERS = audio_file.h audio_portaudio.h audioio.h wxfbtunerframe.h \
	noot-tuner.h notedetection.h options.h tunerframe.h

EXTRA_DIST = tunerframe.fpb

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = noot-tuner

# the application source, library search path, and link libraries
if HAVE_WINDRES
noot_tuner_SOURCES = audio_file.cpp audio_portaudio.cpp audioio.cpp \
	wxfbtunerframe.cpp noot-tuner.cpp deviceselector.cpp notedetection.cpp options.cpp res.rc \
	tunerframe.cpp
else
noot_tuner_SOURCES = audio_file.cpp audio_portaudio.cpp audioio.cpp \
	wxfbtunerframe.cpp noot-tuner.cpp deviceselector.cpp notedetection.cpp options.cpp \
	tunerframe.cpp
endif
noot_tuner_LDFLAGS = $(WX_LIBS) -lfftw3 -lportaudio

.rc.o:
	$(WINDRES) $(CPPFLAGS) -I.. -o $@ $<

Applicationsdir = $(datadir)/applications
Applications_DATA = noot-tuner.desktop
