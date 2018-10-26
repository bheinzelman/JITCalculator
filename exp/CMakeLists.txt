cmake_minimum_required(VERSION 3.0)
    project(jc)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
            file(GLOB Src
                "*.cpp"
                "*.h"
                "*.hpp")

                find_library(READLINE_LIB readline) if (NOT READLINE_LIB)
                    find_library(READLINE_LIB edit)
                        endif()

                            add_executable(main $ { Src })
                                target_link_libraries(main $ { READLINE_LIB })

                                    add_custom_target(runit)
                                        add_custom_command(TARGET runit
                                                               POST_BUILD
                                                                   COMMAND $ { CMAKE_BINARY_DIR }
                                            / exp / main)
