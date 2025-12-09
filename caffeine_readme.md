# The entire(?) Flutter widgets tree and elements tree ported to C++ for use in cocos2d-x 2.2.3 (Geometry Dash-specific edits) because Geode

by ported i mean copies the entire system's code over im that lazy but a good exercise regardless

## File structure
Same(ish) as the Flutter engine file structure (`package:flutter/foundation.dart` and `include/gdlazer/caffeine/foundation/*.hpp`)

Files not included below but appears in the folder contains a class named after the file name. Paths listed below relative to `include/gdlazer/caffeine` for headers and `src/caffeine` for sources

- foundation/Element: Contains the base `Element` class, `RenderObjectElement` and some private classes
- foundation/ComponentElements: Contains the base `ComponentElement` class and its 2 (or 3) most notable derives: `StatelessElement` and `StatefulElement` [TODO]
- foundation/Widget: Contains the base `Widget` class and `StatelessWidget` / `StatefulWidget` (not there yet)

