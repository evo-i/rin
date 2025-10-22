rin_sources = [
  'rin-registry.c',
  'rin-registry-entry.c',
]

rin_headers = [
  'rin-registry.h',
  'rin-registry-entry.h',
]

rin_dependencies = [
  dependency('gio-2.0'),
  dependency('glib-2.0'),
  dependency('gmodule-2.0'),
  dependency('gobject-2.0'),
]

rin_lib = library('rin',
  rin_sources,
  include_directories : include_directories('.'),
  dependencies : rin_dependencies,
  version : '1.0.0',
  soversion : '1',
)

rin_dep = declare_dependency(
  link_with : rin_lib,
  include_directories : include_directories('.'),
)
