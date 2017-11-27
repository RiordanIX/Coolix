CXXFLAGS := -Wall -Werror -Wextra -pthread

BIN := coolix

src_files := $(wildcard src/*.cpp)
obj_files := $(patsubst src/%.cpp,obj/%.o,$(src_files))

.PHONY: all clean remake debug redebug

all: CXXFLAGS += -O2
all: $(BIN)

clean:
	rm -f $(BIN) obj/*.o

debug: CXXFLAGS += -DDEBUG -g -O0 -fno-inline
debug: $(BIN)

remake: clean all

redebug: clean debug

$(BIN): $(obj_files)
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/%.o: src/%.cpp src/*.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $(filter src/%.cpp,$^)

