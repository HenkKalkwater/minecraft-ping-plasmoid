cmake_minimum_required(VERSION 3.10)

project(plasma-minecraftserverping)
 
find_package(ECM 1.4.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set (CMAKE_CXX_STANDARD 14)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(KF5 REQUIRED COMPONENTS
    Plasma
    I18n
)

find_package(Qt5 CONFIG REQUIRED COMPONENTS
	Qml
    Quick
    Core
)

# plasmoid
plasma_install_package(package nl.netsoj.minecraftserverping)

# qml extension plugin
add_subdirectory(minecraftserverpingplugin)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
