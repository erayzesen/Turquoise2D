![logo](https://img.itch.zone/aW1hZ2UvNjcwMzAvMzA0MTM2LnBuZw==/original/RlHVlm.png)

# What is the Turquoise 2D project?

Turquoise 2D is an open source level/game editor software aimed for developing games using many game frameworks (currently Cocos2d-x is the only supported framework). 

If you want to make your own game editor to any game framework,  maybe this might be a good start. Improve it and help its development!

# What is the purpose of the project?
Turquoise2D allows you to design levels for your video game and create user interfaces. 

Turquoise2D currently supports Only Cocos2d-x, you're welcome to contribute and add support for other game engines.

# At which stage is the project now?
Currently, there are many things that are missing and need to be improved; for instance, it only supports Cocos2d-x projects. Maybe you could integrate other frameworks or add new features to it.

Qt 5+ is also currently incompatible. Many functions used are deprecated and must be updated. For example, QRegExp is deprecated since Qt 5.

# Which UI library used?
It's written using Qt Widgets. We chose Qt because it's simple, fast, and cross platform.

# License
Turquoise2D licensed with Zlib license, seen [here.](https://github.com/erayzesen/Turquoise2D/blob/master/licence) 

# How does it work?

[![How It Works](https://img.youtube.com/vi/76ieIQAVkjw/0.jpg)](https://www.youtube.com/watch?v=76ieIQAVkjw)

[Click here to watch on Youtube!](https://www.youtube.com/watch?v=76ieIQAVkjw)

# Features

* Create tile based game levels.
* Sprite-Object drawing system with the draw and the eyedropper tool.
* Simple physics editor (Currently supports box and circle collider shapes).
* Direct game object integration with the framework.
* Spritesheet support.
* Simple and useful development structure.
* You won't need to learn any more framework information except the game library.
* You can do easy compile, test-run process with a few buttons.


![gif_01](https://img.itch.zone/aW1hZ2UvNjcwMzAvMzA0MTQ4LmdpZg==/347x500/g4lPtF.gif)
![gif_02](https://img.itch.zone/aW1hZ2UvNjcwMzAvMzA0MTQ2LmdpZg==/347x500/VBaSOK.gif)
![gif_03](https://img.itch.zone/aW1hZ2UvNjcwMzAvMzA0MTQ3LmdpZg==/347x500/egnBfK.gif)
![gif_04](https://img.itch.zone/aW1hZ2UvNjcwMzAvMzA0MTQ5LmdpZg==/347x500/gnzO1C.gif)

# Requirements
* Qt 4+
* Qt 5 Core Compability (`QT += core5compat`)

# Building
```
cd Source 
qmake Turquoise_2D_Studio.pro
```