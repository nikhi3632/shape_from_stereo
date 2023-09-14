COMPILER = gcc
BINARIES = depth_map quadtree
CFLAGS = -Wall -Werror -g -std=c99
CUNIT_LIB_PATH ?= /opt/homebrew/Cellar/cunit/2.1-3/lib
CUNIT_INCLUDE_PATH ?= /opt/homebrew/Cellar/cunit/2.1-3/include

.PHONY: clean all run-unit-tests

default: all

all: ${BINARIES}

depth_map: calc_depth.c depth_map.c utils.c
	${COMPILER} ${CFLAGS} -o $@ $^ -lm

quadtree: quadtree.c make_qtree.c utils.c
	${COMPILER} ${CFLAGS} -o $@ $^ -lm

unit-tests: calc_depth.c make_qtree.c test/cunit/unit-tests.c
	${COMPILER} ${CFLAGS} -o $@ test/cunit/unit-tests.c -lm -L${CUNIT_LIB_PATH} -I${CUNIT_INCLUDE_PATH} -lcunit

run-unit-tests: unit-tests
	./$^ | tee unit_tests.log

check: ${BINARIES}
	python3 check.py | tee check_results.log

check-dmap: ${BINARIES}
	python3 check.py dmap 2>&1 | tee dmap.log

check-qtree: ${BINARIES}
	python3 check.py qtree 2>&1 | tee qtree.log

tidy:
	rm -rf ${BINARIES} unit-tests *.dSYM unit_tests.log check_results.log dmap.log qtree.log 

run:
	./run.sh 2>&1 | tee run.log

clean:
	rm -rf ${BINARIES} test/output/* unit-tests *.dSYM *.log 
