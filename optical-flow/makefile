all:
	g++-4.9 of.cpp main.cpp -o optical_flow `pkg-config opencv --cflags --libs` -L ~/ffmpeg_build/lib -lavcodec -std=c++11

clean:
	rm *.png
