cmake_minimum_required(VERSION 3.16)
project(SistemaMercado)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Procurar Qt6 ou Qt5 de forma opcional
find_package(Qt6 QUIET COMPONENTS Core Widgets Sql)
if(NOT Qt6_FOUND)
    find_package(Qt5 QUIET COMPONENTS Core Widgets Sql)
    if(Qt5_FOUND)
        set(QT_VERSION_MAJOR 5)
        message(STATUS "Qt5 encontrado")
    endif()
else()
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Qt6 encontrado")
endif()

if(Qt6_FOUND OR Qt5_FOUND)
    message(STATUS "Qt${QT_VERSION_MAJOR} encontrado")
    if(QT_VERSION_MAJOR EQUAL 6)
        qt_standard_project_setup()
    endif()
else()
    message(WARNING "Qt não encontrado - construindo versão simplificada")
    add_definitions(-DNO_QT)
endif()

# Diretórios de inclusão
include_directories(${CMAKE_SOURCE_DIR}/include)

# Lista básica de arquivos fonte (versão simplificada)
set(SOURCES_SIMPLE
    src/simple_main.cpp
    src/models/produto_simple.cpp
)

# Lista arquivos híbridos (funciona com e sem Qt)
set(SOURCES_HYBRID
    src/hybrid_main.cpp
    src/models/produto_simple.cpp
)

# Arquivos de cabeçalho
set(HEADERS_SIMPLE
    include/models/produto_simple.h
)

if(Qt6_FOUND OR Qt5_FOUND)
    # Arquivo de recursos Qt
    if(QT_VERSION_MAJOR EQUAL 6)
        qt_add_resources(RESOURCE_FILES resources/resources.qrc)
    else()
        qt5_add_resources(RESOURCE_FILES resources/resources.qrc)
    endif()
    
    # Versão completa com GUI
    set(SOURCES_GUI
        src/main.cpp
        src/gui/main_window.cpp
    )
    
    set(HEADERS_GUI
        include/gui/main_window.h
    )
    
    # Criar executável completo
    if(QT_VERSION_MAJOR EQUAL 6)
        qt_add_executable(SistemaMercado 
            ${SOURCES_CORE} 
            ${SOURCES_GUI} 
            ${HEADERS_CORE} 
            ${HEADERS_GUI}
            ${RESOURCE_FILES}
        )
    else()
        add_executable(SistemaMercado 
            ${SOURCES_CORE} 
            ${SOURCES_GUI} 
            ${HEADERS_CORE} 
            ${HEADERS_GUI}
            ${RESOURCE_FILES}
        )
    endif()
    
    # Linkar bibliotecas Qt
    if(QT_VERSION_MAJOR EQUAL 6)
        target_link_libraries(SistemaMercado 
            Qt6::Core 
            Qt6::Widgets 
            Qt6::Sql
        )
    else()
        target_link_libraries(SistemaMercado 
            Qt5::Core 
            Qt5::Widgets 
            Qt5::Sql
        )
    endif()
    
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
    
    # Versão híbrida (recomendada)
    add_executable(SistemaMercadoHybrid 
        ${SOURCES_HYBRID} 
        ${HEADERS_SIMPLE}
    )
endif()

# Instalar executável
install(TARGETS SistemaMercado DESTINATION bin)
