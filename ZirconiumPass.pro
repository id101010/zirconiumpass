TEMPLATE = subdirs
SUBDIRS = app \
          src \
          tests

tests.depends =src
app.depends = src
