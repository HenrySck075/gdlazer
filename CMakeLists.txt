cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(osulazer VERSION 1.0.0)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    # Fill start
src/LabelBMFont.cpp
src/LabelBMFont.hpp
src/main.cpp
src/utils.cpp
src/utils.hpp
src/framework/graphics/Color4.cpp
src/framework/graphics/Color4.hpp
src/framework/graphics/color/ColorInfo.hpp
src/framework/utils/IInterpolable.hpp
src/framework/utils/Interpolation.cpp
src/framework/utils/Interpolation.hpp
src/game/beatmaps/BeatmapOnlineStatus.hpp
src/game/graphics/CCResizableSprite.cpp
src/game/graphics/CCResizableSprite.hpp
src/game/graphics/OsuColor.hpp
src/game/graphics/OsuIcon.cpp
src/game/graphics/OsuIcon.hpp
src/game/graphics/containers/BeatSyncedContainer.hpp
src/game/graphics/containers/ParallaxContainer.cpp
src/game/graphics/containers/ParallaxContainer.hpp
src/game/graphics/containers/SizeReactiveContainer.cpp
src/game/graphics/containers/WaveContainer.cpp
src/game/graphics/containers/WaveContainer.hpp
src/game/graphics/containers/beatsync/BeatEvent.hpp
src/game/graphics/ui/ButtonBase.cpp
src/game/graphics/ui/ButtonBase.hpp
src/game/graphics/ui/CCNodeHover.cpp
src/game/graphics/ui/CCNodeHover.hpp
src/game/graphics/ui/OsuText.hpp
src/game/graphics/ui/deco/Triangles.cpp
src/game/graphics/ui/deco/Triangles.hpp
src/game/online/rooms/RoomCategory.hpp
src/game/overlays/OverlayColorProvider.cpp
src/game/overlays/OverlayColorProvider.hpp
src/game/overlays/dialog/PopupDialog.cpp
src/game/overlays/dialog/PopupDialog.hpp
src/game/overlays/dialog/PopupDialogButton.cpp
src/game/overlays/dialog/PopupDialogButton.hpp
src/game/rulesets/mods/ModType.hpp
src/game/scoring/HitResult.hpp
src/game/scoring/RankingTier.hpp
src/game/scoring/ScoreRank.hpp
src/game/screens/menu/ButtonArea.cpp
src/game/screens/menu/ButtonArea.hpp
src/game/screens/menu/ButtonConstants.hpp
src/game/screens/menu/ButtonSystem.cpp
src/game/screens/menu/ButtonSystem.hpp
src/game/screens/menu/ButtonSystemState.hpp
src/game/screens/menu/MainMenu.cpp
src/game/screens/menu/MainMenu.hpp
src/game/screens/menu/MainMenuButton.cpp
src/game/screens/menu/MainMenuButton.hpp
src/game/screens/menu/OsuLogo.cpp
src/game/screens/menu/OsuLogo.hpp
src/game/screens/menu/intro/IntroTriangles.cpp
src/game/screens/menu/intro/IntroTriangles.hpp
src/helpers/ColorHelper.hpp
src/helpers/colors.hpp
src/helpers/CustomActions.cpp
src/helpers/CustomActions.hpp
src/helpers/MouseEvent.cpp
src/helpers/MouseEvent.hpp
src/helpers/sound/SoundManager.cpp
src/helpers/sound/SoundManager.hpp
src/main/PauseLayer.cpp
src/main/PauseLayer.hpp
    # Fill end
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# android
# target_link_libraries(${PROJECT_NAME} -L/data/data/com.termux/files/usr/lib/libGLESv2.so.2)

# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})
