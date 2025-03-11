idf_component_register(
                    SRCS "gatts_table_creat_demo.c" # Source file to compile
                    INCLUDE_DIRS "."    # Include current directory for headers
                    PRIV_REQUIRES nvs_flash bt  # Dependencies
                    )