#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := lvgl-demo
include $(IDF_PATH)/make/project.mk
COMPONENT_ADD_INCLUDEDIRS = components/AHT20
COMPONENT_SRCDIRS = components/AHT20
