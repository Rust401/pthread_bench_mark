#Uncommit the next line to build the intermidiate file
#all: test.cpp i s att o ll

TARGET=main.c
LEVEL=-O3
COMPILER= clang-12

all: $(TARGET)
	$(COMPILER) -std=c11 -o test $(TARGET) $(LEVEL) -lpthread
	time ./test

i:$(TARGET)
	 $(COMPILER) -E $(TARGET) -o test.i $(LEVEL)

ll:$(TARGET)
	 $(COMPILER) -S -emit-llvm $(TARGET) $(LEVEL)

s:$(TARGET)
	 $(COMPILER) -S -mllvm --x86-asm-syntax=intel $(TARGET) $(LEVEL)

att:$(TARGET)
	 $(COMPILER) -S $(TARGET) -o test_att.s $(LEVEL)

o:$(TARGET)
	 $(COMPILER) -c $(TARGET) -o test.o $(LEVEL)

clean:
	rm -f peda* .gdb* *.i *.ll *.s *.o
