CC = gcc
CCFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=199309L -D_DEFAULT_SOURCE
OBJ = myMonitoringTool.o GraphicDisplay.o MemoryUtilization.o CPUUtilization.o BasicComputation.o SignalHandle.o CoresAndFrequency.o
TARGET = myMonitoringTool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $^

myMonitoringTool2.o: myMonitoringTool.c GraphicDisplay.h MemoryUtilization.h CPUUtilization.h BasicComputation.h SignalHandle.h CoresAndFrequency.h
	$(CC) $(CCFLAGS) -c $<

GraphicDisplay.o: GraphicDisplay.c GraphicDisplay.h
	$(CC) $(CCFLAGS) -c $<

MemoryUtilization.o: MemoryUtilization.c MemoryUtilization.h
	$(CC) $(CCFLAGS) -c $<

CPUUtilization.o: CPUUtilization.c CPUUtilization.h
	$(CC) $(CCFLAGS) -c $<

BasicComputation.o: BasicComputation.c BasicComputation.h
	$(CC) $(CCFLAGS) -c $<

SignalHandle.o: SignalHandle.c SignalHandle.h
	$(CC) $(CCFLAGS) -c $<

CoresAndFrequency.o: CoresAndFrequency.c CoresAndFrequency.h
	$(CC) $(CCFLAGS) -c $<

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: help

help:
	@echo "Make: this will trigger the all rule."
	@echo "all: this will compile any modified object or source file into its respective file."
	@echo "clean: removes object files created during compilation."
	@echo "help: this is the instrutcions for using the makefile."