set(RIN_SOURCES
  rin-registry.c
  rin-registry-entry.c
)

set(RIN_HEADERS
  rin-registry.h
  rin-registry-entry.h
  rin-plugin-api.h
)

# Определяем тип сборки
if(WIN32 AND NOT BUILD_SHARED_LIBS)
    add_library(rin STATIC ${RIN_SOURCES} ${RIN_HEADERS})
else()
    add_library(rin SHARED ${RIN_SOURCES} ${RIN_HEADERS})
endif()

target_include_directories(rin
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE ${GLIB_INCLUDE_DIRS}
)

target_link_libraries(rin
  PRIVATE ${GLIB_LIBRARIES}
)

target_compile_options(rin PRIVATE ${GLIB_CFLAGS_OTHER})

# Настройки для статической линковки под Windows
if(WIN32 AND NOT BUILD_SHARED_LIBS)
    target_compile_definitions(rin PRIVATE GOBJECT_STATIC_COMPILATION)
    target_compile_definitions(rin PRIVATE GMODULE_STATIC_COMPILATION)
    target_compile_definitions(rin PRIVATE GIO_STATIC_COMPILATION)

    # Дополнительные библиотеки для статической линковки
    if(MSVC)
        target_link_libraries(rin PRIVATE
            ws2_32
            ole32
            shell32
            advapi32
            user32
            kernel32
        )
    else()
        target_link_libraries(rin PRIVATE
            ws2_32
            ole32
            shell32
            advapi32
            user32
            kernel32
            iconv
            intl
            pcre
            ffi
            z
        )
    endif()
endif()
