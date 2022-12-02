tigr.dll  : tigrLib.c pknative.c tigr\tigr.c 
	gcc -Os -s -fPIC -shared -o tigr.dll tigrLib.c pknative.c tigr\tigr.c -Itigr -I. -lopengl32 -lgdi32

