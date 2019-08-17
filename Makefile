S_DIR=src
B_DIR=build
JAVA_DIR=test/java

CC_OPTS=-c -pipe -Wno-switch -Wall -ggdb -g3 -std=c++11 -O3 -g
LN_OPTS=-g
CC=g++
OS = $(shell uname)

JAVAC=javac

SRCS = $(shell find $(S_DIR) -name *.cpp)
HEADERS = $(shell find $(SRC_DIR) -name *.h)
JAVA_SRCS = $(shell find $(JAVA_DIR) -name *.java)

OBJS = $(SRCS:$(S_DIR)/%.cpp=$(B_DIR)/obj/%.o)
DEPS = $(SRCS:$(S_DIR)/%.cpp=$(B_DIR)/obj/%.d)
JAVA_CLASSES = $(JAVA_SRCS:$(JAVA_DIR)/%.java=$(B_DIR)/%.class)

ifeq ($(OS), Linux)
	MAIN_EXEC = $(B_DIR)/jacey
else
	MAIN_EXEC = $(B_DIR)/jacey.exe
endif

.PHONY: default
default: $(MAIN_EXEC)
.PHONY: compile-all
compile-all: $(MAIN_EXEC) $(JAVA_CLASSES)
clean:
	rm -r build
	mkdir -p $(B_DIR)

test-integration: $(MAIN_EXEC) $(JAVA_CLASSES)
	cd $(B_DIR)
	$(MAIN_EXEC) build.com.jacey.sample.sample0

#
# C++ BUILD COMMANDS
#
$(MAIN_EXEC): $(DEPS) $(OBJS)
	$(CC) $(LN_OPTS) -o $@ $(OBJS)

$(B_DIR)/obj/%.o : $(S_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CC_OPTS) -o $@ $<

$(B_DIR)/obj/%.d: $(S_DIR)/%.cpp
	mkdir -p $(dir $@)
	printf $(dir $@) > $@
	$(CC) $(CC_OPTS) -MM -MG $< >> $@ 

#
# JAVA BUILD COMMANDS
#
$(B_DIR)/%.class : $(JAVA_DIR)/%.java
	$(JAVAC) -d $(B_DIR) $<

-include $(SRCS:%.cpp=$(B_DIR)/obj/%.d)