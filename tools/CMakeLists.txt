# Collection of tools/utilities to use on games created using this engine

include(FindPythonInterp)
set(TOOLS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "")

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



#! generate_components_register : this function generate header with components registered
#
# This utility helps register the components of a game. The generated header can be included and expose
# "RegisterComponents" method to register the components in the desired game's environment.
#
# \arg:file_name name of the file will be generated
# \arg:generated_file output variable contains the full path of the generated file
# \param:COMPONENTS list of files will be processed by the generator
#
function(generate_components_register file_name generated_file)
    cmake_parse_arguments(
            GEN_COMPONENTS
            ""
            ""
            "COMPONENTS"
            ${ARGN}
    )

    if(NOT DEFINED GEN_COMPONENTS_COMPONENTS)
        message( FATAL_ERROR "The list of components to generate register file is empty" )
    endif()

    set(COMPONENTS_REGISTERED_HPP ${CMAKE_CURRENT_SOURCE_DIR}/${file_name}.hpp)
    file(GLOB COMPONENTS_REGISTERED_HEADERS CONFIGURE_DEPENDS ${GEN_COMPONENTS_COMPONENTS})
    # ensure the COMPONENTS_REGISTERED_HEADERS's dependency list doesn't include file_name
    list(REMOVE_ITEM COMPONENTS_REGISTERED_HEADERS ${COMPONENTS_REGISTERED_HPP})

    add_custom_command(
            COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_SOURCE_DIR}/engine_components_generator.py ${COMPONENTS_REGISTERED_HEADERS} > ${COMPONENTS_REGISTERED_HPP}
            DEPENDS ${COMPONENTS_REGISTERED_HEADERS}
            OUTPUT ${COMPONENTS_REGISTERED_HPP}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating code to register components into the object manager"
    )
    add_custom_target(
            generate_components_register_manual ALL
            DEPENDS ${COMPONENTS_REGISTERED_HPP}
    )
    set(${generated_file} ${COMPONENTS_REGISTERED_HPP} PARENT_SCOPE)
endfunction()


function(generate_data_register file_name generated_file)

    set(DATA_REGISTERED_HPP ${CMAKE_CURRENT_SOURCE_DIR}/${file_name}.hpp)
    file(GLOB_RECURSE DATAOBJECTS_REGISTERED "*.scene" "*.prototype")

    add_custom_command(
            COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_SOURCE_DIR}/data_register_generator.py > ${DATA_REGISTERED_HPP}
            DEPENDS ${DATAOBJECTS_REGISTERED}
            OUTPUT ${DATA_REGISTERED_HPP}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Generating code to register data into the object manager"
    )
    add_custom_target(
            generate_data_register_manual ALL
            DEPENDS ${DATA_REGISTERED_HPP}
    )
    set(${generated_file} ${DATA_REGISTERED_HPP} PARENT_SCOPE)
endfunction()
