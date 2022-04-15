include_directories(
	${CMAKE_BINARY_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
	${TDE_INCLUDE_DIR}
	${TQT_INCLUDE_DIRS}
)

link_directories(
	${TQT_LIBRARY_DIRS}
	${TDE_LIB_DIR}
)

tde_add_executable( ${PROJECT_NAME} AUTOMOC
  SOURCES main.cpp easyprojector.cpp bigbutton.cpp biglabel.cpp dialog.cpp
  LINK tdecore-shared tdeui-shared tderandr-shared
  DESTINATION ${BIN_INSTALL_DIR}
)

install( FILES tdeeasyprojector.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

tde_install_icons()
