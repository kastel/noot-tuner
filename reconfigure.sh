#!/bin/sh
# 
# Call this script after downloading the source from a SCM
# (svn, Mercurial, git, ...)

aclocal
libtoolize -i
autoconf
automake --add-missing
