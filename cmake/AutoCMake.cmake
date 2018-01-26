
set( AUTOCMAKE_ROOT ${CMAKE_CURRENT_LIST_DIR})
enable_testing()

include (CMakeParseArguments)

macro( TRACE msg )
  if ( $ENV{AUTOCMAKE_TRACE} MATCHES "ON" )
      MESSAGE( ${msg} )
  endif()
endmacro()

IF(MSVC)
	ADD_DEFINITIONS( -D_CRT_SECURE_NO_DEPRECATE  )
	ADD_DEFINITIONS( -D_CRT_NONSTDC_NO_DEPRECATE )
	ADD_DEFINITIONS( -D_SCL_SECURE_NO_WARNINGS   )
	
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")
ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	
ENDIF()


if(MSVC)
	option(MSVC_STATIC_RUNTIME
		"Link all libraries and executables with the C run-time DLL (msvcr*.dll) 
		instead of the static C run-time library (libcmt*.lib.) 
		The default is to use the C run-time DLL only with the 
		libraries and executables that need it."
		OFF)
	if(MSVC_STATIC_RUNTIME)
		# Use the static C library for all build types
		foreach(var 
			CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
			CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
			CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO
		)
		if(${var} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${var} "${${var}}")
		endif()
		endforeach()
	endif()
endif()


macro( autocmake_msvc_project_group source_files sgbd_cur_dir)
    if(MSVC)
        foreach(sgbd_file ${${source_files}})

            string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})
			string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
            string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)
            string(REPLACE "/" "\\" sgbd_group_name ${sgbd_group_name})
			
            if(sgbd_nogroup)
                set(sgbd_group_name "\\")
            endif(sgbd_nogroup)
			
            source_group(${sgbd_group_name} FILES ${sgbd_file})
			
        endforeach(sgbd_file)
    endif()
endmacro()

macro( autocmake_default_set _variable _defalut _value)
   SET( M "dset variable:${_variable} _defalut:${_defalut} value:${_value}<${${_value}}>" )
   
   if(  ${_value} )
       SET(M "+${M}")
       set( ${_variable} ${${_value}} )
   else()
       set( ${_variable} ${_defalut}  )
	   SET(M "-[${_variable}:${${_variable}}@${_defalut}]${M}")
   endif()
   #MESSAGE("${M} => ${${_variable}}")
endmacro()

macro( autocmake_install_pc       )

  
	CMAKE_PARSE_ARGUMENTS( _this ""
		   "VERSION;NAME;DESCRIPTION;PREFIX;DESTINATION;INCLUDEDIR;SHAREDLIBDIR;EXECDIR;LIBDIR;BINDIR"
		   "LIBS;CFLAGS;REQUIRES"
		   ${ARGN} )
    #MESSAGE( "+_this_EXECDIR : ${_this_EXECDIR}" )
	autocmake_default_set( _this_NAME         "${PROJECT_NAME}"         _this_NAME        )
	autocmake_default_set( _this_VERSION      "${VERSION}"              _this_VERSION     )
	autocmake_default_set( _this_PREFIX       "${CMAKE_INSTALL_PREFIX}" _this_PREFIX      )
	autocmake_default_set( _this_DESTINATION  "lib/pkgconfig"           _this_DESTINATION )
	autocmake_default_set( _this_DESCRIPTION  ""                        _this_DESCRIPTION )
	autocmake_default_set( _this_LIBS         ${_this_NAME}             _this_LIBS        )
	autocmake_default_set( _this_CFLAGS       ""                        _this_CFLAGS      )
	autocmake_default_set( _this_INCLUDEDIR   "include"                 _this_INCLUDEDIR  )
	autocmake_default_set( _this_LIBDIR       "lib"                     _this_LIBDIR      )
	autocmake_default_set( _this_EXECDIR      "bin"                     _this_EXECDIR     )
	autocmake_default_set( _this_SHAREDLIBDIR "lib"                     _this_SHAREDLIBDIR)
	#MESSAGE( "-_this_EXECDIR : ${_this_EXECDIR}" )

	set(_libs "")
	foreach( _lib ${_this_LIBS} )
	  set( _libs "-l${_lib} ${_libs}")
	endforeach()
	set(_requires)
	foreach( _req ${_this_REQUIRES} )
	  if ( _requires )
		  set( _requires "${_requires}, ${_req}")
	  else()
		  set( _requires "${_req}")
	  endif()
	endforeach()

	#MESSAGE("******${_this_NAME} : @${CMAKE_CURRENT_BINARY_DIR}/${_this_NAME}.pc ")
	FILE( WRITE ${CMAKE_CURRENT_BINARY_DIR}/${_this_NAME}.pc 
			"prefix=${_this_PREFIX}\n"
			"exec_prefix=\${prefix}/${_this_EXECDIR}\n"
			"libdir=\${prefix}/${_this_LIBDIR}\n"
			"sharedlibdir=\${prefix}/${_this_SHAREDLIBDIR}\n"
			"includedir=\${prefix}/${_this_INCLUDEDIR}\n"
			"\n"
			"Name: ${_this_NAME}\n"
			"Description: ${_this_DESCRIPTION}\n"
			"Version: ${_this_VERSION}\n"
			"Requires: ${_requires}\n"
			"\n"
			"Requires:\n"
			"Libs: -L\${libdir} -L\${sharedlibdir} ${_libs}\n"
			"Cflags: -I\${includedir} ${_this_CFLAGS}\n"  )
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${_this_NAME}.pc DESTINATION "${_this_DESTINATION}")
endmacro()

macro( autocmake_pkgconfig_init )
     

	find_package(PkgConfig 0.29.1 REQUIRED)
    if( "$ENV{MSYSTEM}" STREQUAL "msys" )
		foreach ( _var $ENV{PKG_CONFIG_LIBDIR} $ENV{PKG_CONFIG_PATH} )
			
			execute_process(COMMAND cygpath --unix ${_var} 
			OUTPUT_VARIABLE _upath 
			OUTPUT_STRIP_TRAILING_WHITESPACE)
			if( NOT _PKG_CONFIG_PATH )
				SET( _PKG_CONFIG_PATH ${_upath})
			else()
				SET( _PKG_CONFIG_PATH ${_upath}:${_PKG_CONFIG_PATH} )
			endif()
		endforeach()
		SET(ENV{PKG_CONFIG_PATH} ${_PKG_CONFIG_PATH})
	endif()
       
endmacro()

macro( autocmake_check_modules _prefix )
    TRACE("+autocmake_check_modules ${_prefix}")
    set( ${_prefix}_LIBS)
	set( ${_prefix}_LINK_DIRECTORIES)
	set( ${_prefix}_INCLUDE_DIRECTORIES)

    foreach( _pkg ${ARGN} )
		pkg_check_modules( ${_pkg} ${_pkg} REQUIRED)
		TRACE("   ${_pkg}_CFLAGS : ${${_pkg}_CFLAGS}")
		TRACE("   ${_pkg}_INCLUDEDIR : ${${_pkg}_INCLUDEDIR}")
		TRACE("   ${_pkg}_INCLUDE_DIRS : ${${_pkg}_INCLUDE_DIRS}")
		TRACE("   ${_pkg}_LIBRARY_DIRS : ${${_pkg}_LIBRARY_DIRS}")
		list(APPEND ${_prefix}_LINK_DIRECTORIES ${${_pkg}_LIBRARY_DIRS} )
		list(APPEND ${_prefix}_INCLUDE_DIRECTORIES ${${_pkg}_INCLUDE_DIRS} )
		list(APPEND ${_prefix}_INCLUDE_DIRECTORIES ${${_pkg}_INCLUDEDIR} )
		list(APPEND ${_prefix}_LIBS ${${_pkg}_LIBRARIES})

	endforeach()
	list(REMOVE_DUPLICATES ${_prefix}_LINK_DIRECTORIES)
	list(REMOVE_DUPLICATES ${_prefix}_INCLUDE_DIRECTORIES)
	list(REMOVE_DUPLICATES ${_prefix}_LIBS)
	foreach(cat LINK_DIRECTORIES;INCLUDE_DIRECTORIES;LIBS)
	   	TRACE("<${cat}>")		
		foreach(item ${${_prefix}_${cat}})
			TRACE("   ${item}")		
		endforeach()
	 
	endforeach(cat )
	TRACE("-autocmake_check_modules ${_prefix}")
	

	
endmacro()

macro( autocmake_add_library _name )
	CMAKE_PARSE_ARGUMENTS( _this "STATIC;SHARED"
		   ""
		   "MODULES;SOURCES"
		   ${ARGN} )
	autocmake_check_modules( _module ${_this_MODULES})
	include_directories( ${_module_INCLUDE_DIRECTORIES} )
	link_directories( ${_module_LINK_DIRECTORIES} )

#	MESSAGE(" ${_this_STATIC} | ${_this_SHARED}")

	if( ${_this_STATIC} )
	    add_library( ${_name} STATIC ${_this_SOURCES})
	endif()

	if( ${_this_SHARED} )
	    add_library( ${_name} SHARED ${_this_SOURCES})
		target_link_libraries( ${_name} ${_module_LIBS})	
	endif()


endmacro()

macro( autocmake_add_add_executable _name )
	CMAKE_PARSE_ARGUMENTS( _this "" ""
		   "MODULES;SOURCES"
		   ${ARGN} )
	autocmake_check_modules( _module ${_this_MODULES})
	include_directories( ${_module_INCLUDE_DIRECTORIES} )
	link_directories( ${_module_LINK_DIRECTORIES} )
	add_executable( ${_name} ${_this_SOURCES})
	target_link_libraries( ${_name} ${_module_LIBS})	
endmacro()
