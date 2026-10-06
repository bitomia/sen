# === CMakeLists.txt ===================================================================================================
#                                               Sen Infrastructure
#                   Released under the Apache License v2.0 (SPDX-License-Identifier Apache-2.0).
#                                    See the LICENSE.txt file for more information.
#                   © Airbus SAS, Airbus Helicopters, and Airbus Defence and Space SAU/GmbH/SAS.
# ======================================================================================================================

set(_archive_work_dir ${CMAKE_CURRENT_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${_archive_work_dir})

add_sen_integration_test(
  cli_archive_setup
  COMMAND
  cli_run
  ${CMAKE_CURRENT_SOURCE_DIR}/data/archive_record.yaml
  WORKING_DIRECTORY ${_archive_work_dir}
  REQ_DEPS cli_run aircrafts
)

append_test_env_modification(cli_archive_setup "PYTHONPATH=path_list_prepend:${CMAKE_CURRENT_SOURCE_DIR}/data")

add_sen_integration_test(
  cli_archive_cleanup
  COMMAND
  ${CMAKE_COMMAND}
  -E
  rm
  -rf
  ${_archive_work_dir}/archive_recording
  WORKING_DIRECTORY ${_archive_work_dir}
  REQ_DEPS cli_archive
)

add_sen_integration_test(
  cli_archive_info
  COMMAND
  ./cli_archive
  info
  ${_archive_work_dir}/archive_recording
  WORKING_DIRECTORY $<TARGET_FILE_DIR:sen::cli_archive>
  REQ_DEPS cli_archive
)

add_sen_integration_test(
  cli_archive_indexed
  COMMAND
  ./cli_archive
  indexed
  ${_archive_work_dir}/archive_recording
  WORKING_DIRECTORY $<TARGET_FILE_DIR:sen::cli_archive>
  REQ_DEPS cli_archive
)

set_tests_properties(cli_archive_setup PROPERTIES FIXTURES_SETUP cli_archive_fixture)
set_tests_properties(cli_archive_cleanup PROPERTIES FIXTURES_CLEANUP cli_archive_fixture)

set_tests_properties(cli_archive_info PROPERTIES FIXTURES_REQUIRED cli_archive_fixture)
set_tests_properties(cli_archive_indexed PROPERTIES FIXTURES_REQUIRED cli_archive_fixture)
