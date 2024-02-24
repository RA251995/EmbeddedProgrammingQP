UV4_PROJ := bluepill-stm32f1.uvprojx
BUILD_DIR := .\build_bluepill-stm32f1
BIN_FILE := test.bin

.PHONY : build flash clean

all: build flash reset

build:
	@echo === Building $(UV4_PROJ)...
	UV4 -r $(UV4_PROJ)

flash:
	@echo === Flashing...
	ST-LINK_CLI -ME -P $(BUILD_DIR)\$(BIN_FILE) 0x08000000
	
reset:
	@echo === Resetting...
	ST-LINK_CLI -Rst

clean:
	@echo === Deleting build directory...
	rm -r build_bluepill-stm32f1
