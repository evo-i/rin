dummy_sources = [
  'dummy_plugin.c',
]

dummy_plugin = shared_module('dummy_plugin',
  dummy_sources,
  include_directories: include_directories('../../'),
  c_args: ['-DRIN_BUILD_PLUGIN', '-static', '-static-libgcc', '-static-libstdc++'],
)
