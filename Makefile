
PATH_BIN  = ./bin
PATH_DATA = ./data

.PHONY: all
all:
	@echo "\n*** Compiling cmeans ***"
	@echo "***"
	mkdir -p $(PATH_BIN)
	g++ $(DEFLAGS) -g3 -W -I include/ -std=c++14 src/main.cpp src/vector2.cpp -o $(PATH_BIN)/cmeans
	@echo "***"

.PHONY: test
test:
	make all DEFLAGS="-D __RANDOM__"
	mkdir -p $(PATH_DATA)
	$(PATH_BIN)/cmeans 20 6 -50 +50 | python3 plot.py -s $(PATH_DATA)/tiny.png
	$(PATH_BIN)/cmeans 50 13 -50 +50 | python3 plot.py -s $(PATH_DATA)/small.png
	$(PATH_BIN)/cmeans 130 27 -50 +50 | python3 plot.py -s $(PATH_DATA)/medium.png
	$(PATH_BIN)/cmeans 250 53 -50 +50 | python3 plot.py -s $(PATH_DATA)/large.png
	$(PATH_BIN)/cmeans 500 109 -50 +50 | python3 plot.py -s $(PATH_DATA)/huge.png

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
	find . -name "*.png" -delete
	@echo "***"
