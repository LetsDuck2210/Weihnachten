AVAILABLE_FLAGS := "SINGLE_BUTTON USE_PULLUP=0"

ifndef PORT
	PORT = /dev/ttyUSB0
endif
ifndef FLAGS
	FLAGS =
endif
BAUDRATE = 115200
JOBS=$(shell nproc)

all: compile upload monitor

compile:
	arduino-cli compile -j $(JOBS) --build-property build.extra_flags=$(FLAGS)

upload:
	arduino-cli upload -p $(PORT)

monitor:
	arduino-cli monitor -c baudrate=$(BAUDRATE) -p $(PORT)


select_port:
	@echo "export PORT=$(shell arduino-cli board list \
		| grep -vE 'Board Name FQBN Core|No board found| *|/dev/ttyS0' \
		| fzf -0 --header='Select your board' \
		| cut -d' ' -f1 \
	)"
select_flags:
	@echo "export FLAGS='$(shell tr ' ' '\n' <<< $(AVAILABLE_FLAGS) | fzf -0 -m -d' ' --header='Select optional flags' | sed 's/[^ ]*/-D&/g')'"


configure: select_port select_flags
