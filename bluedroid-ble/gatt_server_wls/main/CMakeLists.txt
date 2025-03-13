idf_component_register(
                        SRCS "gatt_server_wls.c"    # Source file to compile
                        INCLUDE_DIRS "." #"$ENV{IDF_PATH}/components/bt/include/esp32c3/include" "$ENV{IDF_PATH}/components/bt/host/bluedroid/api/include/api"   # Include current directory for headers
                        REQUIRES nvs_flash bt  # Dependencies
                    )
