cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

set(vst3sdk_SOURCE_DIR "${VST3_SDK_ROOT}")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(DCOffsetter
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "DCOffsetter VST 3 Plug-in"
)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(DCOffsetter
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
)

#- VSTGUI Wanted ----
if(SMTG_ADD_VSTGUI)
    target_sources(DCOffsetter
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(DCOffsetter
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(DCOffsetter
        RESOURCES
            resource/myplugineditor.uidesc
            resource/logo.png
            resource/slider_handle.png
            resource/slider_rail.png
            resource/slider_scale.png
            resource/toggle_small.png
    )
endif(SMTG_ADD_VSTGUI)
# -------------------

smtg_target_add_plugin_snapshots (DCOffsetter
    RESOURCES
        resource/54B41A6759565BB18EEEE67E16FEA83D_snapshot.png
        resource/54B41A6759565BB18EEEE67E16FEA83D_snapshot_2.0x.png
)

target_link_libraries(DCOffsetter
    PRIVATE
        sdk
)

smtg_target_configure_version_file(DCOffsetter)

if(SMTG_MAC)
    smtg_target_set_bundle(DCOffsetter
        BUNDLE_IDENTIFIER com.jonas.dcoffsetter
        COMPANY_NAME "Jonas M"
    )
    smtg_target_set_debug_executable(DCOffsetter
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(DCOffsetter PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT DCOffsetter)

        smtg_target_set_debug_executable(DCOffsetter
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
