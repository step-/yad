# write PACKAGE_URL as "http" not "https" for About dialog to display a clickable link in the Credits tab.
AC_INIT([YAD], [0.42.89], [https://github.com/step-/yad/issues], yad, [http://github.com/step-/yad])
AC_CONFIG_AUX_DIR([build])

AM_INIT_AUTOMAKE([1.11 foreign dist-xz no-dist-gzip])
AM_SILENT_RULES([yes])

AC_PREREQ([2.59])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src])

AM_MAINTAINER_MODE

IT_PROG_INTLTOOL([0.40.0])

AC_PROG_CC

AC_ARG_WITH([gtk],
	[AS_HELP_STRING([--with-gtk=gtk2|gtk3],[set the GTK+ version to use (default - gtk2)])],
	[case "$with_gtk" in
        gtk2|gtk3) ;;
        *) AC_MSG_ERROR([invalid gtk version specified]) ;;
     esac],
    [with_gtk=gtk2])

case "$with_gtk" in
    gtk3)
        GTK_MODULE="gtk+-3.0"
        GTK_REQUIRED="3.22.0"
        GTK_PRINT="gtk+-unix-print-3.0"
        WEBKIT="webkit2gtk-4.1"
        SPELL="gtkspell3-3.0"
        SOURCEVIEW="gtksourceview-3.0"
        ;;
    gtk2)
        GTK_MODULE="gtk+-2.0"
        GTK_REQUIRED="2.24.0"
        GTK_PRINT="gtk+-unix-print-2.0"
        WEBKIT="webkit-1.0"
        SPELL="gtkspell3-2.0"
        SOURCEVIEW="gtksourceview-2.0"
        ;;
esac
PKG_CHECK_MODULES([GTK], [$GTK_MODULE >= $GTK_REQUIRED $GTK_PRINT])
AC_SUBST([GTK_CFLAGS])
AC_SUBST([GTK_LIBS])
AM_CONDITIONAL([GTK2], [test x$with_gtk = xgtk2])

GTK_DISABLE_DEPRECATED

dnl rgb.txt
AC_ARG_WITH([rgb],
	[AS_HELP_STRING([--with-rgb=PATH],
			[set path to rgb.txt file (default - /etc/X11/rgb.txt)])],
	[with_rgb=$withval], [with_rgb=/etc/X11/rgb.txt])
AC_DEFINE_UNQUOTED([RGB_FILE], ["$with_rgb"], [Path to rgb.txt file])

dnl html widget
AC_ARG_ENABLE([html],
	[AS_HELP_STRING([--enable-html],
			[Build YAD with HTML widget (default - no)])],
	[build_html=$enableval], [build_html=no])
if test x$build_html = xyes; then
    PKG_CHECK_MODULES([HTML], [$WEBKIT], [have_html=yes], [have_html=no])
    if test x$with_gtk = xgtk3; then
	if test $have_html = no; then
	    AC_MSG_WARN([$WEBKIT not found -- not building the HTML widget])
	fi
	PKG_CHECK_MODULES([GMODULE], [gmodule-2.0], [have_gmodule=yes], [have_gmodule=no])
	if test $have_gmodule = no; then
	    AC_MSG_WARN([GModule not found -- not building the HTML widget])
	    have_html=no
	fi
    fi
else
    have_html=no
fi
AC_SUBST([HTML_CFLAGS])
AC_SUBST([HTML_LIBS])
if test x$build_html = xyes && test x$with_gtk = xgtk3; then
## because webkitgtk is dynamically loaded
HTML_LIBS=
fi
AM_CONDITIONAL([HTML], [test x$have_html = xyes])
AC_SUBST([GMODULE_CFLAGS])
AC_SUBST([GMODULE_LIBS])
AM_CONDITIONAL([GMODULE], [test x$have_gmodule = xyes])

if test x$have_html = xyes; then
    AC_DEFINE([HAVE_HTML], [1], [Define this if the webkitgtk library is installed])
    if test x$with_gtk = xgtk3; then
	AC_DEFINE_UNQUOTED([WEBKITGTK_LIBNAME], ["lib$WEBKIT.so"], [Define the webkitgtk library file name to be dynamically loaded])
    fi
fi

dnl gio support
AC_ARG_ENABLE([gio],
	[AS_HELP_STRING([--enable-gio],
			[Build YAD with GIO support])],
	[build_gio=$enableval], [build_gio=yes])
if test x$build_gio = xyes; then
    PKG_CHECK_MODULES([GIO], [gio-2.0], [have_gio=yes], [have_gio=no])
else
    have_gio=no
fi
AC_SUBST([GIO_CFLAGS])
AC_SUBST([GIO_LIBS])

if test x$have_gio = xyes; then
    AC_DEFINE([HAVE_GIO], [1], [Define this if you have glib gio framework])
fi

dnl status icon widget
AC_ARG_ENABLE([tray],
	[AS_HELP_STRING([--enable-tray],
			[Build YAD with status icon])],
	[build_tray=$enableval], [build_tray=yes])
AM_CONDITIONAL([TRAY], [test x$build_tray = xyes])

if test x$build_tray = xyes; then
    AC_DEFINE([HAVE_TRAY], [1], [Define this if you need status icon])
fi

dnl spell check
AC_ARG_ENABLE([spell],
	[AS_HELP_STRING([--enable-spell],
			[Build YAD with Spell check support (default - no)])],
	[with_spell=$enableval], [with_spell=no])
if test x$with_spell = xyes; then
    PKG_CHECK_MODULES([SPELL], [$SPELL], [have_spell=yes], [have_spell=no])
else
    have_spell=no
fi
AC_SUBST([SPELL_CFLAGS])
AC_SUBST([SPELL_LIBS])

if test x$have_spell = xyes; then
    AC_DEFINE([HAVE_SPELL], [1], [Define this if you need spell check support])
fi

dnl sourceview
AC_ARG_ENABLE([sourceview],
	[AS_HELP_STRING([--enable-sourceview],
			[Build YAD with GtkSourceView support (default - no)])],
	[with_sourceview=$enableval], [with_sourceview=no])
if test x$with_sourceview = xyes; then
    PKG_CHECK_MODULES([SOURCEVIEW], [$SOURCEVIEW], [have_sourceview=yes], [have_sourceview=no])
else
    have_sourceview=no
fi
AC_SUBST([SOURCEVIEW_CFLAGS])
AC_SUBST([SOURCEVIEW_LIBS])

if test x$have_sourceview = xyes; then
    AC_DEFINE([HAVE_SOURCEVIEW], [1], [Define this if you need GtkSourceView support])
fi

dnl tools
AC_ARG_ENABLE([tools],
	[AS_HELP_STRING([--enable-tools],
			[Build yad-tools utility])],
	[build_tools=$enableval], [build_tools=yes])
AM_CONDITIONAL([BUILD_TOOLS], [test x$build_tools = xyes])

dnl icon browser
AC_ARG_ENABLE([icon-browser],
	[AS_HELP_STRING([--enable-icon-browser],
			[Build YAD icon browser])],
	[build_ib=$enableval], [build_ib=yes])
AM_CONDITIONAL([BUILD_IB], [test x$build_ib = xyes])

AC_SEARCH_LIBS(round, m)

# *******************************
# Internationalization
# *******************************

GETTEXT_PACKAGE=yad
AC_SUBST([GETTEXT_PACKAGE])
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext domain])

AM_GLIB_GNU_GETTEXT

# *******************************

AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_LDFLAGS])

# *******************************

AC_CONFIG_FILES([
Makefile
po/Makefile.in
src/Makefile
data/Makefile
data/icons/Makefile
data/icons/16x16/Makefile
data/icons/24x24/Makefile
data/icons/32x32/Makefile
data/icons/48x48/Makefile
data/icons/96x96/Makefile
data/icons/128x128/Makefile
data/yad.m4
data/yad.spec
])

AC_OUTPUT

echo
echo "Build configuration:"
echo "  GTK+ version         - $with_gtk"
echo "  Path to rgb.txt      - $with_rgb"
echo "  Status icon          - $build_tray"
printf "%s" "  HTML widget          - $have_html"
if ! test x$have_gmodule = xyes && test x$with_gtk = xgtk3; then
    echo " (GModule not found)"
else
    echo
fi
echo "  Spell checking       - $have_spell"
echo "  GtkSourceView        - $have_sourceview"
echo "  GIO support          - $have_gio"
echo "  Tools                - $build_tools"
echo "  Icon browser         - $build_ib"
echo
