cmake_minimum_required(VERSION 3.24)
project(TowerDef)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Environments/qt/5.15.2/mingw81_64")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(TowerDef
        src/main.cpp
        src/GameControl.cpp src/GameControl.h
        src/AbstractGrid.cpp src/AbstractGrid.h
        src/GroundGrid.cpp src/GroundGrid.h
        res.qrc
        src/AirGrid.cpp src/AirGrid.h src/GateGrid.cpp src/GateGrid.h src/AbstractEnemy.cpp src/AbstractEnemy.h src/AbstractFriendObjects.cpp src/AbstractFriendObjects.h src/DefenderFriend.cpp src/DefenderFriend.h src/ShooterFriend.cpp src/ShooterFriend.h src/VanguardFriend.cpp src/VanguardFriend.h src/HealerFriend.cpp src/HealerFriend.h src/SplashCasterFriend.cpp src/SplashCasterFriend.h src/Pointer.cpp src/Pointer.h src/Bullet.cpp src/Bullet.h src/ScoreBoard.cpp src/ScoreBoard.h src/Tick.cpp src/Tick.h src/Cross.cpp src/Cross.h src/CostBoard.cpp src/CostBoard.h)
target_link_libraries(TowerDef
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

if (WIN32)
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
                "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
