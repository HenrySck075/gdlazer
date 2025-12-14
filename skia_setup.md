Skia prebuilt binaries from release of https://github.com/HenrySck075/geode-skia/releases
(if we can get latest w/o explicit tag ref too then coolio)

Bundled with each release is the archive for the supported platforms listed below:
- Win64: skia-Windows.zip
- Android64: skia-Android64.tar.gz
- MacOS: skia-macOS.tar.gz

(platform name are expected to be compared to GEODE_TARGET_PLATFORM cmake variable)

each archive contains `include` which, of course, includes a copy of skia's include folder, and `lib` which contains the shared libraries of skia and dependencies.
The archive should be extracted to `skia/[platform]` (for the package combine to work), CMake should add an include directory to it, and, of course, should links them to the project.

Cache the archive if possible.
