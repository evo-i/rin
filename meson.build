project('rin', 'c',
          version: '0.1.0',
    meson_version: '>= 1.0.0',
  default_options: [ 'warning_level=2', 'werror=false', 'c_std=c17', ],
)

cc = meson.get_compiler('c')

config_h = configuration_data()
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
configure_file(output: 'config.h', configuration: config_h)
add_project_arguments(['-I' + meson.project_build_root()], language: 'c')

project_c_args = []

gcc_clang_test_c_args = [
  '-Wcast-align',
  '-Wdeclaration-after-statement',
  '-Werror=address',
  '-Werror=array-bounds',
  '-Werror=empty-body',
  '-Werror=implicit',
  '-Werror=implicit-function-declaration',
  '-Werror=incompatible-pointer-types',
  '-Werror=init-self',
  '-Werror=int-conversion',
  '-Werror=int-to-pointer-cast',
  '-Werror=main',
  '-Werror=misleading-indentation',
  '-Werror=missing-braces',
  '-Werror=missing-include-dirs',
  '-Werror=nonnull',
  '-Werror=overflow',
  '-Werror=parentheses',
  '-Werror=pointer-arith',
  '-Werror=pointer-to-int-cast',
  '-Werror=redundant-decls',
  '-Werror=return-type',
  '-Werror=sequence-point',
  '-Werror=shadow',
  '-Werror=strict-prototypes',
  '-Werror=trigraphs',
  '-Werror=undef',
  '-Werror=write-strings',
  '-Wformat-nonliteral',
  '-Wignored-qualifiers',
  '-Wimplicit-function-declaration',
  '-Wlogical-op',
  '-Wmissing-declarations',
  '-Wmissing-format-attribute',
  '-Wmissing-include-dirs',
  '-Wmissing-noreturn',
  '-Wnested-externs',
  '-Wno-cast-function-type',
  '-Wno-dangling-pointer',
  '-Wno-missing-field-initializers',
  '-Wno-sign-compare',
  '-Wno-unused-parameter',
  '-Wold-style-definition',
  '-Wpointer-arith',
  '-Wredundant-decls',
  '-Wstrict-prototypes',
  '-Wswitch-default',
  '-Wswitch-enum',
  '-Wundef',
  '-Wuninitialized',
  '-Wunused',
  '-fno-strict-aliasing',
  ['-Werror=format-security', '-Werror=format=2'],
]

msvc_test_c_args = [
  '/wd4201',       # nameless struct/union
  '/wd4204',       # nonstandard extension used: non-constant aggregate initializer
  '/wd4100',       # unreferenced formal parameter
  '/wd4127',       # conditional expression is constant
  '/wd4189',       # local variable is initialized but not referenced
  '/wd4324',       # structure was padded due to alignment specifier
  '/wd4996',       # POSIX name is deprecated
  '/wd4464',       # relative include path contains '..'
  '/wd4710',       # function not inlined
  '/wd4711',       # function selected for automatic inline expansion
  '/wd4820',       # padding added after data member
  '/wd5045',       # compiler will insert Spectre mitigation
  '/std:c17',      # check if compiler supports c17 standard.
]

if get_option('buildtype') != 'plain'
  if cc.get_id() != 'msvc'
    gcc_clang_test_c_args += '-fstack-protector-strong'
  endif
endif

if cc.get_id() == 'msvc'
  foreach arg: msvc_test_c_args
    if cc.has_argument(arg) and arg != '/std:c17'
      project_c_args += arg
    endif
  endforeach
else
  foreach arg: gcc_clang_test_c_args
    if cc.has_multi_arguments(arg)
      project_c_args += arg
    endif
  endforeach
endif

add_project_arguments(project_c_args, language: 'c')

subdir('src')
