#
# Copyright (c) 2022 jdeokkim
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

.PHONY: all clean

_COLOR_BEGIN := $(shell tput setaf 16)
_COLOR_END := $(shell tput sgr0)

PROJECT_NAME := algoitni
PROJECT_FULL_NAME := jdeokkim/algoitni

PROJECT_PATH := ../$(PROJECT_NAME)
PROJECT_PREFIX := $(_COLOR_BEGIN)$(PROJECT_FULL_NAME):$(_COLOR_END)

BINARY_PATH := bin
SOURCE_PATH := .

SOURCES := $(wildcard $(SOURCE_PATH)/*.c)
TARGETS := $(patsubst $(SOURCE_PATH)/%.c,$(BINARY_PATH)/%.out,$(SOURCES))

HOST_PLATFORM := LINUX

ifeq ($(OS),Windows_NT)
	PROJECT_PREFIX := $(PROJECT_NAME):
	HOST_PLATFORM := WINDOWS
else
	UNAME = $(shell uname)

	ifeq ($(UNAME),Linux)
		HOST_PLATFORM = LINUX
	endif
endif

CC := gcc
CFLAGS := -D_DEFAULT_SOURCE -g $(INCLUDE_PATH:%=-I%) -O2 -std=c99

PLATFORM := $(HOST_PLATFORM)

ifeq ($(PLATFORM),WINDOWS)
	TARGETS := $(EXAMPLES:%=$(BINARY_PATH)/%.exe)

	ifneq ($(HOST_PLATFORM),WINDOWS)
		CC := x86_64-w64-mingw32-gcc
	endif
endif

all: pre-build build post-build

pre-build:
	@echo "$(PROJECT_PREFIX) Using: '$(CC)' to build all examples."
    
build: $(TARGETS)

$(BINARY_PATH)/%.out: $(SOURCE_PATH)/%.c
	@mkdir -p $(BINARY_PATH)
	@echo "$(PROJECT_PREFIX) Compiling: $@ (from $<)"
	@$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
    
$(BINARY_PATH)/%.exe: $(SOURCE_PATH)/%.c
	@mkdir -p $(BINARY_PATH)
	@echo "$(PROJECT_PREFIX) Compiling: $@ (from $<)"
	@$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
    
post-build:
	@echo "$(PROJECT_PREFIX) Build complete."

clean:
	@echo "$(PROJECT_PREFIX) Cleaning up."
	@rm -rf $(BINARY_PATH)/*.out
	@rm -rf $(BINARY_PATH)/*.exe
	@rm -rf $(BINARY_PATH)/*.o