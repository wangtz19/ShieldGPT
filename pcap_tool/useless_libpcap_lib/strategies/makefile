INCLUDE = -I ../common

SRC = $(wildcard *.cpp)
OBJS = $(addprefix $(ROOT_DIR)/$(OBJS_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SRC))))
DEPS = $(addprefix $(ROOT_DIR)/$(DEPS_DIR)/,$(patsubst %.cpp,%.d,$(notdir $(SRC))))

vpath %.h ../common

all: $(OBJS)
	echo $^ > objs.txt
$(OBJS):$(ROOT_DIR)/$(OBJS_DIR)/%.o:%.cpp $(ROOT_DIR)/$(DEPS_DIR)/%.d
	$(CC) -c $< -o $@ $(INCLUDE)
$(DEPS):$(ROOT_DIR)/$(DEPS_DIR)/%.d:%.cpp
	touch $@
	echo -n $@ " " > $@
	$(CC) -MM $< >> $@ $(INCLUDE)
-include $(DEPS)