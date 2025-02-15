cmake_minimum_required(VERSION 3.8)
project(HookService LANGUAGES CSharp VERSION 1.0.0)
set(CMAKE_VS_PLATFORM_TOOLSET "v142")
 
# Include CMake utilities for CSharp, for WinForm and WPF application support.
include(CSharpUtilities)
 
# Define the executable, including any .cs files. 
# The .resx and other Properties files are optional here, but including them makes them visible in the VS solution for easy editing. 
add_executable(HookService
    src/main.cs
    src/sdk/FastWin32/FastWin32Settings.cs
    src/sdk/FastWin32/NativeMethods.cs
    src/sdk/FastWin32/SafeNativeHandle.cs
    src/sdk/FastWin32/Asm/AsmTable.cs
    src/sdk/FastWin32/Asm/AsmCompileException.cs
    src/sdk/FastWin32/Asm/Assembler.cs
    src/sdk/FastWin32/Diagnostics/Injector.cs
    src/sdk/FastWin32/Diagnostics/Process32.cs
    src/sdk/FastWin32/Diagnostics/Module32.cs
    src/sdk/FastWin32/Hook/Method/LocalHook.cs
    src/sdk/FastWin32/Hook/Method/LocalHookOld.cs
    src/sdk/FastWin32/Hook/Method/RemoteHook.cs
    src/sdk/FastWin32/Hook/WindowMessage/MessageProxy.cs
    src/sdk/FastWin32/Hook/WindowMessage/KeyboardHook.cs
    src/sdk/FastWin32/Memory/MemoryManagement.cs
    src/sdk/FastWin32/Memory/MemoryIO.cs
    src/sdk/FastWin32/Memory/PageInfo.cs
    src/sdk/FastWin32/Memory/Pointer.cs
    src/sdk/FastWin32/Properties/AssemblyInfo.cs
    src/sdk/FastWin32/Windowing/Window.cs

)
 
# Set the .NET Framework version for the executable.
set_property(TARGET HookService PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
set_property(TARGET HookService PROPERTY VS_GLOBAL_AllowUnsafeBlocks "true")

# Set the executable to be 32-bit.
# set_property(TARGET HookService PROPERTY WIN32_EXECUTABLE TRUE)
# Set the C# language version (defaults to 3.0).
set(CMAKE_CSharp_FLAGS "/langversion:latest")
 

 
# If necessary, link in other library dependencies that were built locally in this source tree.
#target_link_libraries(MyWinFormApp MyLocalLib)
 
# If necessary, link in other library/DLL references, such as 3rd party libraries.
#set_property(TARGET MyWinFormApp PROPERTY 
#    VS_DOTNET_REFERENCE_MyThirdPartyLib /path/to/libs/MyThirdPartyLib.dll)
 
# Add in the .NET reference libraries.
set_property(TARGET HookService PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "System"
    "System.Core"
    "System.Data"
    "System.Drawing"
    "System.Windows.Forms"
)

# add_custom_command(
#     TARGET HookService
#     POST_BUILD
#     COMMAND makensis /DOUTPUT_PATH="${CMAKE_BINARY_DIR}" ${CMAKE_SOURCE_DIR}/installer/setup.nsi
#     COMMENT "Generating NSIS installer in ${CMAKE_BINARY_DIR}..."
# )

add_custom_command(
    TARGET HookService
    POST_BUILD
    COMMAND makensis 
        /DVERSION="${PROJECT_VERSION}"
        /DPRODUCT_NAME="HookService"
        /DOUTPUT_PATH="${CMAKE_BINARY_DIR}"
        ${CMAKE_SOURCE_DIR}/installer/setup.nsi
    COMMENT "Generating NSIS installer in ${CMAKE_BINARY_DIR}..."
)

# cmake -G "Visual Studio 17 2022"  ../