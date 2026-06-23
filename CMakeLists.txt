cmake_minimum_required(VERSION 3.22)
project(AeraVisualizer LANGUAGES CXX)

# ---------------------------------------------------------------------------
# C++ standard
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Qt
# ---------------------------------------------------------------------------
# Point CMAKE_PREFIX_PATH at your Qt installation if it is not on the system
# path, e.g.:
#   cmake -DCMAKE_PREFIX_PATH=/opt/Qt/5.14.2/gcc_64 ..
#
# Qt 5.14 is the version used by the Windows build. Qt 5.15 (LTS) also works
# and is more readily available on most Linux distributions.
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Let CMake drive moc/rcc/uic automatically for all targets below.
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ---------------------------------------------------------------------------
# Threads (replaces wsock32.lib on Linux; also needed by AERA's CoreLibrary)
# ---------------------------------------------------------------------------
find_package(Threads REQUIRED)

# ---------------------------------------------------------------------------
# Preprocessor definitions
#
# WITH_DETAIL_OID     – required for the Visualizer to map objects by detail
#                       OID. See CoreLibrary/base.h; must be set before any
#                       AERA headers are included. Replaces the manual
#                       uncomment step described in the Windows INSTALL.md.
# EXECUTIVE_EXPORTS   – marks r_exec symbols for export (matches vcxproj x64)
# CORELIBRARY_EXPORTS – marks CoreLibrary symbols for export
#
# ENABLE_PROTOBUF is present in the dev vcxproj x64 preprocessor definitions.
# It is commented out here because the TCP/Protobuf IO device sources are
# compiled in but Protobuf itself is not yet a declared dependency of this
# CMakeLists. Uncomment and add find_package(Protobuf REQUIRED) plus
# target_link_libraries(... protobuf::libprotobuf) when adding Protobuf
# support.
#
# _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING is MSVC-only; omitted.
# ---------------------------------------------------------------------------
add_compile_definitions(
    WITH_DETAIL_OID
    EXECUTIVE_EXPORTS
    CORELIBRARY_EXPORTS
    ENABLE_PROTOBUF
)

# ---------------------------------------------------------------------------
# Include directories
# ---------------------------------------------------------------------------
set(SUBMODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules")
set(AERA_DIR       "${SUBMODULES_DIR}/AERA")
set(CORE_DIR       "${AERA_DIR}/submodules/CoreLibrary/CoreLibrary")

include_directories(
    # Visualizer root
    "${CMAKE_CURRENT_SOURCE_DIR}"
    # AERA submodule roots (headers use paths like "r_code/atom.h")
    "${AERA_DIR}"
    # AERA sub-directories referenced directly by headers
    "${AERA_DIR}/r_code"
    "${AERA_DIR}/r_comp"
    "${AERA_DIR}/r_exec"
    "${AERA_DIR}/AERA"
    "${AERA_DIR}/AERA/IODevices/TCP"
    "${AERA_DIR}/AERA/IODevices/TCP/AERA_Protobuf"
    "${AERA_DIR}/usr_operators"
    "${AERA_DIR}/usr_operators/Operators"
    "${AERA_DIR}/usr_operators/Callbacks"
    # CoreLibrary
    "${CORE_DIR}"
)

# ---------------------------------------------------------------------------
# Source files
#
# Mirrors the <ClCompile> items in the dev branch AERA_Visualizer.vcxproj
# (x64 configuration). Files marked ExcludedFromBuild on x64 are omitted;
# they are #included by their companion .cpp files (AERA's unity-build
# pattern for .tpl.cpp and .inline.cpp files).
#
# Source lists are cross-referenced against the aera-on-linux CMakeLists.txt
# (nonbinarysolo/AERA, aera-on-linux branch) which has been tested on
# Ubuntu 24.04 LTS in 64-bit configuration.
# ---------------------------------------------------------------------------

# --- Visualizer sources ---
set(VISUALIZER_SOURCES
    main.cpp
    abagraph.cpp
    aera-checkbox.cpp
    aera-visualizer-window.cpp
    find-dialog.cpp
    replicode-objects.cpp

    graphics-items/aba-sentence-item.cpp
    graphics-items/aba-step-failed-item.cpp
    graphics-items/aera-graphics-item.cpp
    graphics-items/aera-graphics-item-group.cpp
    graphics-items/aera-visualizer-scene.cpp
    graphics-items/anchored-horizontal-line.cpp
    graphics-items/arrow.cpp
    graphics-items/auto-focus-fact-item.cpp
    graphics-items/composite-state-goal-item.cpp
    graphics-items/composite-state-item.cpp
    graphics-items/composite-state-prediction-item.cpp
    graphics-items/drive-item.cpp
    graphics-items/expandable-goal-or-pred-item.cpp
    graphics-items/instantiated-composite-state-item.cpp
    graphics-items/instantiated-model-item.cpp
    graphics-items/io-device-inject-eject-item.cpp
    graphics-items/model-goal-item.cpp
    graphics-items/model-imdl-prediction-item.cpp
    graphics-items/model-item.cpp
    graphics-items/model-prediction-from-requirement-disabled-item.cpp
    graphics-items/model-prediction-from-requirement-item.cpp
    graphics-items/model-prediction-item.cpp
    graphics-items/predicted-instantiated-composite-state-item.cpp
    graphics-items/prediction-item.cpp
    graphics-items/prediction-result-item.cpp
    graphics-items/program-output-fact-item.cpp
    graphics-items/program-reduction-item.cpp
    graphics-items/promoted-prediction-defeated-item.cpp
    graphics-items/promoted-prediction-item.cpp
    graphics-items/reduction-marker-item.cpp
    graphics-items/simulation-commit-item.cpp

    views/explanation-log.cpp
    views/player.cpp
    views/semantics.cpp
    views/task-environment.cpp
    views/text-output.cpp
)

# --- AERA r_code sources ---
set(AERA_RCODE_SOURCES
    ${AERA_DIR}/r_code/atom.cpp
    ${AERA_DIR}/r_code/code_utils.cpp
    ${AERA_DIR}/r_code/image.cpp
    ${AERA_DIR}/r_code/image_impl.cpp
    ${AERA_DIR}/r_code/object.cpp
    ${AERA_DIR}/r_code/r_code.cpp
)

# --- AERA r_comp sources ---
set(AERA_RCOMP_SOURCES
    ${AERA_DIR}/r_comp/class.cpp
    ${AERA_DIR}/r_comp/compiler.cpp
    ${AERA_DIR}/r_comp/decompiler.cpp
    ${AERA_DIR}/r_comp/preprocessor.cpp
    ${AERA_DIR}/r_comp/r_comp.cpp
    ${AERA_DIR}/r_comp/segments.cpp
    ${AERA_DIR}/r_comp/structure_member.cpp
)

# --- AERA r_exec sources ---
set(AERA_REXEC_SOURCES
    ${AERA_DIR}/r_exec/ast_controller.cpp
    ${AERA_DIR}/r_exec/auto_focus.cpp
    ${AERA_DIR}/r_exec/binding_map.cpp
    ${AERA_DIR}/r_exec/callbacks.cpp
    ${AERA_DIR}/r_exec/_context.cpp
    ${AERA_DIR}/r_exec/context.cpp
    ${AERA_DIR}/r_exec/cpp_programs.cpp
    ${AERA_DIR}/r_exec/cst_controller.cpp
    ${AERA_DIR}/r_exec/factory.cpp
    ${AERA_DIR}/r_exec/g_monitor.cpp
    ${AERA_DIR}/r_exec/group.cpp
    ${AERA_DIR}/r_exec/guard_builder.cpp
    ${AERA_DIR}/r_exec/hlp_context.cpp
    ${AERA_DIR}/r_exec/hlp_controller.cpp
    ${AERA_DIR}/r_exec/hlp_overlay.cpp
    ${AERA_DIR}/r_exec/init.cpp
    ${AERA_DIR}/r_exec/mdl_controller.cpp
    ${AERA_DIR}/r_exec/mem.cpp
    ${AERA_DIR}/r_exec/model_base.cpp
    ${AERA_DIR}/r_exec/monitor.cpp
    ${AERA_DIR}/r_exec/opcodes.cpp
    ${AERA_DIR}/r_exec/operator.cpp
    ${AERA_DIR}/r_exec/overlay.cpp
    ${AERA_DIR}/r_exec/pattern_extractor.cpp
    ${AERA_DIR}/r_exec/pgm_controller.cpp
    ${AERA_DIR}/r_exec/pgm_overlay.cpp
    ${AERA_DIR}/r_exec/p_monitor.cpp
    ${AERA_DIR}/r_exec/reduction_core.cpp
    ${AERA_DIR}/r_exec/reduction_job.cpp
    ${AERA_DIR}/r_exec/time_core.cpp
    ${AERA_DIR}/r_exec/time_job.cpp
    ${AERA_DIR}/r_exec/view.cpp
)

# --- CoreLibrary sources ---
set(AERA_CORE_SOURCES
    ${AERA_DIR}/submodules/CoreLibrary/CoreLibrary/base.cpp
    ${AERA_DIR}/submodules/CoreLibrary/CoreLibrary/utils.cpp
    ${AERA_DIR}/submodules/CoreLibrary/CoreLibrary/xml_parser.cpp
)

# --- AERA AERA/ sources (main runtime + IO devices) ---
# These match the sources compiled by the aera-on-linux CMakeLists.
# The TCP/Protobuf sources are included unconditionally so the translation
# units exist; they will compile cleanly without ENABLE_PROTOBUF defined
# because the protobuf-specific code paths are guarded by that flag.
# Enable ENABLE_PROTOBUF above and link Protobuf to activate them fully.
set(AERA_MAIN_SOURCES
    ${AERA_DIR}/AERA/main.cpp
    ${AERA_DIR}/AERA/test_mem.cpp
    ${AERA_DIR}/AERA/IODevices/TCP/tcp_io_device.cpp
    ${AERA_DIR}/AERA/IODevices/TCP/AERA_Protobuf/tcp_connection.cpp
    ${AERA_DIR}/AERA/IODevices/TCP/AERA_Protobuf/tcp_data_message.pb.cc
    ${AERA_DIR}/AERA/IODevices/video_screen/video_screen.cpp
    ${AERA_DIR}/AERA/IODevices/video_screen/video_screen_io_device.cpp
)

# --- usr_operators sources ---
set(AERA_USR_SOURCES
    ${AERA_DIR}/usr_operators/auto_focus_callback.cpp
    ${AERA_DIR}/usr_operators/usr_operators.cpp
    ${AERA_DIR}/usr_operators/Callbacks/callbacks.cpp
    ${AERA_DIR}/usr_operators/Operators/operators.cpp
    ${AERA_DIR}/usr_operators/TestProgram/test_program.cpp
)

# --- Qt resource file ---
#set(QRC_FILES aera_visualizer.qrc)
qt5_add_resources(QRC_GENERATED aera_visualizer.qrc)

# ---------------------------------------------------------------------------
# Executable target
# ---------------------------------------------------------------------------
add_executable(AeraVisualizer
    ${VISUALIZER_SOURCES}
    ${AERA_RCODE_SOURCES}
    ${AERA_RCOMP_SOURCES}
    ${AERA_REXEC_SOURCES}
    ${AERA_CORE_SOURCES}
    ${AERA_MAIN_SOURCES}
    ${AERA_USR_SOURCES}
    ${QRC_GENERATED}
)

# AERA/main.cpp and the Visualizer's main.cpp both define main(). The vcxproj
# handles this by compiling AERA/main.cpp to a distinct object name
# (AERA_main.obj). CMake doesn't need the rename trick but we must ensure the
# linker sees only one main() entry point. The AERA main.cpp is included
# because it contains non-main symbols used at runtime; if it causes a
# duplicate-symbol link error, exclude it and verify which symbols are needed.

# ---------------------------------------------------------------------------
# Link libraries
# ---------------------------------------------------------------------------
target_link_libraries(AeraVisualizer
    PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Threads::Threads
        # stdc++fs is needed on GCC < 9 for std::filesystem. GCC 13 (Ubuntu
        # 24.04 default) includes it in libstdc++ automatically; the explicit
        # link is harmless but generates a warning on newer toolchains.
        # Uncomment if building on an older GCC:
        stdc++fs
        $<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS}>
        $<$<PLATFORM_ID:Windows>:wsock32>
        libprotobuf.a
)

# ---------------------------------------------------------------------------
# Platform-specific compile options
# ---------------------------------------------------------------------------
if(MSVC)
    target_compile_options(AeraVisualizer PRIVATE /W4 /MP)
else()
    target_compile_options(AeraVisualizer PRIVATE
        -Wall
        -Wextra
        -Wno-unused-parameter
        -Wno-reorder
        -fpermissive
    )
endif()

# ---------------------------------------------------------------------------
# Notes for the builder
# ---------------------------------------------------------------------------
# 1. Submodules: run  git submodule update --init --recursive  before cmake.
#    Point the AERA submodule at nonbinarysolo/AERA@aera-on-linux manually
#    if it still references IIIM-IS/AERA@master.
#
# 2. Qt location: if Qt is not found automatically, pass:
#      -DCMAKE_PREFIX_PATH=/path/to/Qt/5.14.2/gcc_64
#
# 3. WITH_DETAIL_OID is set via add_compile_definitions above. The Windows
#    INSTALL.md asks you to uncomment it in CoreLibrary/base.h manually;
#    with CMake that manual step is not needed.
#
# 4. ENABLE_PROTOBUF / Protobuf support: uncomment the definition above,
#    add  find_package(Protobuf REQUIRED)  and add  protobuf::libprotobuf
#    to target_link_libraries. The .pb.cc source is already in the source
#    list; it will need re-generation via protoc if the .proto file changes.
#
# 5. Duplicate main() symbol: both the Visualizer and AERA/AERA/main.cpp
#    define main(). The vcxproj compiles them into separate object files
#    with distinct names and the linker uses the Visualizer's main(). If
#    this causes a link error, remove ${AERA_DIR}/AERA/main.cpp from
#    AERA_MAIN_SOURCES and stub out any symbols it provides that are
#    referenced at runtime.
#
# 6. .tpl.cpp / .inline.cpp files are intentionally excluded. The vcxproj
#    marks them ExcludedFromBuild on x64; they are #included by their
#    companion .cpp files at compile time.
#
# 7. app.ico is referenced via Qt resources (aera_visualizer.qrc). Qt's ICO
#    plugin is available on Linux but not guaranteed to be installed. Consider
#    adding a PNG fallback to the .qrc and guarding the .ico load with
#    #ifdef Q_OS_WIN in main.cpp.


# Remember to run `sudo apt install libgles2-mesa-dev` to get Qt to work