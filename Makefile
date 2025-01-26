# Define the init target 
init:
	# Create components directory if it doesn't exist
	@mkdir -p components

	# Add git submodules if they don't already exist
	@if [ ! -d "components/Adafruit_NeoPixel" ]; then \
		echo "Submodule add Adafruit_NeoPixel"; \
		git submodule add git@github.com:adafruit/Adafruit_NeoPixel.git components/Adafruit_NeoPixel; \
	fi
	@if [ ! -d "components/AsyncDelay" ]; then \
		echo "Submodule add AsyncDelay"; \
		git submodule add git@github.com:stevemarple/AsyncDelay.git components/AsyncDelay; \
	fi
	@if [ ! -d "components/JSN-SR04T" ]; then \
		echo "Submodule add JSN-SR04T"; \
		git submodule add git@github.com:hornmich/JSN-SR04T.git components/JSN-SR04T; \
	fi
	@if [ ! -d "components/arduino-esp32" ]; then \
		echo "Submodule add arduino-esp32"; \
		git submodule add git@github.com:espressif/arduino-esp32.git components/arduino-esp32; \
	fi

	# Checkout specific branch for arduino-esp32 if it exists
	@if [-d "components/arduino-esp32"]; then \
		cd components/arduino-esp32 && git fetch && git checkout idf-release/v5.1; \
	fi

	# Print # CMake configuration if the directory exists
	@for dir in Adafruit_NeoPixel AsyncDelay JSN-SR04T; do \
		if [ "$$dir" = "Adafruit_NeoPixel" ] && [ -d "components/Adafruit_NeoPixel" ]; then \
			echo "# CMakeFiles.txt inside "Adafruit_NeoPixel" folder"; \
			echo -e "# CMakeFiles.txt inside "Adafruit_NeoPixel" folder\n cmake_minimum_required(VERSION 3.5)\n" > components/Adafruit_NeoPixel/CMakeLists.txt; \
			echo -e "idf_component_register(SRCS \"Adafruit_NeoPixel.cpp\" \"esp.c\" \n\t\t\t\t\t INCLUDE_DIRS \".\"\n\t\t\t\t\t REQUIRES \"arduino-esp32\" # Library requires Arduino\n\t\t\t\t\t )\n\n" >> components/Adafruit_NeoPixel/CMakeLists.txt; \
			echo -e "project(Adafruit_NeoPixel) \n" >> components/Adafruit_NeoPixel/CMakeLists.txt; \
		fi; \
		if [ "$$dir" = "AsyncDelay" ] && [ -d "components/AsyncDelay" ]; then \
			echo "# CMakeFiles.txt inside "Adafruit_NeoPixel" folder"; \
			echo -e "# CMakeFiles.txt inside "Adafruit_NeoPixel" folder\n cmake_minimum_required(VERSION 3.5)\n" > components/AsyncDelay/CMakeLists.txt; \
			echo -e "idf_component_register(INCLUDE_DIRS \"src\"\n\t\t\t\t\t REQUIRES \"arduino-esp32\" # Library requires Arduino\n\t\t\t\t\t )\n\n" >> components/AsyncDelay/CMakeLists.txt; \
			echo -e "project(Adafruit_NeoPixel) \n" >> components/AsyncDelay/CMakeLists.txt; \
		fi; \
		if [ "$$dir" = "JSN-SR04T" ] && [ -d "components/JSN-SR04T" ]; then \
			echo "# CMakeFiles.txt inside "JSN-SR04T" folder"; \
			echo -e "# CMakeFiles.txt inside "JSN-SR04T" folder\n cmake_minimum_required(VERSION 3.5)\n" > components/JSN-SR04T/CMakeLists.txt; \
			echo -e "idf_component_register(SRCS \"jsn-sr04t.c\" \n\t\t\t\t\t INCLUDE_DIRS \".\"\n\t\t\t\t\t REQUIRES \"arduino-esp32\" # Library requires Arduino\n\t\t\t\t\t )\n\n" >> components/JSN-SR04T/CMakeLists.txt; \
			echo -e "project(JSN-SR04T) \n" >> components/JSN-SR04T/CMakeLists.txt; \
		fi; \
	done
