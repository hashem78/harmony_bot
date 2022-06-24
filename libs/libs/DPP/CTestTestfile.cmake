# CMake generated Testfile for 
# Source directory: C:/development/cpp/harmony/libs/DPP
# Build directory: C:/development/cpp/harmony/libs/libs/DPP
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(unittests "C:/development/cpp/harmony/libs/libs/DPP/Debug/unittest.exe")
  set_tests_properties(unittests PROPERTIES  _BACKTRACE_TRIPLES "C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;295;add_test;C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(unittests "C:/development/cpp/harmony/libs/libs/DPP/Release/unittest.exe")
  set_tests_properties(unittests PROPERTIES  _BACKTRACE_TRIPLES "C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;295;add_test;C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(unittests "C:/development/cpp/harmony/libs/libs/DPP/MinSizeRel/unittest.exe")
  set_tests_properties(unittests PROPERTIES  _BACKTRACE_TRIPLES "C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;295;add_test;C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(unittests "C:/development/cpp/harmony/libs/libs/DPP/RelWithDebInfo/unittest.exe")
  set_tests_properties(unittests PROPERTIES  _BACKTRACE_TRIPLES "C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;295;add_test;C:/development/cpp/harmony/libs/DPP/CMakeLists.txt;0;")
else()
  add_test(unittests NOT_AVAILABLE)
endif()
