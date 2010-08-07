helpdir = $(datadir)/$(PACKAGE)

EXTRA_DIST = noot-tuner-en.tex texhelp.sty makeidx.sty noot-tuner.chm tex2rtf.ini html pictures

dist_help_DATA = noot-tuner.$(HELPEXTENSION)

#don't give an error if help cannot be built

noot-tuner.htb: $(srcdir)/html/*
	( pushd $(srcdir)/html && ( cp -f ../pictures/* . && $(ZIP) -9 -r ../noot-tuner.htb *; popd ) ) || true

$(srcdir)/html/* : noot-tuner-en.tex
	( pushd $(srcdir) && ( { if ! [ -e html ]; then mkdir html; fi } && tex2rtf noot-tuner-en.tex html/noot-tuner -html; popd ) ) || true

#how to compile in CHM? currently compilation is manual... FIXME