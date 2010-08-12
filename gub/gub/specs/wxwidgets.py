import os

from gub import target
from gub import tools
from gub import context

class Wxwidgets(target.AutoBuild):
	source = 'https://downloads.sourceforge.net/project/wxwindows/2.8.11/wxWidgets-2.8.11.tar.gz?r=http%%3A%%2F%%2Fwxwidgets.org%%2Fdownloads%%2F&ts=1281469491&mirror=heanet'
	configure_flags = tools.AutoBuild.configure_flags + ' --without-subdirs --without-gnomeprint PKG_CONFIG=%(targetdir)s/pkg-config-replacement'
	dependencies = ['libtiff','libpng','gettext']
	def configure(self):
		fake_pkgc_path = '%(targetdir)s/pkg-config-replacement' % self.get_substitution_dict()
		os.system('''
echo "#!/bin/sh
export PKG_CONFIG_PATH=%(targetdir)s/root/%(prefix_dir)s/lib/pkgconfig:%(targetdir)s/root/%(prefix_dir)s/share/pkgconfig
%(tools_prefix)s/bin/pkg-config \$* | sed 's#-\([IL]\)/#-\\\\\\1%(targetdir)s/root/#g'" > %(targetdir)s/pkg-config-replacement

chmod +x %(targetdir)s/pkg-config-replacement
''' % self.get_substitution_dict())
		target.AutoBuild.configure(self)

#class Wxwidgets__linux(Wxwidgets):
#	configure_flags = tools.AutoBuild.configure_flags + '--with-x11 --without-subdirs'
#	dependencies = Wxwidgets.dependencies + ['libsm-devel', 'libx11-devel', 'libxinerama-devel'] #['gtk+-devel', 'tools::glib']

class Wxwidgets__linux(Wxwidgets):
	dependencies = Wxwidgets.dependencies + ['gtk+-devel', 'tools::glib']

