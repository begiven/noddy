from distutils.core import setup, Extension

setup(name="noddy",
      version=1.0,
      ext_modules=[
          Extension("noddy", ["noddymodule.c"])
      ])

setup(name="shoddy",
      version=1.0,
      ext_modules=[
          Extension("shoddy", ["shoddymodule.c"])
      ])
