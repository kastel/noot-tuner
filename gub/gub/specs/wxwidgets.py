import os

from gub import target
from gub import tools
from gub import context

class Wxwidgets(target.AutoBuild):
	source = 'https://downloads.sourceforge.net/project/wxwindows/wxAll/2.9.0/wxWidgets-2.9.0.tar.gz?r=http%%3A%%2F%%2Fwxwidgets.org%%2Fdownloads%%2F&ts=1281666329&mirror=sunet'
	#source = 'https://downloads.sourceforge.net/project/wxwindows/2.8.11/wxWidgets-2.8.11.tar.gz?r=http%%3A%%2F%%2Fwxwidgets.org%%2Fdownloads%%2F&ts=1281469491&mirror=heanet'
	configure_flags = target.AutoBuild.configure_flags + ' --without-subdirs --without-gnomeprint PKG_CONFIG=%(targetdir)s/pkg-config-replacement'
	dependencies = ['libtiff','libpng','gettext']
	def patch(self): #Create a fake pkg-config which prefixes all include and library paths with the target directory
		fake_pkgc_path = '%(targetdir)s/pkg-config-replacement' % self.get_substitution_dict()
		os.system('''
echo "#!/bin/sh
export PKG_CONFIG_PATH=%(targetdir)s/root/%(prefix_dir)s/lib/pkgconfig:%(targetdir)s/root/%(prefix_dir)s/share/pkgconfig
%(tools_prefix)s/bin/pkg-config \$* | sed 's#-\([IL]\)/#-\\\\\\1%(targetdir)s/root/#g'" > %(targetdir)s/pkg-config-replacement

chmod +x %(targetdir)s/pkg-config-replacement
''' % self.get_substitution_dict())
		target.AutoBuild.patch(self)
	def install(self):
		target.AutoBuild.install(self)
		#Create a fake wx-config which prefixes all include and library paths with the target directory
		#(use this as WXCONFIG in programs which use wxWidgets)
		os.system(
'''
echo "#!/bin/sh
%(targetdir)s/root/%(prefix_dir)s/bin/wx-config \$* | sed 's#-\([IL]\)/#-\\\\\\1%(targetdir)s/root/#g'" > %(targetdir)s/root/%(prefix_dir)s/bin/wx-config-replacement

chmod +x %(targetdir)s/root/%(prefix_dir)s/bin/wx-config-replacement
''' % self.get_substitution_dict())

#First version: wxGTK (uncomment which you prefer)
class Wxwidgets__linux(Wxwidgets):
	configure_flags = Wxwidgets.configure_flags + ' --with-x11 --enable-unicode'
	dependencies = Wxwidgets.dependencies + ['libsm-devel', 'libx11-devel', 'libxinerama-devel', 'pangoxft-devel']

#Second version: wxX11
#class Wxwidgets__linux(Wxwidgets):
#	configure_flags = Wxwidgets.configure_flags + ' --without-x11'
#	dependencies = Wxwidgets.dependencies + ['gtk+-devel', 'tools::glib', 'libxinerama']


