cmake_minimum_required(VERSION 3.18)

project(figcone_xml VERSION 0.11.0 DESCRIPTION "rapidxml parser adapter for figcone library")
include(GNUInstallDirs)
include(external/seal_lake)

set(INSTALL_FIGCONE_TREE ON)
SealLake_FindOrDownload(
        figcone_tree 0.11.0
        GIT_REPOSITORY https://github.com/kamchatka-volcano/figcone_tree.git
        GIT_TAG        cmake_maintenance
)
SealLake_FindOrDownload(
        rapidxml 1.13
        GIT_REPOSITORY https://github.com/dwd/rapidxml.git
        GIT_TAG master
)

SealLake_HeaderOnlyLibrary(
        NAMESPACE figcone
        COMPILE_FEATURES cxx_std_17
        INCLUDES figcone_xml/detail/external
        BUILD_STAGE_INCLUDES ${rapidxml_SOURCE_DIR}
        LIBRARIES figcone::figcone_tree
)

SealLake_OptionalBuildSteps(
        IF_ENABLED_AND_STANDALONE tests
)

SealLake_Install(
        PATH figcone_xml/detail/external
        FILES ${rapidxml_SOURCE_DIR}/rapidxml.hpp
)