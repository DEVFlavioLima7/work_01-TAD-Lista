install_headers('uiMain.h')

ui_main = static_library(
  'uiMain',
  'uiMain.c',
  include_directories: [includes_ui, includes_img_trat, includes_historico],
  dependencies: dep_gtk,
  install: true,
)