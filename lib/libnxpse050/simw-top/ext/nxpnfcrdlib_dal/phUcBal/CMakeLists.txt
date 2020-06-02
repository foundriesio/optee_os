

PROJECT(phUcBal)


FILE(GLOB ${PROJECT_NAME}_Sources
    *.c
    src/*.c
)

include_directories(
    inc
	${usbd_lib_cdc_SOURCE_DIR}/inc
    ${usbd_lib_cdc_SOURCE_DIR}/appusbd
)

ADD_EXECUTABLE(${PROJECT_NAME}
    ${${PROJECT_NAME}_Sources}
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    NxpRdLib_Platform
    usbd_lib_cdc
)

ADD_CUSTOM_COMMAND(
    TARGET ${PROJECT_NAME}
    arm-none-eabi-sizes $<TARGET_FILE:${PROJECT_NAME}>
)
