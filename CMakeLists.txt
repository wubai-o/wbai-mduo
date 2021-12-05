cmake_minimum_required(VERSION 2.8)

project(study)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror  -Wno-unused-function")

set(LIB_SRC
    study/signal_slot.cc
    study/eventloop.cc
    study/Channel.cc
    study/Poller.cc
    study/base/CurrentThread.cc
    study/base/Thread.cc
    study/poller/PollPoller.cc
    study/poller/DefaultPoller.cc
    study/base/Timestamp.cc
    study/base/CountDownLatch.cc
    study/EventLoopThread.cc
    study/Timer.cc
    study/TimerQueue.cc
    study/EventLoopThread.cc
    study/Socket.cc
    study/SocketsOps.cc
    study/InetAddress.cc
    study/Acceptor.cc
    study/base/Date.cc
    study/Connector.cc
    study/TcpClient.cc
    study/TcpConnection.cc
    study/Buffer.cc
    study/base/LogStream.cc
    study/base/Logging.cc
    study/base/FileUtil.cc
    study/base/ProcessInfo.cc
    study/base/LogFile.cc
)

set(LIB_LIB
    study
    pthread
    )

add_executable(signal_slot_test tests/signal_slot_test.cc)
add_dependencies(signal_slot_test study)
target_link_libraries(signal_slot_test ${LIB_LIB})

add_executable(test_timestamp tests/test_timestamp.cc)
add_dependencies(test_timestamp study)
target_link_libraries(test_timestamp ${LIB_LIB})

add_executable(test_log tests/test_log.cc)
add_dependencies(test_log study)
target_link_libraries(test_log ${LIB_LIB})

add_executable(test_date tests/test_date.cc)
add_dependencies(test_date study)
target_link_libraries(test_date ${LIB_LIB})

add_executable(test3 tests/test3.cc)
add_dependencies(test3 study)
target_link_libraries(test3 ${LIB_LIB})

add_executable(test4 tests/test4.cc)
add_dependencies(test4 study)
target_link_libraries(test4 ${LIB_LIB})

add_executable(test6 tests/test6.cc)
add_dependencies(test6 study)
target_link_libraries(test6 ${LIB_LIB})

add_executable(test7 tests/test7.cc)
add_dependencies(test7 study)
target_link_libraries(test7 ${LIB_LIB})

add_executable(test_fwrite_unlocked tests/test_fwrite_unlocked.cc)
add_dependencies(test_fwrite_unlocked study)
target_link_libraries(test_fwrite_unlocked ${LIB_LIB})

add_library(study SHARED ${LIB_SRC})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib) 
