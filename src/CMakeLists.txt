add_executable(parser
    main.cpp
    Config.cpp
    FileReader.cpp
    Parser.cpp
)

target_include_directories(parser PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/third_party
)