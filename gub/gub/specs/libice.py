from gub import target

class Libice(target.AutoBuild):
	source = 'http://xorg.freedesktop.org/archive/individual/lib/libICE-1.0.6.tar.bz2'
	dependencies = ['libx11-devel', 'xproto']
