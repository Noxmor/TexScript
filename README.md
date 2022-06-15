# TexScript

## TexScript is a C++ Text Adventure Game with Lua mod support.

Tescript features a base game, which can be seen as a standalone Text Adventure Game itself or be used to showcase the potential for moddind this game. While the internal system is handled via c++, the actual Text Adventure is created by using lua scripts. Internally, TexScript currently supports the following features:

* Map (defined by locations and paths)
* Items
* Basic Lua mod support (only for the base game)
* Localisation

***

## Inspiration
Why did I create Tescript? I always liked JRPGs e.g. Dragon Quest and games that allow the user to add their own content trough modding. I wondered how to create a game that includes modding support. However, I mostly wanted to create this game from scratch without any engine, but also refuse to handle code for any graphics. That's how Texscript was created.

***

## The Goal
Texscript is supposed to be featuring the following:

* JRPG like combat system
* Story progression
* Map (defined by locations and paths)
* Items
* Monsters
* Characters
* Quests
* Lua mod support to customize all of the above

***

## Getting Started
Visual Studio 2022 is recommended, TexScript is officially untested on other development environments whilst focused on a Windows build.

<ins>**1. Downloading the repository:**</ins>

Start by cloning the repository with `git clone --recursive https://github.com/Noxmor/TexScript`.

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

<ins>**2. Creating a visual studio solution:**</ins>

1. Run the [Setup.bat](https://github.com/Noxmor/TexScript/blob/master/scripts/Setup.bat) file found in `scripts` folder. This will generate a Visual Studio solution file.

If changes are made, or if you want to regenerate project files, rerun the [Setup.bat](https://github.com/Noxmor/TexScript/blob/master/scripts/Setup.bat) script file found in `scripts` folder.

***

## Known bugs
Currently none.
