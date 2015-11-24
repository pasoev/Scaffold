cc = "gcc"
ccflags = '-g'
env = Environment(CC = cc, CCFLAGS = ccflags)



libs = Split('SDL2 SDL2_image SDL2_gfx SDL2_ttf m xml2')
libpath = ['/usr/lib', '/usr/local/lib']
cpppath = ['/usr/include/libxml2']

src_files = Split('list.c yxml.c parselvl.c fsm.c geometry.c sprite.c ledge.c enemy.c player.c text.c texture.c scaffold.c menustate.c playstate.c pausestate.c gameoverstate.c')
env.Program(source = src_files, target = 'scaffold', LIBS=libs,
LIBPATH=libpath, CPPPATH=cpppath)
