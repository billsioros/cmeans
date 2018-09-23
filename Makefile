
PATH_BIN = ./bin

.PHONY: all
all:
	@echo "\n*** Compiling cmeans ***"
	@echo "***"
	mkdir -p ./bin
	g++ $(DEFLAGS) -g3 -W -I include/ -std=c++14 src/main.cpp src/vector2.cpp -o $(PATH_BIN)/cmeans
	@echo "***"

.PHONY: test
test:
	make all DEFLAGS="-D __RANDOM__"
	./bin/cmeans 20 6 0 50 > ./test/tiny.dat
	./bin/cmeans 50 13 0 50 > ./test/small.dat
	./bin/cmeans 130 27 0 50 > ./test/medium.dat
	./bin/cmeans 250 53 0 50 > ./test/large.dat
	./bin/cmeans 500 109 0 50 > ./test/huge.dat

.PHONY: debug
debug:
	make all DEFLAGS="-D __DEBUG_CMEANS__"

.PHONY: clean
clean:
	@echo "\n*** Purging cmeans ***"
	@echo "***"
	rm -rIv $(PATH_BIN)
	@echo "***"

.PHONY: discard
discard:
	@echo "\n*** Removing test files ***"
	@echo "***"
	find . -name "*.dat" -delete
	@echo "***"
