add_metasprite_asset(SOURCE "metasprites.nss" TARGET "metasprites.cpp" HEADER "metasprites.hpp" BANK 1 NAMESPACE "UI")

add_library(SourceObj
  OBJECT

  config.s

  main.cpp
  global.cpp
  gameplay.cpp
  title.cpp
  score.cpp
  creature.cpp
  lens.cpp
  subrand.s

  assets-loader.cpp
  attributes.cpp
  input.cpp
  ggsound.cpp
  log.cpp
  assets.s
  banked-metasprites.s
  soundtrack.hpp

  ${CMAKE_CURRENT_BINARY_DIR}/metasprites.cpp
)

set_property(
  SOURCE
  assets.s
  PROPERTY
  OBJECT_DEPENDS

  bg.chr
  spr.chr

  title.nam
  screen.nam
  score.nam

  bg.pal
  spr.pal
)

add_raw_asset(SOURCE "bg.chr")
add_raw_asset(SOURCE "spr.chr")

add_raw_asset(SOURCE "title.nam")
add_raw_asset(SOURCE "screen.nam")
add_raw_asset(SOURCE "score.nam")

add_raw_asset(SOURCE "bg.pal")
add_raw_asset(SOURCE "spr.pal")

set_property(
  SOURCE
  soundtrack.s
  PROPERTY
  OBJECT_DEPENDS
  ${CMAKE_CURRENT_BINARY_DIR}/soundtrack.asm
  # ${CMAKE_CURRENT_BINARY_DIR}/soundtrack_dpcm.asm
  ${CMAKE_CURRENT_SOURCE_DIR}/ggsound.inc
)

set_target_properties(SourceObj PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(
  SourceObj
  PRIVATE
  ${CMAKE_BINARY_DIR}/assets
  ${CMAKE_BINARY_DIR}/generated
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
)
