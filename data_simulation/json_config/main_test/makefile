#目标（要生成的文件名）
TARGET:= main
#编译器
CC:= g++
#编译包含的头文件所在目录 
INCLUDES:=-I ../cereal-master/include/ -I ..
#所有用到的源文件，注意：非当前目录的要+上详细地址
SRCS:=$(wildcard ./*.cpp)
#将SRC变量中所有以.c结尾的文件名替换成对应的以.o结尾的文件名，然后赋回给SRC。
OBJS:=$(patsubst %.cpp, %.o, $(SRCS))
#头文件
Head:=$(wildcard ../*.h )
#库
LIB:= -L .. -ldev_json

ALL:$(TARGET)
	rm -f *.o 

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LIB) 

#$(OBJS): $(SRCS)
#$(CC) -c $^ $(INCLUDES)

$(OBJS):%.o:%.cpp 
	$(CC) -c $< -o $@ $(INCLUDES)

#config_main: config_main.cpp
#	g++ config_main.cpp -o config_main -I /home/quan/Desktop/cereal_test/cereal-master/include