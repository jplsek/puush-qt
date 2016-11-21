TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    src \
    app \
    tests
app.depends = src
tests.depends = src
