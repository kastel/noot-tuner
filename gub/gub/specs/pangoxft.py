from gub import gnome
from gub import target
from gub.specs import pango

class Pangoxft (pango.Pango):
    dependencies = pango.Pango.dependencies + ['libx11-devel', 'libxft-devel']
    configure_flags = pango.Pango.configure_flags + ' --with-x --with-dynamic-modules=yes'
    def get_conflict_dict (self):
        return {'': ['pango', 'pangocairo', 'pango-devel', 'pangocairo-devel', 'pango-doc', 'pangocairo-doc'], 'devel': ['pango', 'pangocairo', 'pango-devel', 'pangocairo-devel', 'pangocairo-doc', 'pango-doc'], 'doc': ['pango', 'pangocairo', 'pango-devel', 'pangocairo-devel', 'pango-doc', 'pangocairo-doc'], 'runtime': ['pango', 'pangocairo', 'pango-devel', 'pangocairo-devel', 'pango-doc', 'pangocairo-doc']}

class Pangoxft__mingw (Pangoxft):
    # FIXME: cut and paste Pango__mingw
    def create_config_files (self, prefix='/usr'):
        Pangoxft.create_config_files (self, prefix)
        etc = self.expand ('%(install_root)s/%(prefix)s/etc/pango', locals ())
        self.dump ('''${PANGO_PREFIX}/lib/pango/${PANGO_MODULE_VERSION}/modules/pango-basic-win32${PANGO_SO_EXTENSION} BasicScriptEngineWin32 PangoEngineShape PangoRenderWin32 common:
''', '%(etc)s/pango.modules', env=locals (), mode='a')
        Pangoxft.fix_config_files (self, prefix)
    configure_flags = (target.AutoBuild.configure_flags
                + ' --disable-rebuilds')

class Pangoxft__darwin (Pangoxft):
    source = 'http://ftp.gnome.org/pub/GNOME/platform/2.29/2.29.91/sources/pango-1.27.1.tar.gz'
    xxx_patches = Pangoxft.patches + [
        'pango-1.26.0-darwin-cx-font.patch',
        ]
    def configure (self):
        Pangoxft.configure (self)
        self.file_sub ([('nmedit', '%(target_architecture)s-nmedit')],
                       '%(builddir)s/libtool')
    def install (self):
        Pangoxft.install (self)                
        # FIXME: PANGO needs .so, NOT .dylib?
        self.dump ('''
set PANGO_SO_EXTENSION=.so
''', '%(install_prefix)s/etc/relocate/pango.reloc', env=locals (), mode='a')

class Pangoxft__darwin__no_quartz_objective_c (Pangoxft):
    config_cache_overrides = Pangoxft.config_cache_overrides + '''
ac_cv_header_Carbon_Carbon_h=${ac_cv_header_Carbon_Carbon_h=no}
'''
