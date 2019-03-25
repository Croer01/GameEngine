# Collection of tools/utilities to use on games created using this engine

#! configure_game_directories : this function configure game folders
#
# This utility create automatically the targets necessary to copy the game
# folders into the bin directory
#
# \arg:game_target name of the cmake target used to define game
# \arg:data_folder relative folder to SOURCE_ROOT param contains game data
# \arg:conf_folder relative folder to SOURCE_ROOT param contains game configuration
# \arg:assets_folder relative folder to SOURCE_ROOT param contains game assets
# \param:SOURCE_ROOT the source root of the game source. By default use the CMAKE_CURRENT_SOURCE_DIR
# \param:BIN_ROOT the source root of the game binary output. By default use the CMAKE_CURRENT_BINARY_DIR
#
function(configure_game_directories game_target data_folder conf_folder assets_folder)
    cmake_parse_arguments(
            CONFIG_GAME
            ""
            "SOURCE_ROOT;BIN_ROOT"
            ""
            ${ARGN}
    )

    if(NOT DEFINED CONFIG_GAME_SOURCE_ROOT)
        set(CONFIG_GAME_SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
    if(NOT DEFINED CONFIG_GAME_BIN_ROOT)
        set(CONFIG_GAME_BIN_ROOT ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    ADD_DEPENDENCIES(${game_target}
            "${game_target}_copy_data"
            "${game_target}_copy_conf"
            "${game_target}_copy_assets"
            )
    message(SOURCE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
    ADD_CUSTOM_TARGET("${game_target}_copy_data"
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CONFIG_GAME_BIN_ROOT}/${data_folder}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_GAME_SOURCE_ROOT}/${data_folder} ${CONFIG_GAME_BIN_ROOT}/${data_folder}
            )
    ADD_CUSTOM_TARGET("${game_target}_copy_conf"
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CONFIG_GAME_BIN_ROOT}/${conf_folder}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_GAME_SOURCE_ROOT}/${conf_folder} ${CONFIG_GAME_BIN_ROOT}/${conf_folder}
            )
    ADD_CUSTOM_TARGET("${game_target}_copy_assets"
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CONFIG_GAME_BIN_ROOT}/${assets_folder}
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_GAME_SOURCE_ROOT}/${assets_folder} ${CONFIG_GAME_BIN_ROOT}/${assets_folder}
            )
endfunction()