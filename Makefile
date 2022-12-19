ifeq ($(OS),Windows_NT)
	MESSAGE = "Building for Windows..."
	CC_ARGS = -lws2_32
	CPP_ARGS = -static-libstdc++ -static-libgcc -lws2_32 -liphlpapi -O2
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		MESSAGE = "Linux support coming soon! In the meantime, if you figure it out, go ahead and contribute and submit a pull request!"
	endif
	ifeq ($(UNAME_S),Darwin)
		MESSAGE = "Building Photon for MacOS..."
		CC_ARGS = 
		CPP_ARGS = -lc++ -std=c++17 -O2
	endif
endif
.PHONY: install-deps
install-deps:
	mkdir include
	@-cd ./include && git clone https://github.com/coluris/photon-deps.git
	make build-c
.PHONY: build-c
build-c:
	@-echo $(MESSAGE)
	cd ./include/photon-deps/artnet && gcc -c *.c $(CC_ARGS)
	g++ ./src/PhotonDMXHandler/*.cpp -o PhotonDMXHandler ./include/photon-deps/artnet/*.o -I ./include/photon-deps $(CPP_ARGS)