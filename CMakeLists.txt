cmake_minimum_required(VERSION 3.18)

project(figcone_xml VERSION 0.9.0 DESCRIPTION "rapidxml parser adapter for figcone library")

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(FIGCONE_XML_SUBPROJECT OFF)
else()
    set(FIGCONE_XML_SUBPROJECT ON)
endif()

include(external/xml)

find_package(figcone_tree 0.9.0 QUIET)
if (NOT figcone_tree_FOUND)
    message("Configuration info: figcone_tree library wasn't found on your system, downloading it from the GitHub repository")
    include (external/figcone_tree)
endif()

add_library(figcone_xml INTERFACE)
add_library(figcone::figcone_xml ALIAS figcone_xml)

target_compile_features(figcone_xml INTERFACE cxx_std_17)
target_include_directories(
        figcone_xml
        INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(figcone_xml INTERFACE figcone::figcone_tree rapidxml)

option(ENABLE_TESTS "Enable tests" OFF)
if (ENABLE_TESTS AND NOT FIGCONE_XML_SUBPROJECT)
    enable_testing()
    add_subdirectory(tests)
endif()

option(INSTALL_FIGCONE_XML "Install figcone_xml library unconditionally" OFF)
if (NOT FIGCONE_XML_SUBPROJECT OR INSTALL_FIGCONE_XML)
    include(GNUInstallDirs)
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/figcone_xml DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

    set(FIGCONE_XML_PACK_PATH ${CMAKE_INSTALL_LIBDIR}/cmake/figcone_xml)
    install(TARGETS figcone_xml
            EXPORT figcone_xml-targets
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/figcone_xml)
    install(EXPORT figcone_xml-targets
            FILE figcone_xmlTargets.cmake
            NAMESPACE figcone::
            DESTINATION ${FIGCONE_XML_PACK_PATH})

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_xmlConfigVersion.cmake
            COMPATIBILITY SameMajorVersion
            ARCH_INDEPENDENT
    )

    configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/figcone_xmlConfig.cmake.in
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_xmlConfig.cmake
            INSTALL_DESTINATION ${FIGCONE_XML_PACK_PATH}
    )

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_xmlConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/figcone_xmlConfigVersion.cmake
            DESTINATION ${FIGCONE_XML_PACK_PATH}
    )
endif()
