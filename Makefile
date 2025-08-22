BUILD_DIR := build
CMAKE_FLAGS := -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
CMAKE := cmake
MAKE := make

.PHONY: all clean rebuild distclean

all: $(BUILD_DIR)/Makefile
	@$(MAKE) -C $(BUILD_DIR)

$(BUILD_DIR)/Makefile: CMakeLists.txt
	@mkdir -p $(BUILD_DIR)
	@$(CMAKE) -S . -B $(BUILD_DIR) -G "Unix Makefiles" $(CMAKE_FLAGS)

clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		$(MAKE) -C $(BUILD_DIR) clean; \
	fi

distclean:
	@rm -rf $(BUILD_DIR)

rebuild: distclean all
