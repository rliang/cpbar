import os
import ycm_core

flags = [
'-x', 'c',
'-std=c11',
'-I', '/usr/include/xcb/',
'-I', '/usr/include/cairo/',
'-I', '/usr/include/libdrm/',
'-I', '/usr/include/libpng16/',
'-I', '/usr/include/pixman-1/',
'-I', '/usr/include/freetype2/',
'-I', '/usr/include/glib-2.0/',
'-I', '/usr/lib/glib-2.0/include/',
'-I', '/usr/include/pango-1.0/',
'-pthread',
'-Wall',
'-Wextra',
'-pedantic',
'-DUSE_CLANG_COMPLETER'
]

def DirectoryOfThisScript():
  return os.path.dirname(os.path.abspath(__file__))

def FlagsForFile(filename):
  return {
    'flags': flags,
    'do_cache': True
  }
