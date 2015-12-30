#!/bin/sh
# 
# Call this script after downloading the source from a SCM
# (svn, Mercurial, git, ...)
#
# This assumes that autoconf, automake, autopoint, libtoolize, etc are
# installed.

autoreconf --force --install
rm po/Makevars.template
