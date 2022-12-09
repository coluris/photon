ifeq ($(OS),Windows_NT)
	MESSAGE = "Building for Windows..."
	CC_ARGS = -lws2_32
	CPP_ARGS = "-static-libstdc++ -static-libgcc -lws2_32 -liphlpapi -O2"
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		MESSAGE = "Linux support coming soon! In the meantime, if you figure it out, go ahead and contribute and submit a pull request!"
	endif
	ifeq ($(UNAME_S),Darwin)
		MESSAGE = "Building Photon for MacOS..."
		CC_ARGS = 
		CPP_ARGS = -lc++ -O2
	endif
endif
.PHONY: install
install:
	npm install
	make build-c
.PHONY: run
run:
	npm run electron:serve
.PHONY: background
background:
	@-make build-c
	./PhotonDMXHandler
.PHONY: build
build:
	npm run electron:build
.PHONY: build-c
build-c:
	echo $(MESSAGE)
	gcc -c ./src/PhotonDMXHandler/artnet/*.c $(CC_ARGS)
	mv *.o ./src/PhotonDMXHandler/artnet/
	g++ ./src/PhotonDMXHandler/*.cpp -o PhotonDMXHandler ./src/PhotonDMXHandler/artnet/*.o -I ./include -I ./src/PhotonDMXHandler/artnet -std=c++17 $(CPP_ARGS)
.PHONY: cleaninstall
cleaninstall:
	npm ci
	make build-c
.PHONY: storybook
storybook:
	npm run storybook
.PHONY: update
update:
	git pull
	npm ci
	make build-c