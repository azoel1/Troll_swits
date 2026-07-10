ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)
include $(DEVKITPRO)/libnx/switch_rules

TARGET := rickrolled
BUILD := build
SOURCES := source
DATA := data
INCLUDES := include

EXEFS_SRC := exefs

APP_TITLE := RICKROLLED
APP_AUTHOR := User
APP_VERSION := 1.0.0

include $(DEVKITPRO)/devkitA64/base_rules
