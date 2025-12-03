src = [
	'about.c',
	'calendar.c',
	'color.c',
	'cpicker.c',
	'dnd.c',
	'entry.c',
	'file.c',
	'font.c',
	'form.c',
	'icons.c',
	'list.c',
	'main.c',
	'notebook.c',
	'notification.c',
	'option.c',
	'paned.c',
	'picture.c',
	'print.c',
	'progress.c',
	'scale.c',
	'text.c',
	'util.c',
]

cfg = configuration_data()

cfg.set_quoted('PACKAGE_NAME', meson.project_name())
cfg.set_quoted('PACKAGE_VERSION', meson.project_version())
cfg.set_quoted('PACKAGE_URL', 'http://github.com/step-/yad')
cfg.set_quoted('RGB_FILE', get_option('rgb'))
cfg.set('HAVE_TRAY', 1)

dep = [
	meson.get_compiler('c').find_library('m'),
	dependency('gtk+-3.0'),
	dependency('gtk+-unix-print-3.0'),
]

gtk_layer_shell = dependency('gtk-layer-shell-0', required: false)
if gtk_layer_shell.found()
	dep += gtk_layer_shell
	cfg.set('HAVE_GTK_LAYER_SHELL', 1)
endif

# TODO add html widget (webkit2gtk-4.1 && gtk3, default false)

configure_file(
	output : 'config.h',
	configuration : cfg,
)

yad = executable(
	'yad',
	src,
	dependencies: dep,
	install: true,
)
