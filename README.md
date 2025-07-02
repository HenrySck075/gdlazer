
<a href="https://www.pixiv.net/en/artworks/130930702"><img src="resources/130930702_p0.png"/></a>
([Artwork URL](https://www.pixiv.net/en/artworks/130930702) in case that doesn't hyperlink)

# Build instructions
For more info, see [our docs](https://docs.geode-sdk.org/getting-started/create-mod#build)
```sh
# Assuming you have the Geode CLI set up already
geode build
```

## Minimum requirements
- A copy of Geometry Dash on latest version (yeah thanks captain obvious) with Geode installed (yeah yeah)
- Clang that supports C++20 at minimum (due to the use of `__VA_OPT__` in some macros)

# Contributing
## `index_friendly_diffs.diff`
Add more diffs that will make the mod index-compliant if you ever add any goofy features that is available for developer releases only.<br/>
(mod verifiers should focus on this part it is very important)<br/>
<br/>
In specific, this diff will be applied on the build action to remove or add changes to the repo.

# Resources
* [Geode SDK Documentation](https://docs.geode-sdk.org/)
* [Geode SDK Source Code](https://github.com/geode-sdk/geode/)
* [Geode CLI](https://github.com/geode-sdk/cli)
* [Bindings](https://github.com/geode-sdk/bindings/)
* [DevTools](https://github.com/geode-sdk/DevTools) (the index builds is **NOT** compatible with gd!lazer as it might yeet some containers to a far, far away region)
* [osu!lazer source](https://github.com/ppy)



to anyone verifying the mod, :3