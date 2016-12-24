TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = \
    lib/uglobalhotkey \
    src \
    app \
    tests
src.depends = lib/uglobalhotkey
app.depends = src
tests.depends = src
