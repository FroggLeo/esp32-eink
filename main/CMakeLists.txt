idf_component_register(
    SRCS "ssd1608.cpp" "main.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES
        esp_driver_gpio
        esp_driver_spi
)