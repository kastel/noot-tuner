from gub import gnome
from gub import target
from gub.specs import pango

class Pangox (pango.Pango):
    dependencies = pango.Pango.dependencies + ['libx11-devel']
    configure_flags = pango.Pango.configure_flags + ' --with-x'
    def get_conflict_dict (self):
        return {'': ['pango', 'pango-devel', 'pango-doc'], 'devel': ['pango', 'pango-devel', 'pango-doc'], 'doc': ['pango', 'pango-devel', 'pango-doc'], 'runtime': ['pango', 'pango-devel', 'pango-doc']}

class Pangox__mingw (Pangox):
    # FIXME: cut and paste Pango__mingw
    def create_config_files (self, prefix='/usr'):
        Pangox.create_config_files (self, prefix)
        etc = self.expand ('%(install_root)s/%(prefix)s/etc/pango', locals ())
        self.dump ('''${PANGO_PREFIX}/lib/pango/${PANGO_MODULE_VERSION}/modules/pango-basic-win32${PANGO_SO_EXTENSION} BasicScriptEngineWin32 PangoEngineShape PangoRenderWin32 common:
''', '%(etc)s/pango.modules', env=locals (), mode='a')
        Pangox.fix_config_files (self, prefix)
    configure_flags = (target.AutoBuild.configure_flags
                + ' --enable-x --disable-rebuilds')

class Pangox__darwin (Pangox):
    source = 'http://ftp.gnome.org/pub/GNOME/platform/2.29/2.29.91/sources/pango-1.27.1.tar.gz'
    xxx_patches = Pangox.patches + [
        'pango-1.26.0-darwin-cx-font.patch',
        ]
    def configure (self):
        Pangox.configure (self)
        self.file_sub ([('nmedit', '%(target_architecture)s-nmedit')],
                       '%(builddir)s/libtool')
    def install (self):
        Pangox.install (self)                
        # FIXME: PANGO needs .so, NOT .dylib?
        self.dump ('''
set PANGO_SO_EXTENSION=.so
''', '%(install_prefix)s/etc/relocate/pango.reloc', env=locals (), mode='a')

class Pangox__darwin__no_quartz_objective_c (Pangox):
    config_cache_overrides = Pangox.config_cache_overrides + '''
ac_cv_header_Carbon_Carbon_h=${ac_cv_header_Carbon_Carbon_h=no}
'''
