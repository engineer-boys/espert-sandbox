ESPER := espert-core/build/libesper.a
ESPER_HEADERS := espert-core/src
BUILD := build

ESPERT_EXTERNALS := espert-core/externals

.PHONY: clean

EXTERNALS_SPDLOG_HEADER := ${ESPERT_EXTERNALS}/spdlog/include
EXTERNALS_GLFW3_HEADER := ${ESPERT_EXTERNALS}/glfw/include
EXTERNALS_GLFW3_LIB := ${ESPERT_EXTERNALS}/glfw/build/src/libglfw3.a

${BUILD}/sandbox: ${BUILD}/main.o
	g++ $< ${ESPER} -o $@ -fsanitize=address ${EXTERNALS_GLFW3_LIB}

${BUILD}/main.o: main.cc
	g++ -c $< -o $@ -I ${ESPER_HEADERS} -I ${EXTERNALS_SPDLOG_HEADER} -I ${EXTERNALS_GLFW3_HEADER}

clean:
	rm -rf build/*

