name = 'core-dingjing'

qt5 = import ('qt5')
cmake = import('cmake')
pkg = import ('pkgconfig')

pkgconfigpath = join_paths('/usr/', get_option('libdir'), 'pkgconfig')
message('pkgconfig: @0@'.format(pkgconfigpath))

env_pkgconfig = environment()
env_pkgconfig.set('PKG_CONFIG_PATH', pkgconfigpath)

include_directories('.')

qt5_dep = dependency('qt5', modules: ['Core', 'Gui'])
c_library_deps = [
	dependency('xcb'),
	dependency('libexif'),
	dependency('gio-2.0'),
	dependency('glib-2.0'),
	dependency('gobject-2.0'),
	dependency('gthread-2.0'),
	dependency('Qt5', modules : ['Widgets']),
	dependency('Qt5', modules : ['Qt5::GuiPrivate']),
	dependency('Qt5', modules : ['X11Extras']),
]

c_library_cxx_sources = [
	'job.cpp',
	'utils.cpp',
	'folder.cpp',
	'fm-config.cpp',
	'file-path.cpp',
	'mime-type.cpp',
	'file-info.cpp',
	'icon-info.cpp',
	'thumbnailer.cpp',
	'dir-list-job.cpp',
	'file-info-job.cpp',
	'thumbnail-job.cpp',
	'volume-manager.cpp',
	'dnd-action-menu.cpp',
	'x-dnd-workaround.cpp',
	'filesystem-info-job.cpp',
]

c_library_headers = [
	'job.h',
	'job_p.h',
	'utils.h',
	'folder.h',
	'global.h',
	'file-path.h',
	'file-info.h',
	'icon-info.h',
	'mime-type.h',
	'fm-config.h',
	'smart-ptr.hpp',
	'clib-global.h',
	'icon-info-p.h',
	'thumbnailer.h',
	'file-info-p.h',
	'dir-list-job.h',
	'thumbnail-job.h',
	'file-info-job.h',
	'volume-manager.h',
	'dnd-action-menu.h',
	'x-dnd-workaround.h',
	'filesystem-info-job.h',
]

shared_lib = shared_library(name, c_library_cxx_sources,
  dependencies: c_library_deps,
  install: true,
)

pkg.generate (shared_lib)
