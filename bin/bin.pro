TEMPLATE = app
TARGET = qforex.sh
OBJECTS_DIR =

PRE_TARGETDEPS = $$PWD/qforex.sh

QMAKE_LINK = cp $$PWD/qforex.sh $@ && : IGNORE REST OF LINE:
QMAKE_STRIP =
CONFIG -= qt separate_debug_info gdb_dwarf_index

QMAKE_CLEAN = qforex.sh

target.path  = $$QTC_PREFIX/bin
INSTALLS    += target

OTHER_FILES = $$PWD/qforex.sh
