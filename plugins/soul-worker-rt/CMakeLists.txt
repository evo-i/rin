cmake_minimum_required(VERSION 3.16)

add_library(soul_worker_rt SHARED
  soul_worker_rt_plugin.c
  soul_worker_rt_private.c
  prefixed_string.c
  md5.c
  achievement.c
  booster.c
  buff.c
  item.c
  cutscene.c
  cinema.c
  table.c
  title.c
  tooltip.c
  ui.c
  league_skill.c
  monster.c
  npc.c
  quest.c
  shop.c
  skill.c
  soul_metry.c
  speech.c
  system_mail.c
)

target_include_directories(soul_worker_rt
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(soul_worker_rt PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/soul-worker-rt
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins/soul-worker-rt
)

target_link_libraries(soul_worker_rt PRIVATE
  advapi32
)

add_executable(test_c test-c.c)

target_link_libraries(test_c PRIVATE soul_worker_rt
  advapi32
)
