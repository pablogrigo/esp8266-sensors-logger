# MQTT + ESP8266 + DHT11 Sensor Logger

A proof of concept using an ESP8266 board with a DHT11 sensor to log readings into a MQTT server connected over WiFi.

## TODO

- [x] Read sensors and print to serial. 
- [x] Connect board to WiFi.
- [x] Publish readings to a local MQTT server.
- [x] Power the board using an external battery.
- [ ] Publish readings to AWS IoT Core + Kinesis Data Firehose + S3.
- [ ] Provision AWS infrastructure using Terraform.
- [ ] Use deep sleep support to extend battery life.
- [ ] Configure QoS.

## Architecture

```
[DHT11] ---wire--- [ESP8266] ---mqtt-over-wifi--- [Mosquitto]
```

## Network communications

MQTT is used to publish data from each sensor into independent topics. 

> The MQTT protocol provides a lightweight method of carrying out messaging using a publish/subscribe model. 
This makes it suitable for Internet of Things messaging such as with low power sensors or mobile devices such as phones, 
embedded computers or microcontrollers. 

## Local development

### Hardware

- ESP8266 WiFi Mini - Arduino compatible
- DHT11 sensor
- Breadboard
- Jumper wires
- USB A to Micro USB cable
- 2700mWh power pack

### DHT11 specs

| Sensor      | Range             | Resolution    |
| ----------- | :---------------: | :-----------: |
| Temperature | 0.00°C to 50.00°C | 2.00°C        |
| Humidity    | 20.00% to 80.00%  | 5.00%         |

### How to compile the Arduino sketch

1. Copy the file `vars.h.example` to `vars.h`.
1. Configure your environment in `vars.h`.
1. Compile and upload to an ESP8266 board.

### How to run the MQTT server

1. `$ cd server-local`
1. `$ docker-compose up`

### How to inspect MQTT messages

1. Download and install a MQTT explorer app. e.g. https://mqtt-explorer.com/
1. Alternatively use `mosquitto_sub` to subscribe to all topics using the console: 
```
$ cd server-local
$ docker-compose exec mosquitto /bin/sh
# mosquitto_sub -F '@Y-@m-@dT@H:@M:@S@z | %t | %p' -t '#'

2021-05-15T04:44:30+0000 | esp8266/temperature | 21.30
2021-05-15T04:44:30+0000 | esp8266/humidity | 64.00
...
```

## Resources

https://en.wikipedia.org/wiki/ESP8266

https://docs.oasis-open.org/mqtt/mqtt/v5.0/mqtt-v5.0.html

https://mosquitto.org/documentation/

https://aws.amazon.com/iot-core/

https://aws.amazon.com/kinesis/data-firehose/
