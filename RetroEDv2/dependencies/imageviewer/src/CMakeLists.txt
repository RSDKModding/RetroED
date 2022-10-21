add_library(pal-image-viewer
    image-viewer.cpp
    image-viewer.hpp
    image-viewer.qrc
)

target_include_directories(pal-image-viewer
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:pal>
)

pal_set_lib_properties(pal-image-viewer)

target_link_libraries(pal-image-viewer
    PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets
)

add_library(Pal::ImageViewer ALIAS pal-image-viewer)

