cflags-y += -Wno-error

cflags-y += -DAX_EMBEDDED=1
cflags-y += -DVERBOSE_APDU_LOGS=0
cflags-y += -DT1oI2C_UM1225_SE050
cflags-y += -DT1oI2C
cflags-y += -DSSS_USE_FTR_FILE

incdirs-y += .
incdirs-y += ../adaptors/include
incdirs-y += ../se050/glue/include
incdirs-y += ../se050/simw-top/hostlib/hostLib/inc/
incdirs-y += ../se050/simw-top/hostlib/hostLib/libCommon/infra/
incdirs-y += ../se050/simw-top/hostlib/hostLib/libCommon/smCom/
incdirs-y += ../se050/simw-top/hostlib/hostLib/libCommon/smCom/T1oI2C/
incdirs-y += ../se050/simw-top/hostlib/hostLib/platform/inc/
incdirs-y += ../se050/simw-top/hostlib/hostLib/se05x_03_xx_xx/
incdirs-y += ../se050/simw-top/sss/ex/inc/
incdirs-y += ../se050/simw-top/sss/inc/
incdirs-y += ../se050/simw-top/sss/port/default/

srcs-y += scp03.c
srcs-y += cert.c
srcs-$(CFG_NXP_SE05X_HUK_DRV) += huk.c
srcs-$(CFG_NXP_SE05X_RNG_DRV) += rng.c
# Disabled not to slow down loading a TA (this operation over i2c is really
# slow)
#srcs-$(call cfg-one-enabled, CFG_CRYPTO_MD5 CFG_CRYPTO_SHA1 CFG_CRYPTO_SHA224 \
#			     CFG_CRYPTO_SHA256 CFG_CRYPTO_SHA384 \
#			     CFG_CRYPTO_SHA512) += hash.c
# srcs-$(CFG_CRYPTO_HMAC) += hmac.c
ifeq ($(CFG_CRYPTO_AES),y)
# srcs-y += aes.c
# srcs-$(CFG_CRYPTO_ECB) += aes_ecb.c
# aes cbc cypher slows down RPMB operations, disable
# srcs-$(CFG_CRYPTO_CBC) += aes_cbc.c
srcs-$(CFG_CRYPTO_CTR) += aes_ctr.c
endif
ifeq ($(CFG_CRYPTO_DES),y)
srcs-$(CFG_CRYPTO_ECB) += des_ecb.c
srcs-$(CFG_CRYPTO_CBC) += des_cbc.c
endif
srcs-$(CFG_CRYPTO_RSA) += rsa.c
srcs-$(CFG_CRYPTO_ECC) += ecc.c
