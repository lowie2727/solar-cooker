# Arduino

The [**src**](src) folder contains the code for both the individual components and the code for the entire test station.

The folder [**src/main**](src/main) contains the various components brought together. Each component has its own cpp file. It is joined via header files in [main.cpp](src/main/main.cpp).

In the [**src/components**](src/components) subfolder, you will find code for each individual component, along with the connection diagram.

## PlatformIO

We migrated from the [Arduino IDE](https://www.arduino.cc/en/software) to [PlatformIO](https://platformio.org/platformio-ide) because of the size of the project.

### Prerequisites

#### Visual Studio Code

To use PlatformIO u need to first install [Visual Studio Code](https://code.visualstudio.com/)

When VSCode is installed you need to install the [PlatformIO extension](https://platformio.org/install/ide?install=vscode) in VSCode and then you are ready to compile.

### Using PlatformIO

#### Opening the PlatformIO project

When the extension is successfully installed, you will get a new icon on the left in VSCode. Click on it and open the correct folder (open the folder with the platform.ini file). In this case open the [arduino](/arduino) folder.

#### Build main

You will now see several project tasks, open main and click on build. The terminal will open, if everything worked out it should show SUCCESS when it is done.

#### Upload and Monitor

To upload the code and view the serial monitor, a button is provided.
