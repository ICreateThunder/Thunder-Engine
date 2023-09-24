# Thunder Engine

## Introduction

This is a small project to learn and practice graphics and advanced C programming. It will follow the tutorial series on the ["Kohi Engine"](https://youtube.com/playlist?list=PLv8Ddw9K0JPg1BEO-RS-0MYs423cvLVtj)

### Credits

Thank you to "Travis Vroman" on YouTube, from which I have taken much code/ideas.

## Specifications

### Goals

1. Open-Source
2. Learn C damn-it!
3. 3D Primarily, with support for 2D

### Language / Compiler

Source code will be written in C. Be careful, with great power comes great responsibility.

Compiler: Clang

### Project Structure

Libraries (DLL / so): 

- Thunder-Engine: Source code for the game engine
- Hot-Reloadable Game Code: References the engine library, allowing for quick updates and changes as opposed to complete recompilation of libraries/game code

Executables:

- Sandbox / TestBed: Executable of test game/features of the game engine
- Game code: Executable using references from the Engine library and "Hot-Reloadable Game Code"
- Editor: Editor logic is outside of the core game engine code, references the game engine library and provides a friendly interface to utilise the engine.

### Feature List

- Lightweight, crossplatform build system
- Low-level utilities (dynamic arrays, string handling, etc)
- Platform layer (Windows and Linux)
- Logger
- File IO
- Application Layer
- Renderer / API Abstraction layer
- Memory management
- Scenegraph/ECS
- Profiling/Debugging utilities
- "Scripting" support via hot-reloading
- Physics system

[Updated as of 24/09/2023]

### Architecture

General overview:

1. Platform Layer
2. Core Layer
3. Resource Management Layer
4. Render Frontend and Backend; Audio; Other systems

#### Platform Layer

This layer abstracts away the specific platform on which the game/engine is to be run. Therefore it will focus on platform specific operations such as: 

- OS API calls
- Console Output
- File IO
- Memory
- Render API Extensions

#### Core Layer

This layer provides the basic required functionality for the game engine. This will focus on datastructures as well as fundamental engine algorithms and features:

- Logger
- Assertions
- Data Structures
- Memory Allocations
- Math Library
- Parsers (XML, CSV, JSON, etc)
- Engine Configuration
- Profiling
- Async File IO
- Localisation
- String Lib
- RNG

#### Resource Management Layer

This layer focuses on assets read from sources; examples include:

- Images
- Materials
- Meshes
- Animations
- World Maps


### Supported Platforms

- Windows
- Linux