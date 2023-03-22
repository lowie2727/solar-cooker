# Arduino

In each subfolder, you will find code for each individual component, along with the connection diagram.

The folder [**main**](main) contains the various components brought together.

## Contributing

### Arduino IDE
You can contribute using the [Arduino IDE](https://www.arduino.cc/en/software)

### Arduino CLI (advanced)

For advanced users who like the command line, there is also the [Arduino CLI](https://arduino.github.io/arduino-cli/0.31/installation/).

To use the Arduino CLI, run the following commands in the appropriate directory.

Check if your board has been detected by your PC.

```bash
arduino-cli board list
```

Delete the appropriate line from the comments and change the number if necessary.

```yaml
# Windows
# default_port: COM0
# Linux
# default_port: /dev/ttyACM0
```

Compile and upload to the board.

```bash
arduino-cli compile --profile mega && arduino-cli upload --profile mega
```


