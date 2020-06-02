# Copyright 2019 NXP
#
# This software is owned or controlled by NXP and may only be used
# strictly in accordance with the applicable license terms.  By expressly
# accepting such terms or by downloading, installing, activating and/or
# otherwise using the software, you are agreeing that you have read, and
# that you agree to comply with and are bound by, such license terms.  If
# you do not agree to be bound by the applicable license terms, then you
# may not retain, install, activate or otherwise use the software.

$(call add-clean-step, rm -rf $(PRODUCT_OUT)/obj/lib/libse050keymasterdevice.so)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/obj/lib/libse050keymasterdevice.so.toc)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/obj_arm/lib/libse050keymasterdevice.so)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/obj_arm/lib/libse050keymasterdevice.so.toc)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/obj/SHARED_LIBRARIES/libse050keymasterdevice_intermediates)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/obj_arm/SHARED_LIBRARIES/libse050keymasterdevice_intermediates)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/testcases/se050keymaster_tests)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/testcases/se05xRotatePlatfSCP03)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/testcases/se05xGetCertificate)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/testcases/se05xGetInfo)
$(call add-clean-step, rm -rf $(PRODUCT_OUT)/testcases/jrcpv1_server)
