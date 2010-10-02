# these are the headers for your project
noinst_HEADERS = audio_file.h audio_portaudio.h audioio.h wxfbtunerframe.h \
	noot-tuner.h notedetection.h options.h tunerframe.h volumemeter.h \
	nogui.h autocov.h powerspectrum.h

EXTRA_DIST = tunerframe.fpb noot-tuner.desktop

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  its name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = noot-tuner

noot_tuner_SOURCES = audio_file.cpp audio_portaudio.cpp audioio.cpp \
	wxfbtunerframe.cpp noot-tuner.cpp deviceselector.cpp notedetection.cpp options.cpp \
	tunerframe.cpp volumemeter.cpp nogui.cpp autocov.cpp powerspectrum.cpp

# the application source, library search path, and link libraries
if HAVE_WINDRES
    noot_tuner_SOURCES += res.rc
endif

noot_tuner_LDFLAGS = $(WX_LIBS) -lfftw3 -lportaudio $(NOOT_LIBS)

res_verbose = $(res_verbose_$(V))
               res_verbose_ = $(res_verbose_$(AM_DEFAULT_VERBOSITY))
               res_verbose_0 = @echo "  RES    $@";

.rc.o: ../version.h
	$(res_verbose) $(WINDRES) $(CPPFLAGS) -I.. -o $@ $<

Applicationsdir = $(datadir)/applications
Applications_DATA = noot-tuner.desktop
