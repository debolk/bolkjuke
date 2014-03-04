AM_CPPFLAGS = -I$(top_srcdir)

bin_PROGRAMS = bolkjuke

bolkjuke_SOURCES = \
    main.c main.h \
    settings.c settings.h \
    controller.c controller.h \
    gui.c gui.h \
    input.c input.h \
    window.c window.h \
    info.c info.h \
    list.c list.h \
    bar.c bar.h

bolkjuke_LDADD = $(top_builddir)/lib/libbolkjuke.la

bolkjuke_LDFLAGS = -lncurses -lmenu -lxine
