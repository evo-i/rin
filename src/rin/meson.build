rin_sources = [
  'rin.c',
  'rin-project.c',
  'rin-metadata.c',
  'rin-info.c',
  'rin-settings.c',
  'rin-stats.c',
  'rin-archive.c',
  'rin-translation.c',
  'rin-translation-metadata.c',
  'rin-text.c',
]

rin_headers = [
  'rin.h',
  'rin-project.h',
  'rin-metadata.h',
  'rin-info.h',
  'rin-settings.h',
  'rin-stats.h',
  'rin-archive.h',
  'rin-translation.h',
  'rin-translation-metadata.h',
  'rin-text.h',
]

version_split = meson.project_version().split('.')
version_conf = configuration_data()
version_conf.set('VERSION', meson.project_version())
version_conf.set('MAJOR_VERSION', version_split[0])
version_conf.set('MINOR_VERSION', version_split[1])
version_conf.set('MICRO_VERSION', version_split[2])

configure_file(
          input: 'rin-version.h.in',
         output: 'rin-version.h',
  configuration: version_conf,
        install: true,
    install_dir: get_option('includedir') / 'rin'
)

rin_deps = [
  dependency('gio-2.0'),
  dependency('glib-2.0'),
  dependency('gobject-2.0'),
]

rin_lib = shared_library('rin-' + rin_api_version,
  rin_sources,
  dependencies: rin_deps,
  c_args: ['-DRIN_COMPILATION'],
  install: true,
)

rin_dep = declare_dependency(link_with: rin_lib,
  include_directories: include_directories('.'))

install_headers(rin_headers, subdir: 'rin')

pkg = import('pkgconfig')
pkg.generate(
  description: 'A shared library for ...',
    libraries: rin_lib,
         name: 'rin',
     filebase: 'rin-' + rin_api_version,
      version: meson.project_version(),
      subdirs: 'rin',
     requires: ['gio-2.0',
                'glib-2.0',
                'gobject-2.0'],
  install_dir: get_option('libdir') / 'pkgconfig'
)

gnome = import('gnome')
rin_gir = gnome.generate_gir(rin_lib,
              sources: [rin_sources, rin_headers],
               header: 'rin.h',
      export_packages: 'rin-' + rin_api_version,
            nsversion: rin_api_version,
            namespace: 'Rin',
        symbol_prefix: 'rin',
    identifier_prefix: 'Rin',
             includes: ['Gio-2.0', 'GObject-2.0', 'GLib-2.0'],
              install: true,
)
