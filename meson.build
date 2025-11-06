project ('rin', 'c',
  version : '1.0.0',
  default_options : ['c_std=c17']
)

# core directory for the Rin Library
subdir('src')
subdir('plugins')
subdir('vizar')
# subdir('utils')

if get_option('examples')
  subdir('examples')
endif

if get_option('tests')
  subdir('tests')
endif
