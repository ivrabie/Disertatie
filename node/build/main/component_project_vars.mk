# Automatically generated build file. Do not edit.
COMPONENT_INCLUDES += $(PROJECT_PATH)/main/include $(PROJECT_PATH)/ble $(PROJECT_PATH)/nvs_adapter $(PROJECT_PATH)/SWL_Gateway/FlashDevice $(PROJECT_PATH)/ble/advertising $(PROJECT_PATH)/SWLoad $(PROJECT_PATH)/ble/ble_device $(PROJECT_PATH)/ble/gatt_client $(PROJECT_PATH)/ble/gatt_service $(PROJECT_PATH)/SWLoad/FlashDeviceManager $(PROJECT_PATH)/ble/gatt_server $(PROJECT_PATH)/SWLoad/FlashingService $(PROJECT_PATH)/etl
COMPONENT_LDFLAGS += -L$(BUILD_DIR_BASE)/main -lstdc++ -lmain
COMPONENT_LINKER_DEPS += 
COMPONENT_SUBMODULES += 
COMPONENT_LIBRARIES += main
COMPONENT_LDFRAGMENTS += 
component-main-build: 
