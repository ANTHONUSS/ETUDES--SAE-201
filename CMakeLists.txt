cmake_minimum_required(VERSION 3.31)
project(SAE201)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# On rajoute le chemin de Qt, sinon il ne trouve pas les fichiers de configuration
set(CMAKE_PREFIX_PATH "C:/Qt/6.9.1/mingw_64/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        PrintSupport # à rajouter si on utilise des composants supplémentaires (dans ce cas là c'est pour Imprimer)
        REQUIRED)

# Rajouter les chemins des dossiers qui contiennent un .ui (pour le compilo, sinon il est po content)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/notepad")

add_executable(SAE201 main.cpp
        notepad/notepad.cpp
        notepad/notepad.h
        notepad/notepad.ui)
target_link_libraries(SAE201
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::PrintSupport # à rajouter si on utilise des composants supplémentaires (dans ce cas là c'est pour Imprimer)
)
# On rajoute le dossier des ressources (images, etc.), sinon il ne les trouve pas
qt_add_resources(RESOURCES resources/resources.qrc) # Fichier .qrc qui contient les ressources
target_sources(SAE201 PRIVATE ${RESOURCES}) # On ajoute les ressources au projet

# ça on y fait pas gaffe, c'est pour le débug
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
