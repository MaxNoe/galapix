# -*- mode: python -*-

galapix_sources = [
    #    'src/md5.cpp',
    # 'src/job.cpp',
    'src/blob.cpp',
    'src/database.cpp',
    'src/database_thread.cpp',
    'src/exec.cpp',
    'src/file_database.cpp',
    'src/file_entry.cpp',
    'src/file_reader.cpp',
    'src/filesystem.cpp',
    'src/framebuffer.cpp',
    'src/galapix.cpp',
    'src/image.cpp',
    'src/job_handle.cpp',
    'src/job_manager.cpp',
    'src/job_worker_thread.cpp',
    'src/jobs/tile_generation_job.cpp',
    'src/jobs/test_job.cpp',
    'src/lisp/getters.cpp',
    'src/lisp/lexer.cpp',
    'src/lisp/lisp.cpp',
    'src/lisp/parser.cpp',
    'src/math.cpp',
    'src/math/rect.cpp',
    'src/math/size.cpp',
    'src/math/vector2f.cpp',
    'src/math/vector2i.cpp',
    'src/math/vector3f.cpp',
    'src/plugins/curl.cpp',
    'src/plugins/imagemagick.cpp',
    'src/plugins/jpeg.cpp',
    'src/plugins/jpeg_memory_dest.cpp',
    'src/plugins/jpeg_memory_src.cpp',
    'src/plugins/kra.cpp',
    'src/plugins/png.cpp',
    'src/plugins/pnm.cpp',
    'src/plugins/rar.cpp',
    'src/plugins/rsvg.cpp',
    'src/plugins/tar.cpp',
    'src/plugins/ufraw.cpp',
    'src/plugins/xcf.cpp',
    'src/plugins/zip.cpp',
    'src/selection.cpp',
    'src/sexpr_file_reader.cpp',
    'src/sexpr_file_writer.cpp',
    'src/software_surface.cpp',
    'src/sqlite/connection.cpp',
    'src/sqlite/error.cpp',
    'src/sqlite/reader.cpp',
    'src/sqlite/statement.cpp',
    'src/surface.cpp',
    'src/texture.cpp',
    'src/thread.cpp',
    'src/tile_database.cpp',
    'src/tools/grid_tool.cpp',
    'src/tools/move_tool.cpp',
    'src/tools/pan_tool.cpp',
    'src/tools/resize_tool.cpp',
    'src/tools/rotate_tool.cpp',
    'src/tools/view_rotate_tool.cpp',
    'src/tools/zoom_rect_tool.cpp',
    'src/tools/zoom_tool.cpp',
    'src/url.cpp',
    'src/viewer.cpp',
    'src/viewer_state.cpp',
    'src/workspace.cpp',
    ]

spacenav_sources = ['src/space_navigator.cpp']

sdl_sources = ['src/sdl_framebuffer.cpp',
               'src/sdl_viewer.cpp']

gtk_sources =['src/gtk_viewer.cpp',
              'src/gtk_viewer_widget.cpp']

# Configure things here
# ------------------------
compile_galapix_sdl = True
compile_galapix_gtk = True
compile_spacenav    = True
# ------------------------

optional_sources = []
optional_defines = []
optional_libs    = []

if compile_spacenav:
    optional_sources += spacenav_sources
    optional_defines += [('HAVE_SPACE_NAVIGATOR', 1)]
    optional_libs    += ['spnav']

if compile_galapix_sdl:
    sdl_env = Environment(CXXFLAGS=['-Wall', '-Werror', '-O2', '-g'],
                          CPPDEFINES = ['GALAPIX_SDL',
                                        ('HAVE_OPENGL', 1)] + optional_defines,
                          LIBS = ['GL', 'GLEW', 'sqlite3', 'jpeg', 'boost_thread'] + optional_libs,
                          OBJPREFIX = "sdl.")
    sdl_env.ParseConfig('pkg-config libpng --libs --cflags')
    sdl_env.ParseConfig('sdl-config --cflags --libs')
    sdl_env.ParseConfig('Magick++-config --libs --cppflags')
    sdl_env.ParseConfig('pkg-config --cflags --libs libcurl')
    sdl_env.Program('galapix.sdl', 
                    galapix_sources + sdl_sources + optional_sources)

if compile_galapix_gtk:
    gtk_env = Environment(CXXFLAGS=['-Wall', '-Werror', '-O2', '-g'],
                          CPPDEFINES = ['GALAPIX_GTK',
                                        ('HAVE_OPENGL', 1)] + optional_defines,
                          LIBS = ['GL', 'GLEW', 'sqlite3', 'jpeg', 'boost_thread'] + optional_libs,
                          OBJPREFIX = "gtk.")
    gtk_env.ParseConfig('pkg-config libpng --libs --cflags')
    gtk_env.ParseConfig('sdl-config --cflags --libs')
    gtk_env.ParseConfig('Magick++-config --libs --cppflags')
    gtk_env.ParseConfig('pkg-config --cflags --libs libcurl')
    gtk_env.ParseConfig('pkg-config --cflags --libs gtkmm-2.4 libglademm-2.4 gtkglextmm-1.2')
    gtk_env.Program('galapix.gtk', 
                    galapix_sources + gtk_sources + optional_sources)

# EOF #
