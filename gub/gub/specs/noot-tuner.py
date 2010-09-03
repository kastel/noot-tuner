from gub import target
from gub import tools

class Noot_tuner(target.AutoBuild):
	source = 'http://dl.dropbox.com/u/8231147/noot-tuner-0.2.0.tar.gz'
	dependencies = ['portaudio', 'libfftw', 'wxwidgets']
	#how to avoid the following line?
        configure_flags = (target.AutoBuild.configure_flags +
          ' --with-wx-config=%(targetdir)s/root/%(prefix_dir)s/bin/wx-config-replacement')
