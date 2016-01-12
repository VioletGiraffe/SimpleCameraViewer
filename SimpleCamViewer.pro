TEMPLATE = subdirs

SUBDIRS += qtutils cpputils app github-releases-autoupdater

cpputils.subdir = cpputils

qtutils.subdir = qtutils
qtutils.depends = cpputils

github-releases-autoupdater.subdir = github-releases-autoupdater
github-releases-autoupdater.depends = cpputils

app.subdir  = app
app.depends = qtutils github-releases-autoupdater
