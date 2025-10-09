soul_worker_sources = [
  'rin-soul-worker-kit.c',
  'rin-soul-worker-probe.c',
  'rin-soul-worker-module.c',
]

soul_worker_headers = [
  'rin-soul-worker-kit.h',
  'rin-soul-worker-probe.h',
  'rin-soul-worker-module.h',
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
  c_args: ['-DRIN_PLUGIN']
)
