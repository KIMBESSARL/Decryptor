#
# CMakeList.txt : CMake project for Compilers, include source and define project specific logic here.
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "    @@                           @@    ”
# ECHO "    @@                           @@    ”
# ECHO "    @@      @@@@@@@@@@@@@        @@    �"
# ECHO "    @@      @@           @@      @@    �"
# ECHO "    @@      @@            @@     @@    �"
# ECHO "    @@      @@             @@    @@    �"
# ECHO "    @@      @@             @@    @@    �"
# ECHO "    @@      @@            @@     @@    �"
# ECHO "    @@      @@           @@      @@    �"
# ECHO "    @@      @@@@@@@@@@@@@        @@    �"
# ECHO "    @@                           @@    ”
# ECHO "    @@     D E C R Y P T E R     @@    ”
# ECHO "    @@      KINGSLY / JATIN      @@    ”
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "

cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Decryptor")

# Add source to this project's executable.
add_executable (Decryptor  
	"Compilers.h"		"Compilers.c"
	"Step1Coder.h"		"Step1Coder.c"		"Main1Coder.c"
	"Main2Reader.c"     "Step2Reader.c"     "Step2Reader.h"
	"Main3Scanner.c"		"Step3Scanner.c"	"Step3Scanner.h"    
 )
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Decryptor PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
