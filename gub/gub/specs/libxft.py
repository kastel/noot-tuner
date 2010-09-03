from gub import target

class Libxft(target.AutoBuild):
	source = 'http://xlibs.freedesktop.org/release/libXft-2.1.7.tar.bz2'
	dependencies = ['libx11', 'libxrender']
