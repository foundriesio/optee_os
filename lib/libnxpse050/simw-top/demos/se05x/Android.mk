# copyright 2019 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.
#

#This file is required for ANDROID to create binaries of SE05X based Demo's
#Android Keymaster does not have  dependancies on below source files, but these binaries can be useful
#to verify Communication between Host and SE05X

include $(CLEAR_VARS)

LOCAL_MODULE := se05xGetInfo
LOCAL_SRC_FILES := \
    demos/se05x/se05x_Get_Info/se05x_Get_Info.c \

LOCAL_C_INCLUDES := \
    $(SIMW_TOP_PATH)/akm/ \
    $(SIMW_TOP_PATH)/sss/ex/inc \
    $(SIMW_TOP_PATH)/sss/inc \
    $(SIMW_TOP_PATH)/sss/port/default \
    $(SIMW_TOP_PATH)/ext/mbedtls/include \
    $(SIMW_TOP_PATH)/hostlib/hostLib/se05x_03_xx_xx \
    $(SIMW_TOP_PATH)/hostlib/hostLib/inc \
    $(SIMW_TOP_PATH)/hostlib/hostLib/libCommon/smCom \
    $(SIMW_TOP_PATH)/hostlib/hostLib/libCommon/infra \

LOCAL_CFLAGS = -Wall -Werror -Wunused -Wno-missing-field-initializers -DKEYMASTER_NAME_TAGS -DSSS_USE_FTR_FILE
LOCAL_CLANG := true
LOCAL_CLANG_CFLAGS += -Wno-error=unused-const-variable -Wno-error=unused-private-field
LOCAL_CLANG_CFLAGS += -fno-sanitize-coverage=edge,indirect-calls,8bit-counters,trace-cmp
LOCAL_MODULE_TAGS := tests

LOCAL_SHARED_LIBRARIES := \
    libse050keymasterdevice \

include $(BUILD_NATIVE_TEST)


ifeq ("${SE05X_Auth}","PlatfSCP03")

include $(CLEAR_VARS)

LOCAL_MODULE := se05xRotatePlatfSCP03
LOCAL_SRC_FILES := \
    demos/se05x/se05x_RotatePlatformSCP03Keys/se05x_TP_PlatformSCP03keys.c \

LOCAL_C_INCLUDES := \
    $(SIMW_TOP_PATH)/akm/ \
    $(SIMW_TOP_PATH)/sss/ex/inc \
    $(SIMW_TOP_PATH)/sss/inc \
    $(SIMW_TOP_PATH)/sss/port/default \
    $(SIMW_TOP_PATH)/ext/mbedtls/include \
    $(SIMW_TOP_PATH)/hostlib/hostLib/se05x_03_xx_xx \
    $(SIMW_TOP_PATH)/hostlib/hostLib/inc \
    $(SIMW_TOP_PATH)/hostlib/hostLib/libCommon/smCom \
    $(SIMW_TOP_PATH)/hostlib/hostLib/libCommon/infra \

LOCAL_CFLAGS = -Wall -Werror -Wunused -Wno-missing-field-initializers -DKEYMASTER_NAME_TAGS -DSSS_USE_FTR_FILE
LOCAL_CLANG := true
LOCAL_CLANG_CFLAGS += -Wno-error=unused-const-variable -Wno-error=unused-private-field
LOCAL_CLANG_CFLAGS += -fno-sanitize-coverage=edge,indirect-calls,8bit-counters,trace-cmp
LOCAL_MODULE_TAGS := tests

LOCAL_SHARED_LIBRARIES := \
    libse050keymasterdevice \

include $(BUILD_NATIVE_TEST)

endif # PlatfSCP03

include $(CLEAR_VARS)

LOCAL_MODULE := se05xGetCertificate
LOCAL_SRC_FILES := \
    demos/se05x/se05x_Get_Certificate/se05x_Get_Certificate.c \

LOCAL_C_INCLUDES := \
    $(SIMW_TOP_PATH)/akm/ \
    $(SIMW_TOP_PATH)/sss/ex/inc \
    $(SIMW_TOP_PATH)/sss/inc \
    $(SIMW_TOP_PATH)/sss/port/default \
    $(SIMW_TOP_PATH)/ext/mbedtls/include \
    $(SIMW_TOP_PATH)/hostlib/hostLib/se05x_03_xx_xx \
    $(SIMW_TOP_PATH)/hostlib/hostLib/inc \
    $(SIMW_TOP_PATH)/hostlib/hostLib/libCommon/smCom \
    $(SIMW_TOP_PATH)/hostlib/hostLib/libCommon/infra \

LOCAL_CFLAGS = -Wall -Werror -Wunused -Wno-missing-field-initializers -DKEYMASTER_NAME_TAGS -DSSS_USE_FTR_FILE
LOCAL_CLANG := true
LOCAL_CLANG_CFLAGS += -Wno-error=unused-const-variable -Wno-error=unused-private-field
LOCAL_CLANG_CFLAGS += -fno-sanitize-coverage=edge,indirect-calls,8bit-counters,trace-cmp
LOCAL_MODULE_TAGS := tests

LOCAL_SHARED_LIBRARIES := \
    libse050keymasterdevice \

include $(BUILD_NATIVE_TEST)
