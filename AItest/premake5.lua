workspace "AItest"
    configurations { "Debug", "Release" }
    architecture "x86_64"
    location "build"          -- .sln y .vcxproj en /build

project "AItest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"       -- usa runtime dinámica de MSVC

    -- exe en bin/Debug o bin/Release
    targetdir ("bin/%{cfg.buildcfg}")
    -- obj intermedios
    objdir ("build/obj/%{cfg.buildcfg}")
    -- directorio de trabajo: AItest/bin  (para que ../assets/... funcione)
    debugdir "bin"

    -- Código del juego + ImGui
    files {
        "src/**.cpp",
        "include/**.h",
        "deps/imgui/*.cpp",
        "deps/imgui/backends/imgui_impl_sdl2.cpp",
        "deps/imgui/backends/imgui_impl_sdlrenderer2.cpp"
    }

    -- Includes
    includedirs {
        "include",
        "deps/SDL2-2.32.8/include",
        "deps/SDL2_image-2.8.8/include",
        "deps/imgui",
        "deps/imgui/backends"
    }

    -- Librerías
    libdirs {
        "deps/SDL2-2.32.8/lib/x64",
        "deps/SDL2_image-2.8.8/lib/x64"
    }

    links {
        "SDL2",
        "SDL2main",
        "SDL2_image"
    }

    -- Filtros lógicos en Visual (para ver src/include más ordenado)
    vpaths {
        ["src"]     = { "src/**.cpp" },
        ["include"] = { "include/**.h" },
        ["imgui"]   = { "deps/imgui/**.cpp", "deps/imgui/**.h" }
    }

    filter "configurations:Debug"
        symbols "On"
        defines { "DEBUG" }

    filter "configurations:Release"
        optimize "On"
        defines { "NDEBUG" }
