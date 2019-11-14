# BGETool

BGETool mostly consists of a DLL which is being injected into Beyond Good and Evil. It allows for things like a Freecam, Teleportation and a re-enabled developer debug console aswell as potentially much more.
This is my first "big" C/C++ project so please don't judge the code too harshly, constructive criticism/pull requests is/are always appreciated though.
I'll greatly appreciate any feedback or screenshots of findings you'll make with the tool :)

## Supported versions
* GOG version 2.1.0.9
* Steam version (Signature timestamp: 09/27/2019 10:55:37)
* Uplay version (Signature timestamp: 09/27/2019 07:35:53)
* [Demo](https://www.4players.de/4players.php/download_info/Downloads/download/5521/Beyond_Good__Evil/Demo.html)

If there's a PC version that's not yet supported, contact me on Discord or send me an email and I'll see what I can do.

## Installing
**I've tried to make this as simple as possible, there are some differences for the Steam and Uplay versions though.**
Download the release [here](https://github.com/4g3v/BGETool/releases) and ***unzip it into the root directory of your Beyond Good and Evil installation.***

If you aren't using the Uplay version, you don't need to copy the "uplay32.dll".

***If you are using the Uplay version, the "uplay32.dll" is necessary because Uplay's DRM is creating problems. Copying this file will disable Uplay integration, meaning that your friends probably won't see that you are playing the game.***

***If you are using the Steam version, you absolutely need to use [Steamless](https://github.com/atom0s/Steamless) which is downloadable [here](https://github.com/atom0s/Steamless/releases/download/v3.0.0.9/Steamless.v3.0.0.9.-.by.atom0s.7z) to unpack the "BGE.exe". Download it, extract it and run the "Steamless.exe". You can leave every option as it is and select the "BGE.exe" as the file to unpack. Now you'll just need to click "Unpack File" and let Steamless do its job. After it has finished you'll need to rename the "BGE.exe" file to something like "BGE_original.exe". Now you'll need to rename the "BGE.exe.unpacked.exe" to "BGE.exe" and you're ready to go!***

## Usage and controlling BGETool
**Instead of starting the game normally, you'll need to run the "BGEToolInjector.exe"**. This will start the game with the self-explanatory window/resolution options in the "BGETool.ini" configuration file.
***Once you are in-game and have loaded a save or created a new game you'll need to press the *Delete* key on your keyboard. You'll hear a typical windows sound to let you know that it's ready to use.***

***Controls:***
* ***Delete*** key | Enables or disables the freecam.
* ***WASD***, ***Q*** and ***E*** keys | Moves the position of the freecam.
* ***Mouse*** | Let's you look around while using the freecam.
* ***Space*** | Teleports you to the location of the freecam. Note that this is still finicky, you'll likely need to hold the space key and move the freecam around to successfully teleport. It's kind of hard right now but with enough patience you'll get where you want.
* ***F12*** | Opens the developer's debug console to enter commands which are specified in the **Console Commands** section.
* ***Insert*** | Allows you to minimize the game if you are having problems ALT-Tabbing like me.

***Demo quirks:***
As a side effect of enabling the debug console, there are several more debug things you can do in the demo. **F1-F12** all do some different thing, F12 is still enabling the console. I haven't figured out what every key does though. 
* ***F1*** | Opens a small window, which you'll have to resize, with some engine information in there. You can press it multiple times for different info.
* ***F3*** | Changes the rendering into **wireframe** mode.
* ***F6*** | Reloads the current level.

## Console commands
Unfortunately these mostly change settings you can set with the "SettingsApplication.exe" but there are a couple of nice exceptions.

| Command | Arguments | Available in the demo? | Description | Example | 
| --- | --- | --- | --- | --- |
| ae_aa | 0-~50 | No | Changes the game's antialiasing. Not every value seems to have an effect and going futher than 50 causes rendering bugs | ae_aa 13 | 
| **ae_bands** | 0/1 | No | Enables/Disables the **black bars** on the top and bottom of the screen | ae_bands 0 |
| **ae_nobands** | 0/1 | Only in the demo | Enables/Disables the **black bars** on the top and bottom of the screen | ae_nobands 0 |
| ae_d2x | 0/1 ? | Yes | Changes something in the rendering of the game. No idea what it is though | ae_d2x 1 |
| ae_no | 0/1 | Yes | Enables/Disables the AfterFx | ae_no 1 |
| ae_noflare | 0/1 | Yes | Enables/Disables the Flare | ae_noflare 0 |
| ae_tv | 0/1 | No | Enables/Disables the Tv-Mode. Changes something in the rendering of the game. No idea what it is though | ae_tv 1 |
| fpsmin | Any decimal? | No | Seems to change the rate at which the game reads its input | fpsmin 20.5 |
| **mipmaplodbias** | Any decimal | Yes | Changes the **[mipmap lod bias](https://www.reddit.com/r/pcmasterrace/comments/433c5x/can_someone_explain_lod_bias_texture_filtering/czff51z/)** | mipmaplodbias 8 |
| perf | 0/1/2/3/.. | Yes | Enables/Disables the **performance profiler** | perf 1 |
| perf_fps | Number | Yes | Changes the rate the performance profiler updates itself | perf_fps 10 |
| perf_fps_min | Number | Yes | Changes the minimum rate the performance profiler updates itself? | perf_fps_min 10 |
| perf_fps_target | Number | Yes | Changes the target rate the performance profiler updates itself? | perf_fps_target 12 |
| **speed** | 0-the limits of the engine | No | **Multiplies the speed of the game by the argument.** | speed 3 |
| water_blur_normal | 0/1? | Yes | Changes something related to the blur of the water | water_blur_normal 0 |
| water_clip | 0/1? | Yes | Changes something related to the rendering of the water | water_clip 0 |
| water_clip_off | 0/1? | Yes | Changes something related to the rendering of the water | water_clip_off 0 |
| water_min_bv | -1.0-1.0? | Yes | Changes something related to the rendering of the water | water_min_bv -1.0 |
| water_smooth | 0/1 | Yes | Changes the WaterQuality | water_smooth 1 |
| water_smoothx | Decimal | Yes | Changes something related to the water quality | water_smoothx 3.0 |
| water_smoothy | Decimal | Yes | Changes something related to the water quality | water_smoothy 3.0 |
| zonalpha_no | 0/1? | Yes | Changes something in the rendering of the game. | zonalpha_no 1 |

## Built With

* [Microsoft Research Detours Package](https://github.com/microsoft/Detours) - Used for function hooking

## License

This project is licensed under the GNU GPLv3 - see the [LICENSE.md](LICENSE.md) file for details

## Thanks to
### Personal
* ***Ubisoft for their awesome game of course.*** This game has played a huge role in my childhood and it's still a great game as an adult. The lighthouse scene with Woof still makes me so emotional after all those years. I'm just being flooded with good memories when I am playing this game, it's also probably the game I have completed the most in my life so far. I'm really anticipating BGE2 and I hope it will be as great as the first title, even though the unresolved cliffhanger was kind of mean. I could go on like this for quite some time but the readme wouldn't end then, so thank you so much Ubisoft!
* ***"StarBoar D"*** for helping me adding support of the Uplay version, aswell as providing me with a Uplay key for BGE. This has greatly sped up the implementation of this version. You are awesome dude! :)
* ***The Beyond Good and Evil community in general***, consider this project as some sort of tribute ^^
* ***#bge1_modding on the [BGE Discord server](https://discord.me/beyondgoodandevil)***, without your support to the first WIP video of the Freecam, this project would have been left unfinished (not saying that it's finished right now as it it) on my harddrive for eternity. Your responses motivated me, and still are motivating me, to keep working on this project.

### Code
* GuidedHacking for [his First-Internal-Hack-Tutorial](https://github.com/guided-hacking/First-Internal-Hack-Tutorial1) which is used as a very simple base for this project
* madshi for [their GetInterfaceMethod function](http://forum.madshi.net/viewtopic.php?p=44424&sid=30c61f2684495a91b21796c97d7b8ffe#p44424)
* Mark Adler for [his CRC32 function](https://stackoverflow.com/questions/27939882/fast-crc-algorithm)
* Neargye for [their magic_enum project](https://github.com/Neargye/magic_enum)