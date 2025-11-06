subdir('plugins')

test('rin-registry', executable('test-rin-registry',
    ['test-rin-registry.c'],
    dependencies: [
      dependency('glib-2.0'),
      dependency('gobject-2.0'),
      rin_dep
      ],
    include_directories: include_directories('../'),
    c_args: ['-DRIN_BUILD_TEST']))
