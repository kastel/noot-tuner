#!/bin/sh
# 
# Call this script after downloading the source from a SCM
# (svn, Mercurial, git, ...)

touch ChangeLog
libtoolize -i
gettextize --no-changelog -f
mv -f Makefile.am~ Makefile.am
mv -f configure.ac~ configure.ac
rm po/Makevars.template
aclocal
autoconf
automake --add-missing
