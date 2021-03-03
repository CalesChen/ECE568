all: main
main: main.cpp helper.cpp helper.h proxy.cpp proxy.h request_info.cpp request_info.h response.cpp response.h cache.cpp cache.h
		g++ -g -o main main.cpp helper.cpp helper.h proxy.cpp proxy.h request_info.cpp request_info.h response.cpp response.h cache.cpp cache.h -lpthread
.PHONY:
		cliean
clean:
		rm -rf *.o main