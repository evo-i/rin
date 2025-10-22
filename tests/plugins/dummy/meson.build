dummy_test_sources = [
  'dummy_test.c',
]

dummy_test_gmodule_sources = [
  'dummy_test_gmodule.c',
]


dummy_dependencies = [
  dependency('glib-2.0'),
  dependency('gmodule-2.0'),
]

dummy_plugin_test = executable('dummy_plugin_test',
  dummy_test_sources,
  dependencies: dummy_dependencies,
  include_directories: include_directories('../../../'),
  c_args: ['-DRIN_BUILD_PLUGIN_TEST'],
)

test('Rin dummy plugin test',
  dummy_plugin_test,
  depends: dummy_plugin,
  env: {'RIN_PLUGIN_PATH': meson.current_build_dir() / '/plugins/'}
)


dummy_plugin_test_gmodule = executable('dummy_plugin_test_gmodule',
  dummy_test_gmodule_sources,
  dependencies: dummy_dependencies,
  include_directories: include_directories('../../../'),
  c_args: ['-DRIN_BUILD_PLUGIN_TEST'],
)

test('Rin dummy plugin test (gmodule)',
  dummy_plugin_test_gmodule,
  depends: dummy_plugin,
  env: {'RIN_PLUGIN_PATH': meson.current_build_dir() / '/plugins/'}
)
