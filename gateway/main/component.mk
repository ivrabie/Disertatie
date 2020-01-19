#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)


#sources
COMPONENT_SRCDIRS += ../wifi/
COMPONENT_SRCDIRS += ../ble/
COMPONENT_SRCDIRS += ../nvs_adapter/
COMPONENT_SRCDIRS += ../SWL_Gateway/FlashDevice/
COMPONENT_SRCDIRS += ../ble/advertising/
COMPONENT_SRCDIRS += ../SWL_Gateway/
COMPONENT_SRCDIRS += ../ble/ble_device/
COMPONENT_SRCDIRS += ../ble/gatt_client/
COMPONENT_SRCDIRS += ../ble/gatt_service/
COMPONENT_SRCDIRS += ../SWL_Gateway/FlashDeviceManager/
COMPONENT_SRCDIRS += ../SWL_Gateway/FlashingService/
COMPONENT_SRCDIRS += ../etl/
COMPONENT_SRCDIRS += ../HttpClient/
# COMPONENT_SRCDIRS += ../etl/atomic/
# COMPONENT_SRCDIRS += ../etl/c/
# COMPONENT_SRCDIRS += ../etl/deprecated/
# COMPONENT_SRCDIRS += ../etl/experimental/
# COMPONENT_SRCDIRS += ../etl/mutex/
# COMPONENT_SRCDIRS += ../etl/private/
# COMPONENT_SRCDIRS += ../etl/profiles/
# COMPONENT_SRCDIRS += ../etl/stl/
#includes
COMPONENT_ADD_INCLUDEDIRS += ../wifi/
COMPONENT_ADD_INCLUDEDIRS += ../ble/
COMPONENT_ADD_INCLUDEDIRS += ../nvs_adapter/
COMPONENT_ADD_INCLUDEDIRS += ../SWL_Gateway/FlashDevice/
COMPONENT_ADD_INCLUDEDIRS += ../ble/advertising/
COMPONENT_ADD_INCLUDEDIRS += ../SWL_Gateway/
COMPONENT_ADD_INCLUDEDIRS += ../ble/ble_device/
COMPONENT_ADD_INCLUDEDIRS += ../ble/gatt_client/
COMPONENT_ADD_INCLUDEDIRS += ../ble/gatt_service/
COMPONENT_ADD_INCLUDEDIRS += ../SWL_Gateway/FlashDeviceManager/
COMPONENT_ADD_INCLUDEDIRS += ../SWL_Gateway/FlashingService/
COMPONENT_ADD_INCLUDEDIRS += ../etl/
COMPONENT_ADD_INCLUDEDIRS += ../HttpClient/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/atomic/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/c/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/deprecated/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/experimental/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/mutex/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/private/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/profiles/
# COMPONENT_ADD_INCLUDEDIRS += ../etl/stl/
COMPONENT_ADD_LDFLAGS=-lstdc++ -l$(COMPONENT_NAME)
CFLAGS += -std=c++17

