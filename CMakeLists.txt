cmake_minimum_required(VERSION 3.18)

project(figcone_xml VERSION 0.11.0 DESCRIPTION "rapidxml parser adapter for figcone library")
include(GNUInstallDirs)
include(external/seal_lake)

set(INSTALL_FIGCONE_TREE ON)
SealLake_Import(
        figcone_tree 0.11.0
        GIT_REPOSITORY https://github.com/kamchatka-volcano/figcone_tree.git
        GIT_TAG        cmake_maintenance
)
SealLake_Download(
        rapidxml 1.13
        GIT_REPOSITORY https://github.com/dwd/rapidxml.git
        GIT_TAG master
        DESTINATION include/figcone_xml/detail/external
        FILES rapidxml.hpp
)

SealLake_HeaderOnlyLibrary(
        NAMESPACE figcone
        COMPILE_FEATURES cxx_std_17
        LIBRARIES figcone::figcone_tree
        DEPENDENCIES figcone_tree 0.11.0
)

SealLake_OptionalBuildSteps(tests)