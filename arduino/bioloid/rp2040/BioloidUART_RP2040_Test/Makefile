# This file contains useful rules for working with Microsoft's VSCode

ARDUINO_CLI = arduino-cli

# override the PROFILE from sketch.yml
PROFILE ?= picow

VID ?= 0x2e8a
PID_picow = 0xf00a
PID_pico  = 0x000a
PID_zero  = 0x0003
PID ?= $(PID_$(PROFILE))

# NOTE: The arduino-cli buffers characters typed until a newline is entered
#		so we use python's miniterm instead which sends each character as it's
#		typed.

.PHONY: compile
compile:
	@echo Compiling with PROFILE = $(PROFILE)
	$(ARDUINO_CLI) compile --profile $(PROFILE)

# Try detecting the RPi Pico via the serial port. If no serial port found,
# look for the RPi Pico mounted as a filesystem.
.PHONY: upload
upload: PORT = $(shell find_port.py --vid $(VID) --pid $(PID))
upload: RPI_DIR = /media/$(USER)/RPI-RP2
upload:
	@if [ -z "$(PORT)" ]; then \
		if [ -d "${RPI_DIR}" ]; then \
			echo Compiling with PROFILE = $(PROFILE); \
			$(ARDUINO_CLI) compile --profile $(PROFILE) --upload --port $(RPI_DIR) && \
			python3 -m serial.tools.miniterm $(PORT) 115200; \
		else \
			echo "No RPi Pico found"; \
		fi \
	else \
		echo Compiling with PROFILE = $(PROFILE); \
		$(ARDUINO_CLI) compile  --profile $(PROFILE) --upload --port $(PORT) &&  \
		python3 -m serial.tools.miniterm $(PORT) 115200; \
	fi

# Fires up a terminal monitor to interact with the rp2040 over USB serial
.PHONY: momitor
monitor: PORT = $(shell find_port.py --vid $(VID) --pid $(PID))
monitor:
	@if [ -z "$(PORT)" ]; then \
		echo "No RPi found with USB ID $(VID):$(PID)"; \
		find_port.py -l; \
		exit 1; \
	fi
	python3 -m serial.tools.miniterm $(PORT) 115200;

# Installs arduino-cli into ~/bin and does the setup for the rp2040
.PHONY: install-cli
install-cli:
	cd ${HOME}; curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	$(ARDUINO_CLI) config init
	$(ARDUINO_CLI) config add board_manager.additional_urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
	$(ARDUINO_CLI) core update-index
	$(ARDUINO_CLI) core install rp2040:rp2040

# vscode-settings will add the INCLUDE_DIRS into the .vscode settings file,
# creating it if needed. It will also set the path to the compiler so that
# the correct system headers get included when using a cross compiler.
.PHONY: vscode-settings
vscode-settings: MK_SETTINGS = ./.vscode/make_settings.py
vscode-settings: VSCODE_SETTINGS = ./.vscode/settings.json
vscode-settings: COMPILE_CMD = $(shell $(ARDUINO_CLI) compile --verbose 2> /dev/null | grep g++ | head -1)
vscode-settings: INC_DIRS = $(patsubst -I%,%,$(sort $(filter -I%, $(COMPILE_CMD))))
vscode-settings: DEFS = $(patsubst -D%,%,$(sort $(filter -D%, $(COMPILE_CMD))))
vscode-settings: IPREFIX = $(patsubst -iprefix%,%,$(filter -iprefix%, $(COMPILE_CMD)))
vscode-settings: PLATFORM_INCS = $(patsubst -iwithprefixbefore/%,$(IPREFIX)%,$(shell cat $(patsubst @%,%,$(filter @%,$(COMPILE_CMD))) /dev/null))
vscode-settings:
	@echo "Updating ${VSCODE_SETTINGS} ..."
	@# We generate a python dictionary (which allows trailing commas) and then
	@# convert the dictionary to json
	@if [ ! -f "${VSCODE_SETTINGS}" ]; then \
		mkdir -p $(dir ${VSCODE_SETTINGS}); \
		(echo '{}' > ${VSCODE_SETTINGS}); \
	fi
	@echo 'import json' > ${MK_SETTINGS}
	@echo 'd = json.loads(open("${VSCODE_SETTINGS}").read())' >> ${MK_SETTINGS}
	@echo 'd["C_Cpp.default.includePath"] = [' >> ${MK_SETTINGS}
	@for dir in $(INC_DIRS) $(PLATFORM_INCS); do \
		if [ "$${dir#/}" != "$${dir}" ]; then \
		    echo '        "'$${dir}'",' >> ${MK_SETTINGS}; \
		else \
			echo '        "$${workspaceFolder}/'$${dir}'",' >> ${MK_SETTINGS}; \
		fi \
	done
	@echo ']' >> ${MK_SETTINGS}
	@echo 'd["C_Cpp.errorSquiggles"] = "enabled"' >> ${MK_SETTINGS}
	@echo 'd["C_Cpp.default.compilerPath"] = "$(firstword $(COMPILE_CMD))"' >> ${MK_SETTINGS}
	@echo 'd["C_Cpp.default.defines"] = [' >> ${MK_SETTINGS}
	@for cflag in $(DEFS); do \
		echo '        "'$${cflag}'",' >> ${MK_SETTINGS}; \
	done
	@echo ']' >> ${MK_SETTINGS}
	@echo 'd["C_Cpp.default.cppStandard"] = "c++11"' >> ${MK_SETTINGS}
	@echo 'print(json.dumps(d, indent=4))' >> ${MK_SETTINGS}
	@python3 ${MK_SETTINGS} > ${VSCODE_SETTINGS}.new
	@# We do a mv after so we don't wipe out the settings if the python script has an error
	@mv ${VSCODE_SETTINGS}.new ${VSCODE_SETTINGS}
	@echo "===== Updated ${VSCODE_SETTINGS} ====="
	python3 -m json.tool ${VSCODE_SETTINGS}

# Reformats the source code for a consistent look

CLANGFORMAT_ARGS := -style='{BasedOnStyle: chromium, IndentWidth: 4, AccessModifierOffset: -3, AlignAfterOpenBracket: AlwaysBreak, BinPackParameters: false, ColumnLimit: 100, SortIncludes: false}'
STYLE_FILES = $(wildcard ./*.h) $(wildcard ./*.cpp) $(wildcard ./*.ino)

.PHONY: style
style:
	@echo "Stylizing source files ..."
	@clang-format --verbose $(CLANGFORMAT_ARGS) -i $(STYLE_FILES)

CPPLINT_ARGS :=

# Find common issues with the code

.PHONY: lint
lint:
	@echo "Linting source files ..."
	@cpplint --linelength=100 --filter=-build/include_subdir,-readability/casting,-whitespace/parens --recursive $(CPPLINT_ARGS) --headers=h  --extensions=c,h,cpp,ino .
