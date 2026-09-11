idf_component_register(
    SRCS "EPD_2in9.c" "main.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES
        esp_driver_gpio
        esp_driver_spi
)