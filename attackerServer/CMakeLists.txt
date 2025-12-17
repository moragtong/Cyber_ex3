cmake_minimum_required(VERSION 4.1)
project(attackerServer C)

set(CMAKE_C_STANDARD 11)

add_executable(attackerServer
        ex2_client.c
        ex2_server.c)
