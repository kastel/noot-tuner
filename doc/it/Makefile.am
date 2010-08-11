#language signature
HLANG = it

EXTRA_DIST = noot-tuner-$(HLANG).tex texhelp.sty makeidx.sty tex2rtf.ini html pictures

helpdir = $(datadir)/$(PACKAGE)/$(HLANG)
dist_help_DATA = noot-tuner.$(HELPEXTENSION)

#don't give an error if help cannot be built

noot-tuner.htb: $(srcdir)/html/*
	( pushd $(srcdir)/html && ( cp -f ../pictures/* . && $(ZIP) -9 -r ../noot-tuner.htb *; popd ) ) || true

$(srcdir)/html/* : noot-tuner-$(HLANG).tex gpl-3.0.tex
	( pushd $(srcdir) && ( { if ! [ -e html ]; then mkdir html; fi } && tex2rtf noot-tuner-$(HLANG).tex html/noot-tuner -html; popd ) ) || true
