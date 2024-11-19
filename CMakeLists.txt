cmake_minimum_required(VERSION 3.10)
project(SDLPlayerProject)

# Find SDL2 and its components
find_package(SDL2 QUIET)
find_package(SDL2_mixer QUIET)
find_package(SDL2_image QUIET)
find_package(SDL2_ttf QUIET)

# Check if packages were found
set(MISSING_LIBS "")
if(NOT SDL2_FOUND)
    if (APPLE)
        list(APPEND MISSING_LIBS "SDL2")
    elseif(UNIX AND NOT APPLE)
        list(APPEND MISSING_LIBS "libsdl2-dev")
    endif()
endif()
if(NOT SDL2_mixer_FOUND)
    if (APPLE)
        list(APPEND MISSING_LIBS "SDL2_mixer")
    elseif(UNIX AND NOT APPLE)
        list(APPEND MISSING_LIBS "libsdl2-mixer-dev")
    endif()
endif()
if(NOT SDL2_image_FOUND)
    if (APPLE)
        list(APPEND MISSING_LIBS "SDL2_image")
    elseif(UNIX AND NOT APPLE)
        list(APPEND MISSING_LIBS "libsdl2-image-dev")
    endif()
endif()
if(NOT SDL2_ttf_FOUND)
    if (APPLE)
        list(APPEND MISSING_LIBS "SDL2_ttf")
    elseif(UNIX AND NOT APPLE)
        list(APPEND MISSING_LIBS "libsdl2-ttf-dev")
    endif()
endif()

if(MISSING_LIBS)
    message(STATUS "Te biblioteki są potrzebne: ${MISSING_LIBS}")
    string(REPLACE ";" " " MISSING_PACKAGES_STR "${MISSING_LIBS}")
    
    if(APPLE)
        message(STATUS "Jezeli posiadasz Homebrew, zainstaluj biblioteki za pomocą:")
        message(STATUS "brew install ${MISSING_PACKAGES_STR}")
    elseif(UNIX AND NOT APPLE)
        message(STATUS "Aby zainstalować brakujące biblioteki, wpisz w terminalu:")
        message(STATUS "sudo apt-get install ${MISSING_PACKAGES_STR}")
        message(STATUS "Uwaga. W zalezności od systemu zamiast apt moze występowac inny manager pakietow:")
        message(STATUS "Polecenie moze wyglądać jako: sudo apt-get install | pacman -S --noconfirm | dnf install -y")
    endif()

    message(FATAL_ERROR "Missing required libraries. Please install them manually.")
endif()

# Add executable
add_executable(${PROJECT_NAME} 
    main.c
    src/controls.c
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
)

# Platform-specific configurations
if(APPLE)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum macOS deployment version")
    include_directories(
        /opt/homebrew/include
        /opt/homebrew/include/SDL2
        /usr/local/include
        /usr/local/include/SDL2
    )
    
    link_directories(
        /opt/homebrew/lib
        /usr/local/lib
    )

        # Get SDL2 flags and libs using sdl2-config
    execute_process(
            COMMAND sdl2-config --cflags
            OUTPUT_VARIABLE SDL_CFLAGS
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
        
    execute_process(
            COMMAND sdl2-config --libs
            OUTPUT_VARIABLE SDL_LIBS
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
        
        # Add the flags to compilation
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SDL_CFLAGS}")
        
        # Link libraries (SDL_LIBS from sdl2-config plus additional libraries)
    target_link_libraries(${PROJECT_NAME}
            ${SDL_LIBS}
            SDL2_image
            SDL2_mixer
            SDL2_ttf
    )

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Creating symlink..."
        COMMAND ln -sf 
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}
    )

elseif(UNIX AND NOT APPLE) 
        execute_process(
            COMMAND sdl2-config --cflags
            OUTPUT_VARIABLE SDL_CFLAGS
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SDL_CFLAGS}")
    
    # Link all required libraries
        target_link_libraries(${PROJECT_NAME}
            SDL2
            SDL2_image
            SDL2_mixer
            SDL2_ttf
        )

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Creating symlink..."
        COMMAND ln -sf 
        ${PROJECT_BINARY_DIR}/${PROJECT_NAME}
        ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}
        )
else()
    message(FATAL_ERROR "Nie mozna skompilowac na tej platformie :(")
endif()