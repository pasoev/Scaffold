import sys

platform = sys.platform

w = ' -Wall -Wextra -Wstrict-prototypes -Wshadow -Wwrite-strings -Wold-style-definition -Wredundant-decls -Wnested-externs -Wmissing-include-dirs -Wstrict-aliasing'
cc = "gcc"

libs = Split('SDL2 SDL2_image SDL2_ttf m')

ccflags = '-g'
if platform == 'win32':
   ccflags += " -mwindows"
   libs.insert(0, 'SDL2main')
   libs.insert(0, 'mingw32')
env = Environment(CC = cc, CCFLAGS = ccflags, tools=['mingw'])





libpath = ['/usr/lib', '/usr/local/lib', 'C:\SDL2\SDL2\i686-w64-mingw32\lib', 'C:\SDL2\SDL2_image-2.0.0\i686-w64-mingw32\lib', 'C:\SDL2\SDL2_ttf-2.0.12\i686-w64-mingw32\lib']
cpppath = ['C:\SDL2\SDL2\i686-w64-mingw32\include', 'C:\SDL2\SDL2_image-2.0.0\i686-w64-mingw32\include', 'C:\SDL2\SDL2_ttf-2.0.12\i686-w64-mingw32\include', 'C:\SDL2\SDL2\i686-w64-mingw32\include\SDL2']

src_files = Split('list.c yxml.c parselvl.c fsm.c geometry.c sprite.c ledge.c enemy.c player.c text.c texture.c scaffold.c menustate.c playstate.c pausestate.c gameoverstate.c')
env.Program(source = src_files, target = 'scaffold', LIBS=libs,
LIBPATH=libpath, CPPPATH=cpppath)
