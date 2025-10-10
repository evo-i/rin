soul_worker_sources = [
  'rin-soul-worker-plugin.c',
]

soul_worker_deps = [
  dependency('glib-2.0'),
  dependency('gmodule-2.0'),
  dependency('gobject-2.0'),
  dependency('gio-2.0'),
  rin_dep,
]

shared_library('rin-plugin-soul-worker',
  soul_worker_sources,
  c_args: ['-D__rin_plugin__'],
  dependencies: soul_worker_deps,
)
