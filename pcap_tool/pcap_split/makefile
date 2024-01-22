CC=g++
ROOT_DIR=$(shell pwd)
OBJS_DIR=objs
DEPS_DIR=deps
USELESS_LIBPCAP_LIB_DIR=../useless_libpcap_lib
USELESS_LIBPCAP_LIB_DIRS=$(patsubst $(USELESS_LIBPCAP_LIB_DIR),,$(shell find $(USELESS_LIBPCAP_LIB_DIR) -maxdepth 1 -type d))
USELESS_LIBPCAP_LIB_OBJS=$(foreach dir,$(USELESS_LIBPCAP_LIB_DIRS),$(shell if [ -f $(dir)/objs.txt ]; then cat $(dir)/objs.txt; fi))

LIBPCAPFLAGS = -lpcap -fopenmp -lboost_program_options -lstdc++fs
CC_FLAG = -std=c++17
INCLUDE=$(addprefix -I ,$(USELESS_LIBPCAP_LIB_DIRS))

SRC=$(wildcard *.cpp)
OBJS = $(addprefix $(OBJS_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SRC))))
DEPS = $(addprefix $(DEPS_DIR)/,$(patsubst %.cpp,%.d,$(notdir $(SRC))))
TARGET=main

$(shell if [ ! -d $(OBJS_DIR) ]; then mkdir -p $(OBJS_DIR);fi)
$(shell if [ ! -d $(DEPS_DIR) ]; then mkdir -p $(DEPS_DIR);fi)

export CC ROOT_DIR OBJS_DIR DEPS_DIR

$(TARGET): $(USELESS_LIBPCAP_LIB_DIRS) $(OBJS) $(USELESS_LIBPCAP_LIB_OBJS)
	$(CC) $(CC_FLAG) -o $@ $(OBJS_DIR)/*.o $(INCLUDE) $(LIBPCAPFLAGS)
$(OBJS):$(OBJS_DIR)/%.o:%.cpp $(DEPS_DIR)/%.d
	$(CC) $(CC_FLAG) -c $< -o $@ $(INCLUDE) $(LIBPCAPFLAGS)
$(DEPS):$(DEPS_DIR)/%.d:%.cpp
	touch $@
	echo -n $@ " " > $@
	$(CC) -MM $< >> $@ $(INCLUDE)
$(USELESS_LIBPCAP_LIB_DIRS): ECHO
	make -C $@
ECHO:
	@echo $(USELESS_LIBPCAP_LIB_DIRS)

.PHONY: clean
clean:
	-rm -r $(OBJS_DIR) $(DEPS_DIR) $(TARGET)

-include $(DEPS)