# fit-mqtt-explorer

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
