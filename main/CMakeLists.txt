# main/CMakeLists.txt

idf_component_register(
    SRCS
        "main.cpp"
        "eink.cpp"
        "buttons.cpp"
    INCLUDE_DIRS
        "."
)