.DEFAULT_HOAD := default
default:
	@echo "welcome to bverbose Makefile."
	@echo "*** ONLY DEBIAN IS SUPPORTED (woah) ***"
	@echo "targets are"
	@echo "	make prepare	installs dependencies - requires root"
	@echo "	make install	compiles some useful binaries."
	@echo "no target specified, exiting."

prepare:
	apt install libuuid1 uuid-dev -y

install:
	mkdir -p bin
	gcc -luuid -I lib -I src test/tape-test.c -o bin/bvr-compose-single
	gcc test/compose-all-in-dir.c -I lib -I src -o bin/bvr-compose-html -luuid
	gcc test/jsbundle-test.c -I lib -I src -luuid -o bin/bvr-jsbundle
	gcc test/jsmin-test.c -I lib -I src -luuid -o bin/bvr-jsmin
	@echo "install target done, now you should add the bin directory to your PATH"
