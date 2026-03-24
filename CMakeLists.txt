cmake_minimum_required(VERSION 4.3)
project(test C)

find_package(raylib REQUIRED)

add_executable(test main.c
        player.c
        bullet.c
        asteroids.c
        collision.c
        particle.c
)
target_link_libraries(test raylib m)