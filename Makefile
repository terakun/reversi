CXX = g++
# CXX = icpc
FLAGS = -std=c++11 -O2 -march=native
INCLUDES = 
LIBS = 
TARGET = main
SRCS = main.cc reversi.cc reversi_ai.cc
OBJS = $(patsubst %.cc,%.o,$(SRCS))

.cc.o:
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJS)
		$(CXX) $(FLAGS) $(FLAG) -o $@ $(OBJS) $(LIBS)

clean:
	rm $(TARGET) $(OBJS)
