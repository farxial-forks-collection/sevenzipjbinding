REM SET HTTP_PROXY=proxy.server:8080

SET CMD_MAKE=C:\mingw64\bin\mingw32-make.exe
SET GCC_BIN=C:/mingw64/bin
SET CMD_GCC=%GCC_BIN%/gcc.exe
SET CMD_GPP=%GCC_BIN%/g++.exe
SET CMD_WINDRES=%GCC_BIN%/windres.exe
SET RUNTIME_LIB=libgcc_s_seh-1.dll

SET JAVA_JDK=C:/Program Files/Java/jdk1.5.0_20

SET CMAKE_TOOLCHAIN=-G"MinGW Makefiles" -DMINGW64=Yes -DCMAKE_RC_COMPILER:FILEPATH=%CMD_WINDRES%  -DCMAKE_C_COMPILER:FILEPATH=%CMD_GCC% -DCMAKE_CXX_COMPILER:FILEPATH=%CMD_GPP% -DCMAKE_SHARED_LINKER_FLAGS:STRING="-specs C:\SevenZipJBinding\gcc64.specs" -DRUNTIME_LIB=%RUNTIME_LIB%
