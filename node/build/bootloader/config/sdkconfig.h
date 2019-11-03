/*
 * Automatically generated file. DO NOT EDIT.
 * Espressif IoT Development Framework (ESP-IDF) Configuration Header
 */
#pragma once
#define CONFIG_IDF_TARGET_ESP32 1
#define CONFIG_IDF_CMAKE 1
#define CONFIG_IDF_TARGET "esp32"
#define CONFIG_IDF_FIRMWARE_CHIP_ID 0x0000
#define CONFIG_SDK_TOOLPREFIX "xtensa-esp32-elf-"
#define CONFIG_APP_BUILD_TYPE_APP_2NDBOOT 1
#define CONFIG_APP_BUILD_GENERATE_BINARIES 1
#define CONFIG_APP_BUILD_BOOTLOADER 1
#define CONFIG_APP_BUILD_USE_FLASH_SECTIONS 1
#define CONFIG_BOOTLOADER_LOG_LEVEL_INFO 1
#define CONFIG_BOOTLOADER_LOG_LEVEL 3
#define CONFIG_BOOTLOADER_VDDSDIO_BOOST_1_9V 1
#define CONFIG_BOOTLOADER_WDT_ENABLE 1
#define CONFIG_BOOTLOADER_WDT_TIME_MS 9000
#define CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE 1
#define CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK 1
#define CONFIG_BOOTLOADER_APP_SECURE_VERSION 0
#define CONFIG_BOOTLOADER_APP_SEC_VER_SIZE_EFUSE_FIELD 32
#define CONFIG_BOOTLOADER_RESERVE_RTC_SIZE 0x0
#define CONFIG_ESPTOOLPY_BAUD_OTHER_VAL 115200
#define CONFIG_ESPTOOLPY_FLASHMODE_DIO 1
#define CONFIG_ESPTOOLPY_FLASHMODE "dio"
#define CONFIG_ESPTOOLPY_FLASHFREQ_40M 1
#define CONFIG_ESPTOOLPY_FLASHFREQ "40m"
#define CONFIG_ESPTOOLPY_FLASHSIZE_4MB 1
#define CONFIG_ESPTOOLPY_FLASHSIZE "4MB"
#define CONFIG_ESPTOOLPY_FLASHSIZE_DETECT 1
#define CONFIG_ESPTOOLPY_BEFORE_RESET 1
#define CONFIG_ESPTOOLPY_BEFORE "default_reset"
#define CONFIG_ESPTOOLPY_AFTER_RESET 1
#define CONFIG_ESPTOOLPY_AFTER "hard_reset"
#define CONFIG_ESPTOOLPY_MONITOR_BAUD_115200B 1
#define CONFIG_ESPTOOLPY_MONITOR_BAUD_OTHER_VAL 115200
#define CONFIG_ESPTOOLPY_MONITOR_BAUD 115200
#define CONFIG_PARTITION_TABLE_CUSTOM 1
#define CONFIG_PARTITION_TABLE_CUSTOM_FILENAME "partitions.csv"
#define CONFIG_PARTITION_TABLE_FILENAME "partitions.csv"
#define CONFIG_PARTITION_TABLE_OFFSET 0x8000
#define CONFIG_COMPILER_OPTIMIZATION_SIZE 1
#define CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_ENABLE 1
#define CONFIG_COMPILER_STACK_CHECK_MODE_NONE 1
#define CONFIG_EFUSE_CODE_SCHEME_COMPAT_3_4 1
#define CONFIG_EFUSE_MAX_BLK_LEN 192
#define CONFIG_ESP32_REV_MIN_0 1
#define CONFIG_ESP32_REV_MIN 0
#define CONFIG_ESP32_DPORT_WORKAROUND 1
#define CONFIG_ESP32_DEFAULT_CPU_FREQ_240 1
#define CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ 240
#define CONFIG_ESP32_TRACEMEM_RESERVE_DRAM 0x0
#define CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES_FOUR 1
#define CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES 4
#define CONFIG_ESP32_ULP_COPROC_RESERVE_MEM 0
#define CONFIG_ESP32_PANIC_PRINT_REBOOT 1
#define CONFIG_ESP32_DEBUG_OCDAWARE 1
#define CONFIG_ESP32_DEBUG_STUBS_ENABLE 1
#define CONFIG_ESP32_BROWNOUT_DET 1
#define CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_0 1
#define CONFIG_ESP32_BROWNOUT_DET_LVL 0
#define CONFIG_ESP32_REDUCE_PHY_TX_POWER 1
#define CONFIG_ESP32_TIME_SYSCALL_USE_RTC_FRC1 1
#define CONFIG_ESP32_RTC_CLK_SRC_INT_RC 1
#define CONFIG_ESP32_RTC_CLK_CAL_CYCLES 1024
#define CONFIG_ESP32_DEEP_SLEEP_WAKEUP_DELAY 2000
#define CONFIG_ESP32_XTAL_FREQ_40 1
#define CONFIG_ESP32_XTAL_FREQ 40
#define CONFIG_ESP32_DPORT_DIS_INTERRUPT_LVL 5
#define CONFIG_ESP_ERR_TO_NAME_LOOKUP 1
#define CONFIG_ESP_SYSTEM_EVENT_QUEUE_SIZE 32
#define CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE 2304
#define CONFIG_ESP_MAIN_TASK_STACK_SIZE 3584
#define CONFIG_ESP_IPC_TASK_STACK_SIZE 1024
#define CONFIG_ESP_TIMER_TASK_STACK_SIZE 3584
#define CONFIG_ESP_CONSOLE_UART_DEFAULT 1
#define CONFIG_ESP_CONSOLE_UART_NUM 0
#define CONFIG_ESP_CONSOLE_UART_BAUDRATE 115200
#define CONFIG_ESP_INT_WDT 1
#define CONFIG_ESP_INT_WDT_TIMEOUT_MS 300
#define CONFIG_ESP_INT_WDT_CHECK_CPU1 1
#define CONFIG_ESP_TASK_WDT 1
#define CONFIG_ESP_TASK_WDT_TIMEOUT_S 5
#define CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0 1
#define CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1 1
#define CONFIG_LOG_DEFAULT_LEVEL_INFO 1
#define CONFIG_LOG_DEFAULT_LEVEL 3
#define CONFIG_LOG_COLORS 1
#define CONFIG_LOG_TIMESTAMP_SOURCE_RTOS 1
#define CONFIG_SPI_FLASH_ROM_DRIVER_PATCH 1
#define CONFIG_SPI_FLASH_DANGEROUS_WRITE_ABORTS 1
#define CONFIG_SPI_FLASH_SUPPORT_ISSI_CHIP 1
#define CONFIG_SPI_FLASH_SUPPORT_GD_CHIP 1

/* List of deprecated options */
#ifdef CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
#define CONFIG_APP_ANTI_ROLLBACK CONFIG_BOOTLOADER_APP_ANTI_ROLLBACK
#endif

#ifdef CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE
#define CONFIG_APP_ROLLBACK_ENABLE CONFIG_BOOTLOADER_APP_ROLLBACK_ENABLE
#endif

#ifdef CONFIG_BOOTLOADER_APP_SECURE_VERSION
#define CONFIG_APP_SECURE_VERSION CONFIG_BOOTLOADER_APP_SECURE_VERSION
#endif

#ifdef CONFIG_BOOTLOADER_APP_SEC_VER_SIZE_EFUSE_FIELD
#define CONFIG_APP_SECURE_VERSION_SIZE_EFUSE_FIELD CONFIG_BOOTLOADER_APP_SEC_VER_SIZE_EFUSE_FIELD
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET
#define CONFIG_BROWNOUT_DET CONFIG_ESP32_BROWNOUT_DET
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL
#define CONFIG_BROWNOUT_DET_LVL CONFIG_ESP32_BROWNOUT_DET_LVL
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL
#define CONFIG_BROWNOUT_DET_LVL_SEL CONFIG_ESP32_BROWNOUT_DET_LVL_SEL
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_0
#define CONFIG_BROWNOUT_DET_LVL_SEL_0 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_0
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_1
#define CONFIG_BROWNOUT_DET_LVL_SEL_1 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_1
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_2
#define CONFIG_BROWNOUT_DET_LVL_SEL_2 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_2
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_3
#define CONFIG_BROWNOUT_DET_LVL_SEL_3 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_3
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_4
#define CONFIG_BROWNOUT_DET_LVL_SEL_4 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_4
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_5
#define CONFIG_BROWNOUT_DET_LVL_SEL_5 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_5
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_6
#define CONFIG_BROWNOUT_DET_LVL_SEL_6 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_6
#endif

#ifdef CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_7
#define CONFIG_BROWNOUT_DET_LVL_SEL_7 CONFIG_ESP32_BROWNOUT_DET_LVL_SEL_7
#endif

#ifdef CONFIG_ESP32_COMPATIBLE_PRE_V2_1_BOOTLOADERS
#define CONFIG_COMPATIBLE_PRE_V2_1_BOOTLOADERS CONFIG_ESP32_COMPATIBLE_PRE_V2_1_BOOTLOADERS
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_DEFAULT
#define CONFIG_COMPILER_OPTIMIZATION_LEVEL_DEBUG CONFIG_COMPILER_OPTIMIZATION_DEFAULT
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_SIZE
#define CONFIG_COMPILER_OPTIMIZATION_LEVEL_RELEASE CONFIG_COMPILER_OPTIMIZATION_SIZE
#endif

#ifdef CONFIG_ESP_CONSOLE_UART
#define CONFIG_CONSOLE_UART CONFIG_ESP_CONSOLE_UART
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_BAUDRATE
#define CONFIG_CONSOLE_UART_BAUDRATE CONFIG_ESP_CONSOLE_UART_BAUDRATE
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_CUSTOM
#define CONFIG_CONSOLE_UART_CUSTOM CONFIG_ESP_CONSOLE_UART_CUSTOM
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_CUSTOM_NUM_0
#define CONFIG_CONSOLE_UART_CUSTOM_NUM_0 CONFIG_ESP_CONSOLE_UART_CUSTOM_NUM_0
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_CUSTOM_NUM_1
#define CONFIG_CONSOLE_UART_CUSTOM_NUM_1 CONFIG_ESP_CONSOLE_UART_CUSTOM_NUM_1
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_DEFAULT
#define CONFIG_CONSOLE_UART_DEFAULT CONFIG_ESP_CONSOLE_UART_DEFAULT
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_NONE
#define CONFIG_CONSOLE_UART_NONE CONFIG_ESP_CONSOLE_UART_NONE
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_NUM
#define CONFIG_CONSOLE_UART_NUM CONFIG_ESP_CONSOLE_UART_NUM
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_RX_GPIO
#define CONFIG_CONSOLE_UART_RX_GPIO CONFIG_ESP_CONSOLE_UART_RX_GPIO
#endif

#ifdef CONFIG_ESP_CONSOLE_UART_TX_GPIO
#define CONFIG_CONSOLE_UART_TX_GPIO CONFIG_ESP_CONSOLE_UART_TX_GPIO
#endif

#ifdef CONFIG_COMPILER_CXX_EXCEPTIONS
#define CONFIG_CXX_EXCEPTIONS CONFIG_COMPILER_CXX_EXCEPTIONS
#endif

#ifdef CONFIG_COMPILER_CXX_EXCEPTIONS_EMG_POOL_SIZE
#define CONFIG_CXX_EXCEPTIONS_EMG_POOL_SIZE CONFIG_COMPILER_CXX_EXCEPTIONS_EMG_POOL_SIZE
#endif

#ifdef CONFIG_ESP32_DISABLE_BASIC_ROM_CONSOLE
#define CONFIG_DISABLE_BASIC_ROM_CONSOLE CONFIG_ESP32_DISABLE_BASIC_ROM_CONSOLE
#endif

#ifdef CONFIG_COMPILER_DISABLE_GCC8_WARNINGS
#define CONFIG_DISABLE_GCC8_WARNINGS CONFIG_COMPILER_DISABLE_GCC8_WARNINGS
#endif

#ifdef CONFIG_BOOTLOADER_EFUSE_SECURE_VERSION_EMULATE
#define CONFIG_EFUSE_SECURE_VERSION_EMULATE CONFIG_BOOTLOADER_EFUSE_SECURE_VERSION_EMULATE
#endif

#ifdef CONFIG_ESP32_RTC_CLK_SRC
#define CONFIG_ESP32_RTC_CLOCK_SOURCE CONFIG_ESP32_RTC_CLK_SRC
#endif

#ifdef CONFIG_ESP32_RTC_CLK_SRC_EXT_CRYS
#define CONFIG_ESP32_RTC_CLOCK_SOURCE_EXTERNAL_CRYSTAL CONFIG_ESP32_RTC_CLK_SRC_EXT_CRYS
#endif

#ifdef CONFIG_ESP32_RTC_CLK_SRC_EXT_OSC
#define CONFIG_ESP32_RTC_CLOCK_SOURCE_EXTERNAL_OSC CONFIG_ESP32_RTC_CLK_SRC_EXT_OSC
#endif

#ifdef CONFIG_ESP32_RTC_CLK_SRC_INT_8MD256
#define CONFIG_ESP32_RTC_CLOCK_SOURCE_INTERNAL_8MD256 CONFIG_ESP32_RTC_CLK_SRC_INT_8MD256
#endif

#ifdef CONFIG_ESP32_RTC_CLK_SRC_INT_RC
#define CONFIG_ESP32_RTC_CLOCK_SOURCE_INTERNAL_RC CONFIG_ESP32_RTC_CLK_SRC_INT_RC
#endif

#ifdef CONFIG_ESP32_RTC_EXT_CRYST_ADDIT_CURRENT
#define CONFIG_ESP32_RTC_EXTERNAL_CRYSTAL_ADDITIONAL_CURRENT CONFIG_ESP32_RTC_EXT_CRYST_ADDIT_CURRENT
#endif

#ifdef CONFIG_ESPTOOLPY_FLASHMODE_DIO
#define CONFIG_FLASHMODE_DIO CONFIG_ESPTOOLPY_FLASHMODE_DIO
#endif

#ifdef CONFIG_ESPTOOLPY_FLASHMODE_DOUT
#define CONFIG_FLASHMODE_DOUT CONFIG_ESPTOOLPY_FLASHMODE_DOUT
#endif

#ifdef CONFIG_ESPTOOLPY_FLASHMODE_QIO
#define CONFIG_FLASHMODE_QIO CONFIG_ESPTOOLPY_FLASHMODE_QIO
#endif

#ifdef CONFIG_ESPTOOLPY_FLASHMODE_QOUT
#define CONFIG_FLASHMODE_QOUT CONFIG_ESPTOOLPY_FLASHMODE_QOUT
#endif

#ifdef CONFIG_SECURE_FLASH_ENC_ENABLED
#define CONFIG_FLASH_ENCRYPTION_ENABLED CONFIG_SECURE_FLASH_ENC_ENABLED
#endif

#ifdef CONFIG_SECURE_FLASH_ENCRYPTION_MODE_DEVELOPMENT
#define CONFIG_FLASH_ENCRYPTION_INSECURE CONFIG_SECURE_FLASH_ENCRYPTION_MODE_DEVELOPMENT
#endif

#ifdef CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_CACHE
#define CONFIG_FLASH_ENCRYPTION_UART_BOOTLOADER_ALLOW_CACHE CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_CACHE
#endif

#ifdef CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_DEC
#define CONFIG_FLASH_ENCRYPTION_UART_BOOTLOADER_ALLOW_DECRYPT CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_DEC
#endif

#ifdef CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_ENC
#define CONFIG_FLASH_ENCRYPTION_UART_BOOTLOADER_ALLOW_ENCRYPT CONFIG_SECURE_FLASH_UART_BOOTLOADER_ALLOW_ENC
#endif

#ifdef CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES_FOUR
#define CONFIG_FOUR_UNIVERSAL_MAC_ADDRESS CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES_FOUR
#endif

#ifdef CONFIG_ESP_GDBSTUB_MAX_TASKS
#define CONFIG_GDBSTUB_MAX_TASKS CONFIG_ESP_GDBSTUB_MAX_TASKS
#endif

#ifdef CONFIG_ESP_GDBSTUB_SUPPORT_TASKS
#define CONFIG_GDBSTUB_SUPPORT_TASKS CONFIG_ESP_GDBSTUB_SUPPORT_TASKS
#endif

#ifdef CONFIG_ESP_INT_WDT
#define CONFIG_INT_WDT CONFIG_ESP_INT_WDT
#endif

#ifdef CONFIG_ESP_INT_WDT_CHECK_CPU1
#define CONFIG_INT_WDT_CHECK_CPU1 CONFIG_ESP_INT_WDT_CHECK_CPU1
#endif

#ifdef CONFIG_ESP_INT_WDT_TIMEOUT_MS
#define CONFIG_INT_WDT_TIMEOUT_MS CONFIG_ESP_INT_WDT_TIMEOUT_MS
#endif

#ifdef CONFIG_ESP_IPC_TASK_STACK_SIZE
#define CONFIG_IPC_TASK_STACK_SIZE CONFIG_ESP_IPC_TASK_STACK_SIZE
#endif

#ifdef CONFIG_BOOTLOADER_LOG_LEVEL
#define CONFIG_LOG_BOOTLOADER_LEVEL CONFIG_BOOTLOADER_LOG_LEVEL
#endif

#ifdef CONFIG_BOOTLOADER_LOG_LEVEL_DEBUG
#define CONFIG_LOG_BOOTLOADER_LEVEL_DEBUG CONFIG_BOOTLOADER_LOG_LEVEL_DEBUG
#endif

#ifdef CONFIG_BOOTLOADER_LOG_LEVEL_ERROR
#define CONFIG_LOG_BOOTLOADER_LEVEL_ERROR CONFIG_BOOTLOADER_LOG_LEVEL_ERROR
#endif

#ifdef CONFIG_BOOTLOADER_LOG_LEVEL_INFO
#define CONFIG_LOG_BOOTLOADER_LEVEL_INFO CONFIG_BOOTLOADER_LOG_LEVEL_INFO
#endif

#ifdef CONFIG_BOOTLOADER_LOG_LEVEL_NONE
#define CONFIG_LOG_BOOTLOADER_LEVEL_NONE CONFIG_BOOTLOADER_LOG_LEVEL_NONE
#endif

#ifdef CONFIG_BOOTLOADER_LOG_LEVEL_VERBOSE
#define CONFIG_LOG_BOOTLOADER_LEVEL_VERBOSE CONFIG_BOOTLOADER_LOG_LEVEL_VERBOSE
#endif

#ifdef CONFIG_BOOTLOADER_LOG_LEVEL_WARN
#define CONFIG_LOG_BOOTLOADER_LEVEL_WARN CONFIG_BOOTLOADER_LOG_LEVEL_WARN
#endif

#ifdef CONFIG_ESP_MAIN_TASK_STACK_SIZE
#define CONFIG_MAIN_TASK_STACK_SIZE CONFIG_ESP_MAIN_TASK_STACK_SIZE
#endif

#ifdef CONFIG_SDK_MAKE_WARN_UNDEFINED_VARIABLES
#define CONFIG_MAKE_WARN_UNDEFINED_VARIABLES CONFIG_SDK_MAKE_WARN_UNDEFINED_VARIABLES
#endif

#ifdef CONFIG_ESP32_MEMMAP_TRACEMEM
#define CONFIG_MEMMAP_TRACEMEM CONFIG_ESP32_MEMMAP_TRACEMEM
#endif

#ifdef CONFIG_ESP32_MEMMAP_TRACEMEM_TWOBANKS
#define CONFIG_MEMMAP_TRACEMEM_TWOBANKS CONFIG_ESP32_MEMMAP_TRACEMEM_TWOBANKS
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD
#define CONFIG_MONITOR_BAUD CONFIG_ESPTOOLPY_MONITOR_BAUD
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_115200B
#define CONFIG_MONITOR_BAUD_115200B CONFIG_ESPTOOLPY_MONITOR_BAUD_115200B
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_230400B
#define CONFIG_MONITOR_BAUD_230400B CONFIG_ESPTOOLPY_MONITOR_BAUD_230400B
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_2MB
#define CONFIG_MONITOR_BAUD_2MB CONFIG_ESPTOOLPY_MONITOR_BAUD_2MB
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_57600B
#define CONFIG_MONITOR_BAUD_57600B CONFIG_ESPTOOLPY_MONITOR_BAUD_57600B
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_921600B
#define CONFIG_MONITOR_BAUD_921600B CONFIG_ESPTOOLPY_MONITOR_BAUD_921600B
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_9600B
#define CONFIG_MONITOR_BAUD_9600B CONFIG_ESPTOOLPY_MONITOR_BAUD_9600B
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_OTHER
#define CONFIG_MONITOR_BAUD_OTHER CONFIG_ESPTOOLPY_MONITOR_BAUD_OTHER
#endif

#ifdef CONFIG_ESPTOOLPY_MONITOR_BAUD_OTHER_VAL
#define CONFIG_MONITOR_BAUD_OTHER_VAL CONFIG_ESPTOOLPY_MONITOR_BAUD_OTHER_VAL
#endif

#ifdef CONFIG_ESP32_NO_BLOBS
#define CONFIG_NO_BLOBS CONFIG_ESP32_NO_BLOBS
#endif

#ifdef CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES
#define CONFIG_NUMBER_OF_UNIVERSAL_MAC_ADDRESS CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_DISABLE
#define CONFIG_OPTIMIZATION_ASSERTIONS_DISABLED CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_DISABLE
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_ENABLE
#define CONFIG_OPTIMIZATION_ASSERTIONS_ENABLED CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_ENABLE
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_SILENT
#define CONFIG_OPTIMIZATION_ASSERTIONS_SILENT CONFIG_COMPILER_OPTIMIZATION_ASSERTIONS_SILENT
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_ASSERTION_LEVEL
#define CONFIG_OPTIMIZATION_ASSERTION_LEVEL CONFIG_COMPILER_OPTIMIZATION_ASSERTION_LEVEL
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION
#define CONFIG_OPTIMIZATION_COMPILER CONFIG_COMPILER_OPTIMIZATION
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_DEFAULT
#define CONFIG_OPTIMIZATION_LEVEL_DEBUG CONFIG_COMPILER_OPTIMIZATION_DEFAULT
#endif

#ifdef CONFIG_COMPILER_OPTIMIZATION_SIZE
#define CONFIG_OPTIMIZATION_LEVEL_RELEASE CONFIG_COMPILER_OPTIMIZATION_SIZE
#endif

#ifdef CONFIG_SDK_PYTHON
#define CONFIG_PYTHON CONFIG_SDK_PYTHON
#endif

#ifdef CONFIG_ESP32_REDUCE_PHY_TX_POWER
#define CONFIG_REDUCE_PHY_TX_POWER CONFIG_ESP32_REDUCE_PHY_TX_POWER
#endif

#ifdef CONFIG_ESP32_SPIRAM_SUPPORT
#define CONFIG_SPIRAM_SUPPORT CONFIG_ESP32_SPIRAM_SUPPORT
#endif

#ifdef CONFIG_SPI_FLASH_DANGEROUS_WRITE
#define CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS CONFIG_SPI_FLASH_DANGEROUS_WRITE
#endif

#ifdef CONFIG_SPI_FLASH_DANGEROUS_WRITE_ABORTS
#define CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS_ABORTS CONFIG_SPI_FLASH_DANGEROUS_WRITE_ABORTS
#endif

#ifdef CONFIG_SPI_FLASH_DANGEROUS_WRITE_ALLOWED
#define CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS_ALLOWED CONFIG_SPI_FLASH_DANGEROUS_WRITE_ALLOWED
#endif

#ifdef CONFIG_SPI_FLASH_DANGEROUS_WRITE_FAILS
#define CONFIG_SPI_FLASH_WRITING_DANGEROUS_REGIONS_FAILS CONFIG_SPI_FLASH_DANGEROUS_WRITE_FAILS
#endif

#ifdef CONFIG_COMPILER_STACK_CHECK
#define CONFIG_STACK_CHECK CONFIG_COMPILER_STACK_CHECK
#endif

#ifdef CONFIG_COMPILER_STACK_CHECK_MODE_ALL
#define CONFIG_STACK_CHECK_ALL CONFIG_COMPILER_STACK_CHECK_MODE_ALL
#endif

#ifdef CONFIG_COMPILER_STACK_CHECK_MODE
#define CONFIG_STACK_CHECK_MODE CONFIG_COMPILER_STACK_CHECK_MODE
#endif

#ifdef CONFIG_COMPILER_STACK_CHECK_MODE_NONE
#define CONFIG_STACK_CHECK_NONE CONFIG_COMPILER_STACK_CHECK_MODE_NONE
#endif

#ifdef CONFIG_COMPILER_STACK_CHECK_MODE_NORM
#define CONFIG_STACK_CHECK_NORM CONFIG_COMPILER_STACK_CHECK_MODE_NORM
#endif

#ifdef CONFIG_COMPILER_STACK_CHECK_MODE_STRONG
#define CONFIG_STACK_CHECK_STRONG CONFIG_COMPILER_STACK_CHECK_MODE_STRONG
#endif

#ifdef CONFIG_ESP_SYSTEM_EVENT_QUEUE_SIZE
#define CONFIG_SYSTEM_EVENT_QUEUE_SIZE CONFIG_ESP_SYSTEM_EVENT_QUEUE_SIZE
#endif

#ifdef CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE
#define CONFIG_SYSTEM_EVENT_TASK_STACK_SIZE CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE
#endif

#ifdef CONFIG_ESP_TASK_WDT
#define CONFIG_TASK_WDT CONFIG_ESP_TASK_WDT
#endif

#ifdef CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0
#define CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU0 CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU0
#endif

#ifdef CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1
#define CONFIG_TASK_WDT_CHECK_IDLE_TASK_CPU1 CONFIG_ESP_TASK_WDT_CHECK_IDLE_TASK_CPU1
#endif

#ifdef CONFIG_ESP_TASK_WDT_PANIC
#define CONFIG_TASK_WDT_PANIC CONFIG_ESP_TASK_WDT_PANIC
#endif

#ifdef CONFIG_ESP_TASK_WDT_TIMEOUT_S
#define CONFIG_TASK_WDT_TIMEOUT_S CONFIG_ESP_TASK_WDT_TIMEOUT_S
#endif

#ifdef CONFIG_ESP_TIMER_TASK_STACK_SIZE
#define CONFIG_TIMER_TASK_STACK_SIZE CONFIG_ESP_TIMER_TASK_STACK_SIZE
#endif

#ifdef CONFIG_SDK_TOOLPREFIX
#define CONFIG_TOOLPREFIX CONFIG_SDK_TOOLPREFIX
#endif

#ifdef CONFIG_ESP32_TRACEMEM_RESERVE_DRAM
#define CONFIG_TRACEMEM_RESERVE_DRAM CONFIG_ESP32_TRACEMEM_RESERVE_DRAM
#endif

#ifdef CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES_TWO
#define CONFIG_TWO_UNIVERSAL_MAC_ADDRESS CONFIG_ESP32_UNIVERSAL_MAC_ADDRESSES_TWO
#endif

#ifdef CONFIG_ESP32_ULP_COPROC_ENABLED
#define CONFIG_ULP_COPROC_ENABLED CONFIG_ESP32_ULP_COPROC_ENABLED
#endif

#ifdef CONFIG_ESP32_ULP_COPROC_RESERVE_MEM
#define CONFIG_ULP_COPROC_RESERVE_MEM CONFIG_ESP32_ULP_COPROC_RESERVE_MEM
#endif

#ifdef CONFIG_COMPILER_WARN_WRITE_STRINGS
#define CONFIG_WARN_WRITE_STRINGS CONFIG_COMPILER_WARN_WRITE_STRINGS
#endif

#ifdef CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP
#define CONFIG_WIFI_LWIP_ALLOCATION_FROM_SPIRAM_FIRST CONFIG_SPIRAM_TRY_ALLOCATE_WIFI_LWIP
#endif

