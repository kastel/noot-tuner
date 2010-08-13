from gub import target

class Libsm(target.AutoBuild):
	source = 'http://xorg.freedesktop.org/archive/individual/lib/libSM-1.1.1.tar.bz2'
	dependencies = ['libx11-devel', 'xproto', 'libice-devel']
