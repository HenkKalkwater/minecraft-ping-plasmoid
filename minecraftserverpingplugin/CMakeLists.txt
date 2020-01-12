add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_nl.netsoj.minecraftserverping\")
set(minecraftserverping_SRCS
    minecraftserverping.cpp
    minecraftserver.cpp
    serverlistmodel.cpp
)

add_library(minecraftserverpingplugin SHARED ${minecraftserverping_SRCS})

target_link_libraries(minecraftserverpingplugin
                      KF5::I18n
                      Qt5::Core
                      Qt5::Qml
                      Qt5::Gui)

install(TARGETS minecraftserverpingplugin DESTINATION ${KDE_INSTALL_QMLDIR}/nl/netsoj/minecraftserverping)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/nl/netsoj/minecraftserverping)
