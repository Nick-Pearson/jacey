
S_DIR=src
B_DIR=build
JAVA_DIR=test/java

CC_OPTS=-c -pipe -Wno-switch -ggdb -g3 -std=c++11 -O3 -g
LN_OPTS=-g
CC=g++
OS = $(shell uname)

JAVAC=javac

SRCS = $(shell find $(S_DIR) -name *.cpp)
OBJS = $(SRCS:$(S_DIR)/%.cpp=$(B_DIR)/obj/%.o)
JAVA_SRCS = $(shell find $(JAVA_DIR) -name *.java)
JAVA_CLASSES = $(JAVA_SRCS:$(JAVA_DIR)/%.java=$(B_DIR)/%.class)

MAIN_EXEC = $(B_DIR)/jacey

default: $(MAIN_EXEC)
compile: $(MAIN_EXEC) $(JAVA_CLASSES)
clean:
	rm -r build
	mkdir -p $(B_DIR)

test: $(MAIN_EXEC) $(JAVA_CLASSES)
	cd $(B_DIR)
	$(MAIN_EXEC) com.jacey.sample.sample0

#
# C++ BUILD COMMANDS
#
$(MAIN_EXEC): $(OBJS)
	$(CC) $(LN_OPTS) -o $@ $(OBJS)

$(B_DIR)/obj/%.o : $(S_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CC_OPTS) -o $@ $<

#
# JAVA BUILD COMMANDS
#
$(B_DIR)/%.class : $(JAVA_DIR)/%.java
	$(JAVAC) -d $(B_DIR) $<
