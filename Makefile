# Output files
BUILD_DIR  = build

# Mocks files
MOCKS_DIR  = ./
MOCKS_SRCS = $(wildcard $(MOCKS_DIR)/*.c)
MOCKS_DEPS = $(addprefix $(BUILD_DIR)/,$(notdir $(MOCKS_SRCS:.c=.d)))
MOCKS_OBJS = $(MOCKS_DEPS:.d=.o)

# Unity files
UNITY_DIR  = ../unity
UNITY_SRCS = $(wildcard $(UNITY_DIR)/*.c)
UNITY_DEPS = $(addprefix $(BUILD_DIR)/,$(notdir $(UNITY_SRCS:.c=.d)))
UNITY_OBJS = $(UNITY_DEPS:.d=.o)
UNITY_HEAP_FLAGS =                              \
	UNITY_EXCLUDE_STDLIB_MALLOC             \
	UNITY_INTERNAL_HEAP_SIZE_BYTES=256
UNITY_DEFS = $(addprefix -D,$(UNITY_HEAP_FLAGS))

# Test files
TEST_DIR  = test
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_DEPS = $(addprefix $(BUILD_DIR)/,$(notdir $(TEST_SRCS:.c=.d)))
TEST_OBJS = $(TEST_DEPS:.d=.o) $(UNITY_OBJS)
TEST_DEFS = $(UNITY_DEFS)

# Include folders
INCLUDE_DIRS =                          \
	$(MOCKS_DIR)                    \
	$(UNITY_DIR)
INCLUDE      =  $(addprefix -I,$(INCLUDE_DIRS))

# Options
CC         = gcc
CFLAGS     = -fPIC -Wall -O0 -g


# Dependencies
-include $(MOCKS_DEPS)
-include $(UNITY_DEPS)
-include $(TEST_DEPS)

# Rules
.PHONY: clean mocks build_test test

mocks: $(MOCKS_OBJS)

build_test: $(TEST_OBJS)

test: build_test
	@echo
	@echo Linking test_mocks:
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_mocks \
		$(TEST_OBJS)
	@echo
	@echo Run test_mocks:
	@$(BUILD_DIR)/test_mocks

$(BUILD_DIR)/%.o: $(MOCKS_DIR)/%.c
	@echo
	@echo Compiling $(notdir $<):
	@mkdir -p $(dir $@)
	$(CC) -MMD -MP $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(UNITY_DIR)/%.c
	@echo
	@echo Compiling $(notdir $<):
	@mkdir -p $(dir $@)
	$(CC) -MMD -MP $(CFLAGS) $(UNITY_DEFS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@echo
	@echo Compiling $(notdir $<):
	@mkdir -p $(dir $@)
	$(CC) -MMD -MP $(CFLAGS) $(INCLUDE) $(TEST_DEFS) -c -o $@ $<

clean:
	@echo
	@echo Removing folder: $(BUILD_DIR)
	@rm -rf $(BUILD_DIR)
