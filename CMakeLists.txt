cmake_minimum_required(VERSION 3.18)

project(figcone_xml VERSION 1.2.0 DESCRIPTION "rapidxml parser adapter for figcone library")
include(external/seal_lake)

SealLake_IsInstallEnabled(INSTALL_FIGCONE_TREE)
SealLake_Import(
        figcone_tree 2.1.0
        GIT_REPOSITORY https://github.com/kamchatka-volcano/figcone_tree.git
        GIT_TAG        v2.1.0
)
SealLake_Bundle(
        NAME figcone_rapidxml
        SKIP_LOAD
        GIT_REPOSITORY https://github.com/dwd/rapidxml.git
        GIT_TAG v1.0.0
        TEXT_REPLACEMENTS
            "namespace rapidxml" "namespace figcone::xml::rapidxml"
)

SealLake_ObjectLibrary(
        NAMESPACE figcone
        SOURCES
            src/paramparser.cpp
            src/parser.cpp
            src/utils.cpp
        COMPILE_FEATURES cxx_std_17
        INCLUDES
            ${SEAL_LAKE_SOURCE_figcone_rapidxml}
        INTERFACE_LIBRARIES
            figcone::figcone_tree
        DEPENDENCIES
            figcone_tree 2.1.0
)

SealLake_OptionalSubProjects(tests)