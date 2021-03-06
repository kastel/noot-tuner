AC_INIT(noot-tuner, 0.3.1, [dc.kastel@gmail.com])
AM_INIT_AUTOMAKE([foreign])

AM_SILENT_RULES([yes])

AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_SED
AC_PROG_GREP
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([version.h po/Makefile.in])
AC_CONFIG_MACRO_DIR([m4])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION(0.18.3)

CFLAGS="-Wall -O2 -g0 $CFLAGS"
CXXFLAGS="-Wall -O2 -g0 -fexceptions $CXXFLAGS -fno-strict-aliasing"
CXXDEBUG="-Wall -O0 -g3 -DDEBUG"
CDEBUG=$CXXDEBUG

AC_ARG_ENABLE(debug,
[[  --enable-wxdebug    Enable debugging]],
[CXXFLAGS="$CXXFLAGS $CXXDEBUG"; CFLAGS="$CFLAGS $CDEBUG"],
[])

AC_ARG_ENABLE(wxdebug,
[[  --enable-wxdebug    Link against wxWidgets-dbg]],
[WXDEBUG=--debug=yes],
[WXDEBUG=--debug=no])

AC_ARG_ENABLE(sndfile,
    AC_HELP_STRING([--disable-sndfile], [Disable sound file analysis (libsndfile)]),
    [ac_cv_sndfile=$enableval], [ac_cv_sndfile=1])

NOOT_LIBS=""

if test $ac_cv_sndfile == 1; then
        PKG_PROG_PKG_CONFIG

        PKG_CHECK_MODULES(SNDFILE, sndfile >= 1.0.2, ac_cv_sndfile=1, ac_cv_sndfile=0)

        if test $ac_cv_sndfile == 0; then
            AC_MSG_WARN([libsndfile not detected: disabling sound file support])
            ac_cv_sndfile=0
        else
            CXXFLAGS="$SNDFILE_CFLAGS $CXXFLAGS"
            NOOT_LIBS="$SNDFILE_LIBS $NOOT_LIBS"
        fi
fi

AC_DEFINE_UNQUOTED([HAVE_SNDFILE],${ac_cv_sndfile}, [Set to 1 if you have libsndfile.])
AC_DEFINE_UNQUOTED([SNDFILE_SUPPORT], $ac_cv_sndfile, [Set to 1 to activate sound file support])
AC_SUBST([NOOT_LIBS])

AM_OPTIONS_WXCONFIG

AC_CHECK_HEADER([portaudio.h], ,
	[AC_MSG_ERROR([Portaudio v19 is required to compile this program])])
AC_CHECK_LIB([portaudio],[Pa_GetDefaultInputDevice],,
	[AC_MSG_ERROR([Portaudio v19 is required to compile this program])])

AC_CHECK_HEADER([fftw3.h],,
	[AC_MSG_ERROR([FFTW3 is required to compile this program])])
AC_CHECK_LIB([fftw3],[fftw_cleanup],,
	[AC_MSG_ERROR([FFTW3 is required to compile this program])])

AC_CHECK_TOOL(WINDRES, [windres])

AM_CONDITIONAL(HAVE_WINDRES, [test $WINDRES])

#NOTE: forcing wxHTML help on Windows to avoid using ugly hhc.exe

#check if we are compiling with mingw
if $GREP mingw <<< $host > /dev/null; then
#  HELPEXTENSION=chm;
#  OPTLIBS=core,base;
  HELPEXTENSION=htb;
  OPTLIBS=core,base,html,adv;
else
  HELPEXTENSION=htb;
  OPTLIBS=core,base,html,adv;
fi

AC_SUBST(HELPEXTENSION)
AC_SUBST(WINDRES)


wxWin=0
AM_PATH_WXCONFIG(2.8.8, wxWin=1)
if test "$wxWin" != 1; then
    AC_MSG_ERROR([
            wxWidgets must be installed on your system
            but wx-config script couldn't be found.

            Please check that wx-config is in path, the directory
            where wxWidgets libraries are installed (returned by
            'wx-config --libs' command) is in LD_LIBRARY_PATH or
            equivalent variable and wxWidgets version is 2.8.8 or above.
    ])
fi

CPPFLAGS="$WX_CPPFLAGS $CPPFLAGS"
CXXFLAGS="$WX_CPPFLAGS $CXXFLAGS"

[VERSION_MAJOR=`echo ${VERSION}_ | sed "s/^\([0-9]*\).*$/\\1/"`]
[VERSION_MINOR=`echo ${VERSION}_ | sed "s/^\([0-9]*\)\.\([0-9]*\).*$/\\2/;s/.*_.*/0/"`]
[VERSION_BUILD=`echo ${VERSION}_ | sed "s/^\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*$/\\3/;s/.*_.*/0/"`]
[  VERSION_REV=`echo ${VERSION}_ | sed "s/^\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\)\.\([0-9]*\).*$/\\4/;s/.*_.*/0/"`]

AC_SUBST(VERSION_MAJOR)
AC_SUBST(VERSION_MINOR)
AC_SUBST(VERSION_BUILD)
AC_SUBST(VERSION_REV)

AC_SUBST(WX_LIBS)

AC_ARG_VAR([ZIP], [path to zip executable])
if test x$ZIP == x; then
    AC_PATH_PROG([ZIP], [zip])
    if test x$ZIP == x; then
            AC_MSG_WARN(['zip' program not found. Might not be able to build documentation]);
    fi
fi

AC_ARG_VAR([XSLTPROC], [path to xsltproc executable])
if test x$XSLTPROC == x; then
    AC_PATH_PROG([XSLTPROC], [xsltproc])
    if test x$XSLTPROC == x; then
            AC_MSG_WARN(['xsltproc' program not found. Might not be able to build documentation]);
    fi
fi

AC_ARG_VAR([LYX], [path to lyx executable])
if test x$LYX == x; then
    AC_PATH_PROG([LYX], [lyx])
    if test x$LYX == x; then
            AC_MSG_WARN(['lyx' program not found. Might not be able to build documentation]);
    fi
fi

AC_ARG_VAR([HTMLHELP_XSL], [path to htmlhelp.xsl])
if test x$HTMLHELP_XSL == x; then
    HTMLHELP_XSL=/usr/share/xml/docbook/stylesheet/docbook-xsl/htmlhelp/htmlhelp.xsl
fi

AC_OUTPUT(Makefile src/Makefile doc/Makefile doc/en/Makefile doc/it/Makefile)

