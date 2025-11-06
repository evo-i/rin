sw_sources = [
  'sw_plugin.c',
  'sw_private.c',
]

sw_deps = [
  dependency('libzip', required: true),
]

shared_module('soul_worker',
  sw_sources,
  dependencies: sw_deps,
  include_directories: include_directories('../../../'),
)
