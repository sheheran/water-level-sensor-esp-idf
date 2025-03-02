SHELL := /bin/bash

# Define the init target 
init:
# Create components directory if it doesn't exist
	@mkdir -p components

#Initialize esp-idf
	@echo "Initializing ESP-IDF..."
	@. /home/minindu/Documents/esp/esp-idf/export.sh

# Add git submodules if they don't already exist
	@if [ ! -d "components/Adafruit_NeoPixel" ]; then \
		echo "Submodule add Adafruit_NeoPixel"; \
		git submodule add git@github.com:adafruit/Adafruit_NeoPixel.git components/Adafruit_NeoPixel; \
	else \
		echo "Adafruit_NeoPixel submodule already exists, updating."; \
	fi
	@if [ ! -d "components/AsyncDelay" ]; then \
		echo "Submodule add AsyncDelay"; \
		git submodule add git@github.com:stevemarple/AsyncDelay.git components/AsyncDelay; \
	else \
		echo "AsyncDelay submodule already exists, updating."; \
	fi
	@if [ ! -d "components/JSN-SR04T" ]; then \
		echo "Submodule add JSN-SR04T"; \
		git submodule add git@github.com:hornmich/JSN-SR04T.git components/JSN-SR04T; \
	else \
		echo "JSN-SR04T submodule already exists, updating."; \
	fi
# 	@if [ ! -d "components/arduino-esp32" ]; then \
# 		echo "Submodule add arduino-esp32"; \
# 		git submodule add git@github.com:espressif/arduino-esp32.git components/arduino-esp32; \
# 	else \
# 		echo "arduino-esp32 submodule already exists, updating."; \
# 	fi
# # Re-initialize submodules
# 	@git submodule update --init --recursive

# # Checkout specific branch for arduino-esp32 if it exists
# 	@if [ -d "components/arduino-esp32" ]; then \
# 		cd components/arduino-esp32 && git fetch && git checkout release/v3.1.x && git submodule update --init --recursive; \
# 	fi

# Print and create CMakeLists.txt if the directory exists
	@for dir in Adafruit_NeoPixel AsyncDelay JSN-SR04T; do \
		if [ "$$dir" = "Adafruit_NeoPixel" ] && [ -d "components/Adafruit_NeoPixel" ]; then \
	echo "# CMakeLists.txt inside Adafruit_NeoPixel folder"; \
	echo -e "cmake_minimum_required(VERSION 3.5)\n\n\
	idf_component_register(SRCS \"Adafruit_NeoPixel.cpp\" \"rsp.c\" \n\t\t\t\t\t\
	INCLUDE_DIRS \".\"\n\t\t\t\t\t\
	REQUIRES \"arduino-esp32\")\n\n\
	project(Adafruit_NeoPixel)" > components/Adafruit_NeoPixel/CMakeLists.txt; \
			fi; \
		if [ "$$dir" = "AsyncDelay" ] && [ -d "components/AsyncDelay" ]; then \
	echo "# CMakeLists.txt inside AsyncDelay folder"; \
	echo -e "cmake_minimum_required(VERSION 3.5)\n\n\
	idf_component_register(INCLUDE_DIRS \"src\"\n\t\t\t\t\t\
	REQUIRES \"arduino-esp32\")\n\n\
	project(AsyncDelay)" > components/AsyncDelay/CMakeLists.txt; \
			fi; \
		if [ "$$dir" = "JSN-SR04T" ] && [ -d "components/JSN-SR04T" ]; then \
	echo "# CMakeLists.txt inside JSN-SR04T folder"; \
	echo -e "cmake_minimum_required(VERSION 3.5)\n\n\
	idf_component_register(SRCS \"jsn-sr04t.c\" INCLUDE_DIRS \".\"\n\t\t\t\t\t\
	REQUIRES \"arduino-esp32\")\n\n\
	project(JSN-SR04T)" > components/JSN-SR04T/CMakeLists.txt; \
		fi; \
		done

clean:
#Initialize esp-idf
	@echo "Initializing ESP-IDF..."
	@. /home/minindu/Documents/esp/esp-idf/export.sh

	@if [ -d "build" ]; then \
		echo "Clean build directory.."; \
		rm -rf build/; \
		idf.py fullclean; \
	else \
		echo "No build directory found, skipping clean."; \
	fi


cleanall:
	@echo "Removing submodules..."
	@for dir in Adafruit_NeoPixel AsyncDelay JSN-SR04T arduino-esp32; do \
		if [ -d "components/$$dir/" ]; then \
			echo "Removing submodule $$dir..."; \
			git submodule deinit -f components/$$dir; \
			git rm -f components/$$dir; \
			rm -rf .git/modules/components/$$dir; \
		fi; \
	done
	@rm -rf components
	@echo "All submodules and components directory removed." 