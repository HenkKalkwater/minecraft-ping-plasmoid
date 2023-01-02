# Minecraft Server Ping Plasmoid

An Plasmoid to show the status of a Minecraft server.

## Screenshots
![Screenshot showing the plasmoid in a connected state, displaying a server
icon, server name, amount of players online and the message of the day of
a Minecraft server](work-assets/screenshots/screenshot-connected.png)
![Screenshot sowing the plasmoid displaying an error](work-assets/screenshots/screenshot-error.png)

## Building
This should be the standard CMake build procedure:

**Note:** this will usually install the plugin to the default prefix, usually `/usr/local/`. Qt may
not load plugins from this path. Run `cmake ..` with the option `-DCMAKE_INSTALL_PREFIX=/usr` to 
install to `/usr`, where Qt will find it. This may interfere with your system package manager!

```bash
mkdir build && cd build
cmake ..
make
sudo make install
```


### Required dependencies
- KDE extra-cmake-modules
- KDE Frameworks 5
- Qt5 Core
- Qt5 QML
- Qt5 Quick
