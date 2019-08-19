#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)


#sources
COMPONENT_SRCDIRS += ../ble/
COMPONENT_SRCDIRS += ../nvs_adapter/
COMPONENT_SRCDIRS += ../SWLoad/FlashDevice/
COMPONENT_SRCDIRS += ../ble/advertising/
COMPONENT_SRCDIRS += ../SWLoad/
COMPONENT_SRCDIRS += ../ble/ble_device/
COMPONENT_SRCDIRS += ../ble/gatt_client/
COMPONENT_SRCDIRS += ../ble/gatt_service/
COMPONENT_SRCDIRS += ../SWLoad/FlashDeviceManager/
COMPONENT_SRCDIRS += ../ble/gatt_server/
COMPONENT_SRCDIRS += ../SWLoad/FlashingService/
COMPONENT_SRCDIRS += ../etl/

#includes
COMPONENT_ADD_INCLUDEDIRS += ../ble/
COMPONENT_ADD_INCLUDEDIRS += ../nvs_adapter/
COMPONENT_ADD_INCLUDEDIRS += ../SWL_Gateway/FlashDevice/
COMPONENT_ADD_INCLUDEDIRS += ../ble/advertising/
COMPONENT_ADD_INCLUDEDIRS += ../SWLoad/
COMPONENT_ADD_INCLUDEDIRS += ../ble/ble_device/
COMPONENT_ADD_INCLUDEDIRS += ../ble/gatt_client/
COMPONENT_ADD_INCLUDEDIRS += ../ble/gatt_service/
COMPONENT_ADD_INCLUDEDIRS += ../SWLoad/FlashDeviceManager/
COMPONENT_ADD_INCLUDEDIRS += ../ble/gatt_server/
COMPONENT_ADD_INCLUDEDIRS += ../SWLoad/FlashingService/
COMPONENT_ADD_INCLUDEDIRS += ../etl/

COMPONENT_ADD_LDFLAGS=-lstdc++ -l$(COMPONENT_NAME)
CFLAGS += -std=c++17
