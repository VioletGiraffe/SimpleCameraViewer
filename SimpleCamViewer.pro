TEMPLATE = subdirs

SUBDIRS += qtutils cpputils app 

qtutils.subdir = qtutils
qtutils.depends = cpputils

cpputils.subdir = cpputils

app.subdir  = app
app.depends = qtutils
