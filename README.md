# FIT MQTT Client

Authors: Ondřej Míchal <xmicha80>, Žaneta Grossová <xgross11>

## Build requirements

- CMake (>=3.5)
- C++ compiler (C++17 compliant)
- Qt (>=5.9) - Core, Widgets, Svg
- Eclipse Paho MQTT C Client Library
- Eclipse Paho MQTT C++ Client Library

## State of the project

### Explorer

**Completed features:**

- Connecting to MQTT broker (subscribing to all topics)
- Tree view of topics
- History of messages
- Max limit of messages
- Simple preview of messages
- Sending messages (plain text & files)
- Sending messages to subtopics
- Visual difference between sent and received messages
- Creating snapshots of current state of topics

**Missing features:**

- Image recognition
- Image previews

### Dashboard

**Completed features:**

- Ability to create simple widgets
- Subscribing widgets to different topics

**Missing features:**

- Functional widget types (for different types of payloads - e.g., temps, humidity,..)
- Saving dashboard setup to a configuration file
- Loading dashboard setup from a configuration file
- Advanced presentation of data in widgets
- Sending payloads via widgets

### Simulator

**Completed features:**

- Sending messages at a set interval, sending premade messages
- Loading simulator setup from a configuration file

**Missing features:**

- Understanding received data (e.g. setpoints)
- Sending binary data (e.g. messages)

## Simulation of traffic

The application has a mode for simulating traffic. The mode can be switched to
by clicking an appropriate icon (with tooltip "Simulator") in the application's
toolbar.

Before a simulation can be started, it has to be configured first. A 
configuration file is a JSON file with specific data defined in it. All data
has to be correct. If it is not, the simulator will simply not load it.

### Simulation config file structure

The root element is an object with two keys:

- `topic` <`string`> - the root topic where all messages will be sent
- `devices` <`array`> - list of simulated devices

The `devices` array can hold various number of simulated devices.

Each device is an object with four keys:

- `topic` <`string`> - subtopic where this devices sends messages
- `type` <`string`> - type of the device
- `period` <`int`> - timeout between sending messages [ms]
- `payloads` <`array`> - list of messages to be sent

The `payloads` array can hold various number of payloads. Only paylods of type
<`string`> have been tested but other types should also be supported.

Example configuration files can be found in [`data/simulations`](https://github.com/HarryMichal/fit-mqtt-explorer/tree/main/data/simulations).

## Attributions

- The Qt Documentation
- Doxygen theme: https://jothepro.github.io/doxygen-awesome-css/
