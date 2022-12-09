install:
	npm install
	make build-c
run:
	npm run electron:serve
background:
	@-make build-c
	./PhotonDMXHandler
build:
	npm run electron:build
build-c:
	gcc -c ./src/PhotonDMXHandler/artnet/*.c -lws2_32
	mv *.o ./src/PhotonDMXHandler/artnet/
	g++ ./src/PhotonDMXHandler/*.cpp -o PhotonDMXHandler ./src/PhotonDMXHandler/artnet/*.o -I ./include -I ./src/PhotonDMXHandler/artnet -static-libstdc++ -static-libgcc -lws2_32 -liphlpapi -O2
cleaninstall:
	npm ci
	make build-c
storybook:
	npm run storybook
update:
	git pull
	npm ci
	make build-c