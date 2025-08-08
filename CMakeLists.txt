cmake_minimum_required(VERSION 3.16)
project(SistemaMercado)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Procurar Qt6 de forma opcional
find_package(Qt6 QUIET COMPONENTS Core Widgets Sql)

if(Qt6_FOUND)
    message(STATUS "Qt6 encontrado")
    qt_standard_project_setup()
else()
    message(WARNING "Qt6 não encontrado - construindo versão simplificada")
    add_definitions(-DNO_QT)
endif()

# Diretórios de inclusão
include_directories(${CMAKE_SOURCE_DIR}/include)

# Lista básica de arquivos fonte (versão simplificada)
set(SOURCES_SIMPLE
    src/simple_main.cpp
    src/models/produto_simple.cpp
)

# Arquivos de cabeçalho
set(HEADERS_SIMPLE
    include/models/produto_simple.h
)

if(Qt6_FOUND)
    # Arquivo de recursos Qt
    qt_add_resources(RESOURCE_FILES resources/resources.qrc)
    
    # Versão completa com GUI
    set(SOURCES_GUI
        src/main.cpp
        src/gui/main_window.cpp
    )
    
    set(HEADERS_GUI
        include/gui/main_window.h
    )
    
    # Criar executável completo
    qt_add_executable(SistemaMercado 
        ${SOURCES_CORE} 
        ${SOURCES_GUI} 
        ${HEADERS_CORE} 
        ${HEADERS_GUI}
        ${RESOURCE_FILES}
    )
    
    # Linkar bibliotecas Qt
    target_link_libraries(SistemaMercado 
        Qt6::Core 
        Qt6::Widgets 
        Qt6::Sql
    )
    
    # Procurar SQLite3
    find_library(SQLITE3_LIBRARY sqlite3)
    if(SQLITE3_LIBRARY)
        target_link_libraries(SistemaMercado ${SQLITE3_LIBRARY})
    endif()
    
else()
    # Versão simplificada - apenas console
    add_executable(SistemaMercado 
        ${SOURCES_SIMPLE} 
        ${HEADERS_SIMPLE}
    )
endif()

# Instalar executável
install(TARGETS SistemaMercado DESTINATION bin)
