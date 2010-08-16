#language signature
HLANG = it

helpdir = $(datadir)/$(PACKAGE)

EXTRA_DIST = noot-tuner.lyx gpl-3.2.lyx gpl-3.2.sgml noot-tuner.xml html pictures

dist_help_DATA = noot-tuner.$(HELPEXTENSION)

#don't give an error if help cannot be built

noot-tuner.htb: $(srcdir)/html
	( pushd $(srcdir)/html && ( cp -fr ../pictures . && $(ZIP) -9 -r ../noot-tuner.htb *; popd ) ) || echo "WARNING: Failed to create documentation HTB file"

noot-tuner.xml gpl-3.2.sgml: noot-tuner.lyx gpl-3.2.lyx
	( cd $(srcdir) && rm -f noot-tuner.xml gpl-3.2.sgml && $(LYX) noot-tuner.lyx -e docbook-xml ) || echo "WARNING: Failed to update docbook documentation"

$(srcdir)/html : noot-tuner.xml gpl-3.2.sgml
	( cd $(srcdir) && ( rm -rf html; mkdir html; cd html && \
	$(XSLTPROC) --stringparam htmlhelp.use.hhk 1 \
                 $(HTMLHELP_XSL) ../noot-tuner.xml ) ) || echo "WARNING: Failed to translate documentation into HTML Help"
