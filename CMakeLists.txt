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
src/helpers/CustomActions.cpp
src/helpers/CustomActions.hpp
src/helpers/MouseEvent.cpp
src/helpers/MouseEvent.hpp
src/helpers/colors.hpp
src/helpers/ColorHelper.hpp
src/helpers/sound/SoundManager.hpp
src/helpers/sound/SoundManager.cpp
src/framework/README.md
src/framework/utils/Interpolation.hpp
src/framework/utils/IInterpolable.hpp
src/framework/utils/Interpolation.cpp
src/framework/graphics/Color4.hpp
src/framework/graphics/color/ColorInfo.hpp
src/framework/io/stores/FontStores.hpp
src/main/PauseLayer.hpp
src/main/PauseLayer.cpp
src/game/README.md
src/game/scoring/HitResult.hpp
src/game/scoring/ScoreRank.hpp
src/game/scoring/RankingTier.hpp
src/game/screens/menu/MainMenu.hpp
src/game/screens/menu/ButtonSystem.hpp
src/game/screens/menu/ButtonSystem.cpp
src/game/screens/menu/ButtonArea.hpp
src/game/screens/menu/MainMenuButton.hpp
src/game/screens/menu/MainMenu.cpp
src/game/screens/menu/ButtonSystemState.hpp
src/game/screens/menu/MainMenuButton.cpp
src/game/screens/menu/intro/IntroTriangles.cpp
src/game/screens/menu/intro/IntroTriangles.hpp
src/game/graphics/OsuColor.h
src/game/graphics/OsuIcon.hpp
src/game/graphics/CCResizableSprite.hpp
src/game/graphics/CCResizableSprite.cpp
src/game/graphics/ui/CCNodeHover.hpp
src/game/graphics/ui/CCNodeHover.cpp
src/game/graphics/ui/deco/Triangles.cpp
src/game/graphics/ui/deco/Triangles.hpp
src/game/graphics/containers/WaveContainer.cpp
src/game/graphics/containers/WaveContainer.hpp
src/game/graphics/containers/BeatSyncedContainer.hpp
src/game/graphics/containers/beatsync/BeatDetector.hpp
src/game/graphics/containers/beatsync/README.md
src/game/graphics/containers/beatsync/TimeStamp.hpp
src/game/graphics/containers/beatsync/BeatDetector.cpp
src/game/graphics/containers/beatsync/TimeStamp.cpp
src/game/graphics/containers/beatsync/BeatEvent.hpp
src/game/overlays/OverlayColorProvider.cpp
src/game/overlays/OverlayColorProvider.hpp
src/game/overlays/dialog/PopupDialog.hpp
src/game/overlays/dialog/DialogButton.cpp
src/game/overlays/dialog/PopupDialog.cpp
src/game/overlays/dialog/DialogButton.hpp
src/game/beatmaps/BeatmapOnlineStatus.hpp
src/game/rulesets/mods/ModType.hpp
src/game/online/rooms/RoomCategory.hpp
    # Fill end
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
