ESPER := ../Esper/build/libesper.a
ESPER_HEADERS := ../Esper/src
BUILD := build

EXTERNALS := externals

.PHONY: clean

EXTERNALS_SPDLOG_HEADER := ../Esper/${EXTERNALS}/spdlog/include
EXTERNALS_GLFW3_HEADER := ../Esper/${EXTERNALS}/glfw/include
EXTERNALS_GLFW3_LIB := ../Esper/${EXTERNALS}/glfw/build/src/libglfw3.a

${BUILD}/sandbox: ${BUILD}/main.o
	g++ $< ${ESPER} -o $@ -fsanitize=address ${EXTERNALS_GLFW3_LIB}

${BUILD}/main.o: main.cc
	g++ -c $< -o $@ -I ${ESPER_HEADERS} -I ${EXTERNALS_SPDLOG_HEADER} -I ${EXTERNALS_GLFW3_HEADER}

clean:
	rm -rf build/*

