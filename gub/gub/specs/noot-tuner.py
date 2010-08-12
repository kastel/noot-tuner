from gub import target
from gub import tools

class Noot_tuner(target.AutoBuild):
	source = 'http://dl.dropbox.com/u/8231147/noot-tuner-0.1.0.1.tar.gz'
	dependencies = ['portaudio', 'libfftw', 'wxwidgets']
        configure_flags = (tools.AutoBuild.configure_flags +
          ' --with-wx-config="%(targetdir)s/root/%(prefix_dir)s/bin/wx-config --prefix=%(targetdir)s/root/%(prefix_dir)s"')
