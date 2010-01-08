MK_ENV_NAME := $(lastword $(MAKEFILE_LIST))

$(info MK_ENV_NAME = $(MK_ENV_NAME))

MK_ENV_DIR := $(dir $(MK_ENV_NAME))

$(info MK_ENV_DIR = $(MK_ENV_DIR))

ROOT_DIR := $(abspath $(PWD)/$(MK_ENV_DIR)..)
						
$(info ROOT_DIR = $(ROOT_DIR))

