cmake_minimum_required(VERSION 3.12)
project(HardwareSimulationFixture C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Enable the CTest testing framework infrastructure
enable_testing()

# 1. The Listener Target (Simulates your 3rd-party blocking software)
add_executable(test_listener listener_app.c)

# 2. The Injector Target (Simulates the test suite firing the HW interrupt)
add_executable(test_injector injector_app.c)

# Set platform macros
if(VXWORKS)
    target_compile_definitions(test_listener PRIVATE TARGET_OS_VXWORKS)
    target_compile_definitions(test_injector PRIVATE TARGET_OS_VXWORKS)
else()
    target_compile_definitions(test_listener PRIVATE TARGET_OS_LINUX)
    target_compile_definitions(test_injector PRIVATE TARGET_OS_LINUX)
    target_link_libraries(test_listener PRIVATE rt)
    target_link_libraries(test_injector PRIVATE rt)
endif()

# -------------------------------------------------------------------
# Define CTest Test Cases
# -------------------------------------------------------------------

# Test Case A: Verify the Node can initialize and clean up properly
add_test(NAME Test_HW_Node_Lifecycle COMMAND test_injector --setup-only)

# Test Case B: The integration test loop
# CTest executes the listener app, which expects a mock interrupt 
# vector (e.g., 0xDEADBEEF) within a 5-second timeout window.
add_test(NAME Test_Interrupt_Blocking_Read COMMAND test_listener)
set_tests_properties(Test_Interrupt_Blocking_Read PROPERTIES TIMEOUT 5)
