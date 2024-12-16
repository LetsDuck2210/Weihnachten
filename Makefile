FLAGS =
PORT = /dev/ttyUSB0
BAUDRATE=115200

all: compile upload monitor

compile:
	arduino-cli compile --build-property build.extra_flags=$(FLAGS)

upload:
	arduino-cli upload -p $(PORT)

monitor:
	arduino-cli monitor -c baudrate=$(BAUDRATE) -p $(PORT)
