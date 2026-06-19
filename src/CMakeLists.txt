add_executable(kmdview
    main.cpp
    mainwindow.cpp
    mainwindow.h
    documenttheme.cpp
    documenttheme.h
)

target_link_libraries(kmdview PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

install(TARGETS kmdview ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
