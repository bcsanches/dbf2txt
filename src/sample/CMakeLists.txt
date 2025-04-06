
add_executable(sample
	main.cpp
)

target_include_directories(sample 
	PRIVATE ${dbf2txt_SOURCE_DIR}/src/dbf2txt    
)

target_link_libraries(sample
	PRIVATE dbf2txt	
)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    
#set_property(TARGET sample PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set_property(TARGET sample PROPERTY VS_DEBUGGER_COMMAND_ARGUMENTS "..\\..\\..\\sampleData\\dbase_83.dbf text.txt")


