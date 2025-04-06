
add_library(
    dbf2txt
    DbfFile.cpp
	DbfFile.h
)

target_link_libraries(dbf2txt)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

#target_compile_definitions(dbf2txt)

