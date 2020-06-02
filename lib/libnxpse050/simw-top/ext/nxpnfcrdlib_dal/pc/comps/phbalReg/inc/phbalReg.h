/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic BAL Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHBALREG_H
#define PHBALREG_H

#include <ph_Status.h>
#include <phOsal.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PHBAL_REG

/** \defgroup phbalReg Bus Abstraction Layer
* @{
*/

/**
* \brief BAL type structure
*/
typedef struct
{
    uint16_t       wId;              /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint8_t        bBalType;         /**< BAL type used by HAL to configure the BAL configured at runtime. */
} phbalReg_Type_t;
/** @} */

/**
* \name BAL types
*/
/*@{*/
#define PHBAL_REG_TYPE_SPI                  0x0001U /**< SPI communication channel. */
#define PHBAL_REG_TYPE_I2C                  0x0002U /**< I2C communication channel. */
#define PHBAL_REG_TYPE_SERIAL_WIN           0x0003U /**< SerialWin communication channel. */
#define PHBAL_REG_TYPE_KERNEL_SPI           0x0004U /**< Linux kernel space SPI communication channel. */
#define PHBAL_REG_TYPE_USER_SPI             0x0005U /**< Linux user space SPI communication channel. */

/*@}*/

#define PH_DRIVER_SET_HIGH            1          /**< Logic High. */
#define PH_DRIVER_SET_LOW             0          /**< Logic Low. */

#endif /* NXPBUILD__PHBAL_REG */

#ifdef NXPBUILD__PHBAL_REG_SERIALWIN

/** \defgroup phbalReg_SerialWin Component : RS232 (Windows)
* \brief RS232 BAL (Windows)
*
* Serial-Line (COM-Port) interface for Windows.
* @{
*/

#define PHBAL_REG_SERIALWIN_ID          0x01U       /**< ID for Serial BAL component (Win32) */

/**
* \brief RS232 (Windows) BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;/**< Contains layer ID and bal type for this BAL component, NEVER MODIFY! */
    int8_t * pComPortString;    /**< Com-Port string. */
    void * pComHandle;          /**< Handle to communication port. */
    uint32_t dwBitRate;         /**< Storage for bit rate setting. */
    uint16_t wTimeout;          /**< Storage for timeout setting. */
    uint16_t wEnumBufSize;      /**< Size of the enumeration buffer. Maximum possible size is 255 bytes. */
    uint8_t * pEnumBuffer;      /**< Pointer to the enumeration buffer for the GetPortList() function. */
    uint8_t bStopBits;          /**< Number of stopbits. */
} phbalReg_SerialWin_DataParams_t;

/**
* \name BAL Communication Configs
*/
/*@{*/
#define PHBAL_REG_SERIALWIN_CONFIG_BITRATE      (PH_CONFIG_CUSTOM_BEGIN)    /**< Configure communication speed. */
#define PHBAL_REG_SERIALWIN_CONFIG_STOPBITS     (PH_CONFIG_CUSTOM_BEGIN+1)  /**< Configure number of stop bits. */
/*@}*/

/**
* \name BAL Configuration Values
*/
/*@{*/
#define PHBAL_REG_SERIALWIN_VALUE_BITRATE_9600      0x0000U     /**< 9600 kBit/s. */
#define PHBAL_REG_SERIALWIN_VALUE_BITRATE_19200     0x0001U     /**< 19200 kBit/s. */
#define PHBAL_REG_SERIALWIN_VALUE_BITRATE_38400     0x0002U     /**< 38400 kBit/s. */
#define PHBAL_REG_SERIALWIN_VALUE_BITRATE_57600     0x0003U     /**< 57600 kBit/s. */
#define PHBAL_REG_SERIALWIN_VALUE_BITRATE_115200    0x0004U     /**< 115200 kBit/s. */
#define PHBAL_REG_SERIALWIN_VALUE_BITRATE_230400    0x0005U     /**< 230400 kBit/s. */
#define PHBAL_REG_SERIALWIN_VALUE_BITRATE_460800    0x0006U     /**< 460800 kBit/s. */

#define PHBAL_REG_SERIALWIN_VALUE_STOPBITS_ONE    0x0000U     /**< One stopbit. */
#define PHBAL_REG_SERIALWIN_VALUE_STOPBITS_ONE5   0x0001U     /**< One and a half stopbit. */
#define PHBAL_REG_SERIALWIN_VALUE_STOPBITS_TWO    0x0002U     /**< Two stopbit. */
/*@}*/

/**
* \brief Initialize the RS232 BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER \b wEnumBufSize is invalid.
*/
phStatus_t phbalReg_SerialWin_Init(
                                   phbalReg_SerialWin_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wSizeOfDataParams,                      /**< [In] Specifies the size of the data parameter structure. */
                                   uint16_t wEnumBufSize,                           /**< [In] Size of the enumeration buffer. Maximum possible size is 255 bytes. */
                                   uint8_t * pEnumBuffer                            /**< [In] Pointer to the enumeration buffer for the GetPortList() function. */
                                   );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_SERIALWIN */

#ifdef NXPBUILD__PHBAL_REG_VISA

/** \cond INCLUDE_INTERNALS \defgroup phbalReg_Visa Component : VISA
* \brief VISA-Library BAL component.
* @{
*/

#define PHBAL_REG_VISA_ID          0x02U       /**< ID for VISA BAL component*/

/**
* \name Function pointers
*/
/*@{*/
typedef int32_t (*PHBAL_REG_VISA_OPEN_DEFAULT_RM)(uint32_t* vi);
typedef int32_t (*PHBAL_REG_VISA_OPEN)(uint32_t sesn, int8_t* name, uint32_t mode, uint32_t timeout, uint32_t* vi);
typedef int32_t (*PHBAL_REG_VISA_CLOSE)(uint32_t vi);

typedef int32_t (*PHBAL_REG_VISA_FIND_RSRC)(uint32_t sesn, char* expr, uint32_t* vi, uint32_t* retCnt, char desc[]);
typedef int32_t (*PHBAL_REG_VISA_FIND_NEXT)(uint32_t vi, char desc[]);

typedef int32_t (*PHBAL_REG_VISA_READ)(uint32_t vi, uint8_t* buf, uint32_t cnt, uint32_t* retCnt);
typedef int32_t (*PHBAL_REG_VISA_WRITE)(uint32_t vi, uint8_t* buf, uint32_t cnt, uint32_t* retCnt);

typedef int32_t (*PHBAL_REG_VISA_SET_ATTRIBUTE)(uint32_t vi, uint32_t attrName, uint32_t attrValue);
typedef int32_t (*PHBAL_REG_VISA_GET_ATTRIBUTE)(uint32_t vi, uint32_t attrName, void * attrValue);


/*@}*/

/**
* \brief VISA BAL parameter structure
*/
typedef struct
{
    uint16_t                        wId;                /**< Layer ID for this BAL component, NEVER MODIFY! */
    int8_t *                        pVisaPortString;    /**< VISA-Port string. */
    uint32_t                        defaultRM;            /**< Default Resource Manager for VISA */
    void *                          pHandle;            /**< Handle of the visa32.dll. */
    uint32_t                        instr;                /**< instrument address */
    PHBAL_REG_VISA_OPEN_DEFAULT_RM  pviOpenDefaultRM;   /**< function pointer for viOpenDefaultRM. */
    PHBAL_REG_VISA_OPEN             pviOpen;            /**< function pointer for pviOpen. */
    PHBAL_REG_VISA_CLOSE            pviClose;           /**< function pointer for pviClose. */
    PHBAL_REG_VISA_FIND_RSRC        pviFindRsrc;        /**< function pointer for pviFindRsrc. */
    PHBAL_REG_VISA_FIND_NEXT        pviFindNext;        /**< function pointer for pviFindNext. */
    PHBAL_REG_VISA_READ             pviRead;            /**< function pointer for pviRead. */
    PHBAL_REG_VISA_WRITE            pviWrite;           /**< function pointer for pviWrite. */
    PHBAL_REG_VISA_SET_ATTRIBUTE    pviSetAttribute;    /**< function pointer for pviSetAttribute. */
    PHBAL_REG_VISA_GET_ATTRIBUTE    pviGetAttribute;    /**< function pointer for pviGetAttribute. */
} phbalReg_Visa_DataParams_t;

/**
* \brief Initialize the VISA BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_USE_CONDITION Required dependencies not available.
*/
phStatus_t phbalReg_Visa_Init(
                              phbalReg_Visa_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wSizeOfDataParams                /**< [In] Specifies the size of the data parameter structure. */
                              );

/** @} \endcond */
#endif /* NXPBUILD__PHBAL_REG_VISA */

#ifdef NXPBUILD__PHBAL_REG_RDFPGAV6

/** \cond INCLUDE_INTERNALS \defgroup phbalReg_RdFpgaV6 Component : RdFpgaV6
* \brief BAL-Component for the FPGA-Box Reader
* @{
*/

#define PHBAL_REG_RDFPGAV6_ID     0x03U   /**< ID for RdFpga BAL component */

/**
* \name Custom Configs
*/
/*@{*/
#define PHBAL_REG_RDFPGAV6_ADDITIONAL_INFO      (PH_CONFIG_CUSTOM_BEGIN)    /**< Set / Get additional information . */
/*@}*/

/**
* \name Custom Error Codes
*/
/*@{*/
#define PHBAL_REG_RDFPGAV6_ERR_DLL        (PH_ERR_CUSTOM_BEGIN+0)    /**< phCDllReader.dll threw an non standard error read wAddionalInfo to get error code */
/*@}*/

/**
* \name Function pointers
*/
/*@{*/
typedef int16_t (*PHBAL_REG_RDFPGAV6_OPEN) (uint16_t wReaderType, uint8_t * paForceSoftwareStack);
typedef int16_t (*PHBAL_REG_RDFPGAV6_CLOSE)();
typedef int16_t (*PHBAL_REG_RDFPGAV6_INIT) (uint16_t wProtocolType);
typedef int16_t (*PHBAL_REG_RDFPGAV6_SETPARAM) (uint32_t dwIdentifier, uint32_t dwValue, uint8_t bParamSetMode);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GETPARAM) (uint32_t dwIdentifier, uint32_t * pdwValue);
typedef int16_t (*PHBAL_REG_RDFPGAV6_EXCHANGEBITS) (uint8_t * pTxBuffer, uint16_t wTxBytes, uint8_t bNumValidTxBits, uint8_t * pRxBuffer, uint16_t dwRxBufferSize, uint16_t * pdwRxBytes, uint8_t * pbNumValidRxBits);
typedef int16_t (*PHBAL_REG_RDFPGAV6_SETTARGETJOINER) (uint16_t joiner);
typedef int16_t (*PHBAL_REG_RDFPGAV6_SETJOINERREG) (uint8_t address, uint8_t value);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GETJOINERREG) (uint8_t address, uint8_t * pvalue);
typedef int16_t (*PHBAL_REG_RDFPGAV6_LOADJOINERINIFILE) (uint8_t * filename);
typedef int16_t (*PHBAL_REG_RDFPGAV6_ACTFIELD) ();
typedef int16_t (*PHBAL_REG_RDFPGAV6_DEACTIVATEFIELD) ();
typedef int16_t (*PHBAL_REG_RDFPGAV6_HFPAUSE) (uint32_t pause_time);
typedef int16_t (*PHBAL_REG_RDFPGAV6_SET_POTIPOS) (uint16_t pos);
typedef int16_t (*PHBAL_REG_RDFPGAV6_SET_DEFAULTPOTIPOS) (uint16_t pos);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_POTIPOS) (uint16_t * pos);
typedef int16_t (*PHBAL_REG_RDFPGAV6_INC_POTIPOS)();
typedef int16_t (*PHBAL_REG_RDFPGAV6_DEC_POTIPOS)();
typedef int16_t (*PHBAL_REG_RDFPGAV6_SET_TIMEOUTVALUE) (uint32_t timeout_value);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_TIMEOUTVALUE) (uint32_t * timeout_value);
typedef int16_t (*PHBAL_REG_RDFPGAV6_LOAD_PATTERN) (uint8_t * filename, uint8_t id);
typedef int16_t (*PHBAL_REG_RDFPGAV6_PLAY_PATTERN) (uint8_t id);
typedef int16_t (*PHBAL_REG_RDFPGAV6_PLAYSEQA) ();
typedef int16_t (*PHBAL_REG_RDFPGAV6_PLAYSEQB) ();
typedef int16_t (*PHBAL_REG_RDFPGAV6_PLAYSINUS) ();
typedef int16_t (*PHBAL_REG_RDFPGAV6_SELECT_TRIGGER) (uint16_t source);
typedef int16_t (*PHBAL_REG_RDFPGAV6_LBLRST) (uint16_t rising_edge_cnt, uint32_t sinus, uint32_t hf_pause);
typedef int16_t (*PHBAL_REG_RDFPGAV6_LBLRST_COUNTERRESET) (uint16_t rising_edge_cnt, uint32_t sinus, uint32_t hf_pause);
typedef int16_t (*PHBAL_REG_RDFPGAV6_LBLRST_SSCOUNTERRESET) (uint16_t rising_edge_cnt, uint32_t sinus, uint32_t hf_pause);
typedef int16_t (*PHBAL_REG_RDFPGAV6_LBLRST_SINGLESHOT) (uint16_t rising_edge_cnt, uint32_t sinus, uint32_t hf_pause);
typedef int16_t (*PHBAL_REG_RDFPGAV6_LBLRST_WITHCOMMAND) (uint32_t sinus);
typedef int16_t (*PHBAL_REG_RDFPGAV6_ACT_FDT_MEASUREMENT) ();
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_FDT) (uint32_t * fdt);
typedef int16_t (*PHBAL_REG_RDFPGAV6_ACT_ANTITEARINGMODE) (uint32_t sinus, uint32_t hf_pause);
typedef int16_t (*PHBAL_REG_RDFPGAV6_RESONANCE_FREQ_TEST) (uint32_t hf_pause, uint16_t repeat_freq_id);
typedef int16_t (*PHBAL_REG_RDFPGAV6_ACT_FDT_PICC_TO_PCD_MEASUREMENT) (uint32_t time, uint8_t check_resp, uint16_t rx_byte_cnt, uint8_t rx_bit_cnt, uint8_t * rx_expected);
typedef int16_t (*PHBAL_REG_RDFPGAV6_ACT_LMA_MEASUREMENT) ();
typedef int16_t (*PHBAL_REG_RDFPGAV6_ACT_SEQUENCERMODE)();
typedef int16_t (*PHBAL_REG_RDFPGAV6_DEACTIVATE_MODES)();
typedef int16_t (*PHBAL_REG_RDFPGAV6_SET_JOINER_RXGAIN) (uint8_t value);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_JOINER_RXGAIN) (uint8_t * pvalue);
typedef int16_t (*PHBAL_REG_RDFPGAV6_ACTIVEDS_SET_FDT) (uint32_t value);
typedef int16_t (*PHBAL_REG_RDFPGAV6_DS_SET_FDT) (uint32_t value, uint8_t id);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_VERSION) (uint8_t * pvalue);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_REVISION) (uint8_t * pvalue);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_ERROR_CODES) (uint8_t * perrors, uint8_t max_error_cnt, uint8_t * perror_cnt);
typedef int16_t (*PHBAL_REG_RDFPGAV6_SET_CLOCK_SOURCE) (uint16_t clock_source);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_FIRST_RESPONSE) (uint8_t * pRxBuffer, uint16_t wRxBufferSize, uint16_t * pwRxBytes, uint8_t * pNumValidRxBits, int16_t * pwError);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_RESPONSE) (uint8_t * pRxBuffer, uint16_t wRxBufferSize, uint16_t * pwRxBytes, uint8_t * pNumValidRxBits, int16_t * pwError);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_IQ_TIMEOUTVALUE) (uint32_t * pdwTimeoutValue);
typedef int16_t (*PHBAL_REG_RDFPGAV6_SET_IQ_TIMEOUTVALUE) (uint32_t dwTimeoutValue);
typedef int16_t (*PHBAL_REG_RDFPGAV6_GET_TERMINALID) (uint8_t * pTerminalId);
typedef int16_t (*PHBAL_REG_RDFPGAV6_RESET_JOINER) ( );
/*@}*/

/**
* \brief RDFPGA BAL parameter structure
*/
typedef struct
{
    uint16_t                                                wId;                                        /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint8_t *                                                pDeviceName;                                /**< Name of FPGA Box Device device. */
    uint8_t                                                    bPortOpen;                                    /**< Internal State if port is opened. */
    uint16_t                                                wAdditionalInfo;                            /**< Storage for additional error information. */
    uint16_t                                                wTimeout;                                    /**< Shadow for the timeout. */
    void *                                                  pHandle;                                    /**< Handle of the phCDllReaderLib.dll. */
    PHBAL_REG_RDFPGAV6_OPEN                                 pfRdFpga_Open;                              /**< function pointer for pfRdFpga_Open. */
    PHBAL_REG_RDFPGAV6_CLOSE                                pfRdFpga_Close;                             /**< function pointer for pfRdFpga_Close. */
    PHBAL_REG_RDFPGAV6_INIT                                 pfRdFpga_Init;                              /**< function pointer for pfRdFpga_Init. */
    PHBAL_REG_RDFPGAV6_SETPARAM                             pfRdFpga_SetParam;                          /**< function pointer for pfRdFpga_SetParam. */
    PHBAL_REG_RDFPGAV6_GETPARAM                             pfRdFpga_GetParam;                          /**< function pointer for pfRdFpga_GetParam. */
    PHBAL_REG_RDFPGAV6_EXCHANGEBITS                         pfRdFpga_ExchangeBits;                      /**< function pointer for pfRdFpga_ExchangeBits. */
    PHBAL_REG_RDFPGAV6_SETTARGETJOINER                      pfRdFpga_SetTargetJoiner;                   /**< function pointer for pfRdFpga_SetTargetJoiner. */
    PHBAL_REG_RDFPGAV6_SETJOINERREG                         pfRdFpga_SetJoinerReg;                      /**< function pointer for pfRdFpga_SetJoinerReg. */
    PHBAL_REG_RDFPGAV6_GETJOINERREG                         pfRdFpga_GetJoinerReg;                      /**< function pointer for pfRdFpga_GetJoinerReg. */
    PHBAL_REG_RDFPGAV6_LOADJOINERINIFILE                    pfRdFpga_LoadJoinerIniFile;                 /**< function pointer for pfRdFpga_LoadJoinerIniFile. */
    PHBAL_REG_RDFPGAV6_ACTFIELD                             pfRdFpga_ActivateField;                     /**< function pointer for pfRdFpga_ActivateField. */
    PHBAL_REG_RDFPGAV6_DEACTIVATEFIELD                      pfRdFpga_DeactivateField;                   /**< function pointer for pfRdFpga_DeactivateField. */
    PHBAL_REG_RDFPGAV6_HFPAUSE                              pfRdFpga_HFPause;                           /**< function pointer for pfRdFpga_HFPause. */
    PHBAL_REG_RDFPGAV6_SET_POTIPOS                          pfRdFpga_SetPotiPos;                        /**< function pointer for pfRdFpga_SetPotiPos. */
    PHBAL_REG_RDFPGAV6_SET_DEFAULTPOTIPOS                   pfRdFpga_SetDefaultPotiPos;                 /**< function pointer for pfRdFpga_SetDefaultPotiPos. */
    PHBAL_REG_RDFPGAV6_GET_POTIPOS                          pfRdFpga_GetPotiPos;                        /**< function pointer for pfRdFpga_GetPotiPos. */
    PHBAL_REG_RDFPGAV6_INC_POTIPOS                          pfRdFpga_IncPotiPos;                        /**< function pointer for pfRdFpga_IncPotiPos. */
    PHBAL_REG_RDFPGAV6_DEC_POTIPOS                          pfRdFpga_DecPotiPos;                        /**< function pointer for pfRdFpga_DecPotiPos. */
    PHBAL_REG_RDFPGAV6_SET_TIMEOUTVALUE                     pfRdFpga_SetTimeoutValue;                   /**< function pointer for pfRdFpga_SetTimeoutValue. */
    PHBAL_REG_RDFPGAV6_GET_TIMEOUTVALUE                     pfRdFpga_GetTimeoutValue;                   /**< function pointer for pfRdFpga_GetTimeoutValue. */
    PHBAL_REG_RDFPGAV6_LOAD_PATTERN                         pfRdFpga_LoadPattern;                       /**< function pointer for pfRdFpga_LoadPattern. */
    PHBAL_REG_RDFPGAV6_PLAY_PATTERN                         pfRdFpga_PlayPattern;                       /**< function pointer for pfRdFpga_PlayPattern. */
    PHBAL_REG_RDFPGAV6_PLAYSEQA                             pfRdFpga_PlaySeqA;                          /**< function pointer for pfRdFpga_PlaySeqA. */
    PHBAL_REG_RDFPGAV6_PLAYSEQB                             pfRdFpga_PlaySeqB;                          /**< function pointer for pfRdFpga_PlaySeqB. */
    PHBAL_REG_RDFPGAV6_PLAYSINUS                            pfRdFpga_PlaySinus;                         /**< function pointer for pfRdFpga_PlaySinus. */
    PHBAL_REG_RDFPGAV6_SELECT_TRIGGER                       pfRdFpga_SelectTrigger;                     /**< function pointer for pfRdFpga_SelectTrigger. */
    PHBAL_REG_RDFPGAV6_LBLRST                               pfRdFpga_LabelReset;                        /**< function pointer for pfRdFpga_LabelReset. */
    PHBAL_REG_RDFPGAV6_LBLRST_COUNTERRESET                  pfRdFpga_LabelResetCounterReset;            /**< function pointer for pfRdFpga_LabelResetCounterReset. */
    PHBAL_REG_RDFPGAV6_LBLRST_SSCOUNTERRESET                pfRdFpga_LabelResetCounterResetSingleShot;  /**< function pointer for pfRdFpga_LabelResetCounterResetSingleShot. */
    PHBAL_REG_RDFPGAV6_LBLRST_SINGLESHOT                    pfRdFpga_LabelResetSingleShot;              /**< function pointer for pfRdFpga_LabelResetSingleShot. */
    PHBAL_REG_RDFPGAV6_LBLRST_WITHCOMMAND                   pfRdFpga_LabelResetWithCommand;             /**< function pointer for pfRdFpga_LabelResetWithCommand. */
    PHBAL_REG_RDFPGAV6_ACT_FDT_MEASUREMENT                  pfRdFpga_ActivateFDTMeasurement;            /**< function pointer for pfRdFpga_ActivateFDTMeasurement. */
    PHBAL_REG_RDFPGAV6_GET_FDT                              pfRdFpga_GetFDT;                            /**< function pointer for pfRdFpga_GetFDT. */
    PHBAL_REG_RDFPGAV6_ACT_ANTITEARINGMODE                  pfRdFpga_ActivateAntiTearingMode;           /**< function pointer for pfRdFpga_ActivateAntiTearingMode. */
    PHBAL_REG_RDFPGAV6_RESONANCE_FREQ_TEST                  pfRdFpga_ResonanceFrequenceTest;            /**< function pointer for pfRdFpga_ResonanceFrequenceTest. */
    PHBAL_REG_RDFPGAV6_ACT_FDT_PICC_TO_PCD_MEASUREMENT      pfRdFpga_ActivateFDTMeasurementPICCtoPCD;   /**< function pointer for pfRdFpga_ActivateFDTMeasurementPICCtoPCD. */
    PHBAL_REG_RDFPGAV6_ACT_LMA_MEASUREMENT                    pfRdFpga_ActivateLMAMeasurement;            /**< function pointer for pfRdFpga_ActivateLMAMeasurement. */
    PHBAL_REG_RDFPGAV6_ACT_SEQUENCERMODE                    pfRdFpga_ActivateSequencerMode;             /**< function pointer for pfRdFpga_ActivateSequencerMode. */
    PHBAL_REG_RDFPGAV6_DEACTIVATE_MODES                        pfRdFpga_DeactivateModes;                   /**< function pointer for pfRdFpga_DeactivateModes. */
    PHBAL_REG_RDFPGAV6_SET_JOINER_RXGAIN                    pfRdFpga_SetJoinerRxGain;                   /**< function pointer for pfRdFpga_SetJoinerRxGain. */
    PHBAL_REG_RDFPGAV6_GET_JOINER_RXGAIN                    pfRdFpga_GetJoinerRxGain;                   /**< function pointer for pfRdFpga_GetJoinerRxGain. */
    PHBAL_REG_RDFPGAV6_ACTIVEDS_SET_FDT                     pfRdFpga_SetFDTPCDforActiveDS;              /**< function pointer for pfRdFpga_SetFDTPCDforActiveDS. */
    PHBAL_REG_RDFPGAV6_DS_SET_FDT                           pfRdFpga_SetFDTPCDforDS;                    /**< function pointer for pfRdFpga_SetFDTPCDforDS. */
    PHBAL_REG_RDFPGAV6_GET_VERSION                          pfRdFpga_GetVersion;                        /**< function pointer for pfRdFpga_GetVersion. */
    PHBAL_REG_RDFPGAV6_GET_REVISION                         pfRdFpga_GetRevision;                       /**< function pointer for pfRdFpga_GetRevision. */
    PHBAL_REG_RDFPGAV6_GET_ERROR_CODES                      pfRdFpga_GetErrorCodes;                     /**< function pointer for pfRdFpga_GetErrorCodes. */
    PHBAL_REG_RDFPGAV6_SET_CLOCK_SOURCE                     pfRdFpga_SetClockSource;                    /**< function pointer for pfRdFpga_SetClockSource. */
    PHBAL_REG_RDFPGAV6_GET_FIRST_RESPONSE                   pfRdFpga_GetFirstResponse;                  /**< function pointer for pfRdFpga_GetFirstResponse. */
    PHBAL_REG_RDFPGAV6_GET_RESPONSE                         pfRdFpga_GetResponse;                       /**< function pointer for pfRdFpga_GetResponse. */
    PHBAL_REG_RDFPGAV6_GET_IQ_TIMEOUTVALUE                  pfRdFpga_GetIQTimeoutValue;                 /**< function pointer for pfRdFpga_GetIQTimeoutValue. */
    PHBAL_REG_RDFPGAV6_SET_IQ_TIMEOUTVALUE                  pfRdFpga_SetIQTimeoutValue;                 /**< function pointer for pfRdFpga_SetIQTimeoutValue. */
    PHBAL_REG_RDFPGAV6_GET_TERMINALID                       pfRdFpga_GetTerminalId;                     /**< function pointer for pfRdFpga_GetTerminalId. */
    PHBAL_REG_RDFPGAV6_RESET_JOINER                         pfRdFpga_ResetJoiner;                       /**< function pointer for pfRdFpga_ResetJoiner. */
} phbalReg_RdFpgaV6_DataParams_t;

/**
* \brief Initialize the RDFPGA BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_USE_CONDITION Required dependencies not available.
*/
phStatus_t phbalReg_RdFpgaV6_Init(
                                  phbalReg_RdFpgaV6_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wSizeOfDataParams                    /**< [In] Specifies the size of the data parameter structure. */
                                  );

/** @} \endcond */
#endif /* NXPBUILD__PHBAL_REG_RDFPGAV6 */

#ifdef NXPBUILD__PHBAL_REG_RD70XUSBWIN

/** \defgroup phbalReg_Rd70xUsbWin Component : Rd70x USB (Windows)
* \brief Rd70x BAL (Windows)
* @{
*/

#define PHBAL_REG_RD70X_USBWIN_ID         0x05U       /**< ID for PEGODA USB component */

/**
* \brief Rd70x USB (Windows) BAL parameter structure
*/
typedef struct
{
    uint16_t wId;           /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint8_t ucTxSeq;        /**< Sequence counter for packets. */
    uint8_t * pDeviceName;  /**< Name of currently open USB device. */
    void * pDeviceHandle;   /**< Handle to the USB device. */
    void * pPipeOut;        /**< Handle to Usb Out-pipe */
    void * pPipeIn;         /**< Handle to Usb In-pipe. */
    uint16_t wTimeoutWrMs;  /**< TO value for Usb Write pipe transfer. */
    uint16_t wTimeoutRdMs;  /**< TO value for Usb Read pipe transfer. */
} phbalReg_Rd70xUsbWin_DataParams_t;

/**
* \brief Initialize the PEGODA USB BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_Rd70xUsbWin_Init(
                                     phbalReg_Rd70xUsbWin_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                     uint16_t wSizeOfDataParams                         /**< [In] Specifies the size of the data parameter structure. */
                                     );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_RD70XUSBWIN */

#ifdef NXPBUILD__PHBAL_REG_PCSCWIN

/** \defgroup phbalReg_PcscWin Component : PCSC (Windows)
* \brief PC/SC BAL Windows
*
* Smartcard Interface.
* @{
*/

#define PHBAL_REG_PCSCWIN_ID            0x06U       /**< ID for PCSC BAL component (Win32) */

/** \name Custom Option Bits
*
* Can be used for \ref phhalHw_Exchange.
*/
/*@{*/
/**
* \brief Calls \b SCardControl with given parameters.
*
* Make sure \b pTxBuffer contents are as follows:
\code
        pTxBuffer[0]               pTxBuffer[1]               pTxBuffer[2]               pTxBuffer[3]           [pTxBuffer[4-n]]
+--------------------------+--------------------------+--------------------------+--------------------------+----------------------+
| dwControlCode(bit 31-24) | dwControlCode(bit 23-16) |  dwControlCode(bit 15-8) |  dwControlCode(bit 7-0)  |       [Data]         |
+--------------------------+--------------------------+--------------------------+--------------------------+----------------------+
\endcode
*/
#define PHBAL_REG_PCSCWIN_OPTION_SCARDCONTROL   0x0010U
/*@}*/

/**
* \brief PCSC (Windows) BAL parameter structure
*/
typedef struct
{
    uint16_t wId;           /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint8_t bProtocolType;  /**< Define protocol type of contact interface. */
    uint8_t bShare;         /**< Define share type of PCSC interface. */
    uint8_t * pAtrBuffer;   /**< Pointer to buffer used for storing the ATR after card activation. */
    uint16_t wAtrBufSize;   /**< Size of the ATR buffer. */
    void * hContext;        /**< Context for SmartCard class. */
    void * hCard;           /**< Handle of card. */
    uint8_t * pReaderName;  /**< Name of the reader. */
    uint16_t wTimeoutWrMs;  /**< TO value for Write transfer. */
    uint16_t wTimeoutRdMs;  /**< TO value for Read transfer. */
} phbalReg_PcscWin_DataParams_t;

/**
* \name BAL Communication Configs
*/
/*@{*/
#define PHBAL_REG_PCSCWIN_CONFIG_PROTOCOL   (PH_CONFIG_CUSTOM_BEGIN)    /**< Used Protocol; e.g. #PHBAL_REG_PCSCWIN_VALUE_PROTOCOL_T0. */
#define PHBAL_REG_PCSCWIN_CONFIG_SHARE      (PH_CONFIG_CUSTOM_BEGIN+1)  /**< Used Access mode; e.g. #PHBAL_REG_PCSCWIN_VALUE_SHARE_SHARED. */
/*@}*/

/**
* \name BAL Communication Configuration values
*/
/*@{*/
#define PHBAL_REG_PCSCWIN_VALUE_PROTOCOL_T0         0x0000U /**< T=0 protocol. */
#define PHBAL_REG_PCSCWIN_VALUE_PROTOCOL_T1         0x0001U /**< T=1 protocol. */
#define PHBAL_REG_PCSCWIN_VALUE_PROTOCOL_UNDEFINED  0x0002U /**< UNDEFINED protocol. */
#define PHBAL_REG_PCSCWIN_VALUE_SHARE_SHARED        0x0000U /**< SHARED access. */
#define PHBAL_REG_PCSCWIN_VALUE_SHARE_EXCLUSIVE     0x0001U /**< EXCLUSIVE access. */
#define PHBAL_REG_PCSCWIN_VALUE_SHARE_DIRECT        0x0002U /**< DIRECT access. */
/*@}*/

/**
* \brief Initialize the PCSC BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER \b wAtrBufSize is invalid.
*/
phStatus_t phbalReg_PcscWin_Init(
                                 phbalReg_PcscWin_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                                 uint8_t * pAtrBuffer,                          /**< [In] Pointer to buffer used for storing the ATR after card activation. */
                                 uint16_t wAtrBufSize                           /**< [In] Size of the ATR buffer. */
                                 );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_PCSCWIN */

#ifdef NXPBUILD__PHBAL_REG_PIPELIN

/** \cond INCLUDE_INTERNALS \defgroup phbalReg_PipeLin Component : Pipe (Linux)
* \brief Pipe BAL (Unix/Linux)
*
* Interfaces NCSim Simulation over Pipes.
* @{
*/

#define PHBAL_REG_PIPELIN_ID            0x07U       /**< ID for Pipe Linux */

#define PHBAL_REG_PIPELIN_MODE_RAW      0x00U   /**< RAW exchange mode. Exchanges null-terminated strings. */
#define PHBAL_REG_PIPELIN_MODE_RS232    0x01U   /**< RS232 exchange mode according to Maat DigiSpec; Exchanges binary data. */
#define PHBAL_REG_PIPELIN_MODE_I2C      0x02U   /**< I2C exchange mode according to Maat DigiSpec; Exchanges binary data. */
#define PHBAL_REG_PIPELIN_MODE_SPI      0x03U   /**< SPI exchange mode according to Maat DigiSpec; Exchanges binary data. */

/**
* \name PipeLin specific configs
*/
/*@{*/
#define PHBAL_REG_PIPELIN_CONFIG_MODE               (PH_CONFIG_CUSTOM_BEGIN)    /**< Set operating mode. (e.g. #PHBAL_REG_PIPELIN_MODE_RS232). */
#define PHBAL_REG_PIPELIN_CONFIG_BITRATE            (PH_CONFIG_CUSTOM_BEGIN+1)  /**< Configure communication speed. */
#define PHBAL_REG_PIPELIN_CONFIG_I2C_SLAVE_ADDR     (PH_CONFIG_CUSTOM_BEGIN+2)  /**< Configure slave address for I2C communication mode. */
#define PHBAL_REG_PIPELIN_CONFIG_SETCARD            (PH_CONFIG_CUSTOM_BEGIN+3)  /**< Set card configuration for simulation. */
#define PHBAL_REG_PIPELIN_CONFIG_RESET                (PH_CONFIG_CUSTOM_BEGIN+4)  /**< Reset the simulation. */
/*@}*/

/**
* \name PipeLin Bitrate Values
*/
/*@{*/
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_9600          0x0000U /**< 9600 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_19200         0x0001U /**< 19200 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_38400         0x0002U /**< 38400 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_57600         0x0003U /**< 57600 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_115200        0x0004U /**< 115200 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_128000        0x0005U /**< 128000 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_230400        0x0006U /**< 230400 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_460800        0x0007U /**< 460800 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_921600        0x0008U /**< 921600 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_RS232_1228800       0x0009U /**< 1228800 kBit/s RS232. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_SPI_10K             0x0010U /**< 10 kBit/s SPI. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_SPI_100K            0x0011U /**< 100 kBit/s SPI. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_SPI_1M              0x0012U /**< 1 MBit/s SPI. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_SPI_3M              0x0013U /**< 3 MBit/s SPI. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_SPI_5M              0x0014U /**< 5 MBit/s SPI. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_SPI_8M              0x0015U /**< 8 MBit/s SPI. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_I2C_10K             0x0020U /**< 10 KBit/s I2C. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_I2C_100K            0x0021U /**< 100 KBit/s I2C. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_I2C_200K            0x0022U /**< 200 KBit/s I2C. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_I2C_400K            0x0023U /**< 400 KBit/s I2C. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_I2CL_1M             0x0044U /**< 1 MBit/s I2CL. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_I2CL_3M             0x0045U /**< 3 MBit/s I2CL. */
#define PHBAL_REG_PIPELIN_VALUE_BITRATE_I2CL_5M             0x0046U /**< 5 MBit/s I2CL. */
/*@}*/

/**
* \name PipeLin Bitrate Values
*
* Note: Set in MSB(yte), they set the TX-Mode, in the LSB the RX-Mode.
*/
/*@{*/
#define PHBAL_REG_PIPELIN_TX_TWO_CARDS          0x80U   /**< Set this in the MSB(yte) to have two cards instead of one. */
#define PHBAL_REG_PIPELIN_RXTX_MIFARE_106       0x00U   /**< MIFARE(R) Operating mode at 106kbit/s. */
#define PHBAL_REG_PIPELIN_RXTX_MIFARE_212       0x01U   /**< MIFARE(R) Operating mode at 212kbit/s. */
#define PHBAL_REG_PIPELIN_RXTX_MIFARE_424       0x02U   /**< MIFARE(R) Operating mode at 414kbit/s. */
#define PHBAL_REG_PIPELIN_RXTX_MIFARE_848       0x03U   /**< MIFARE(R) Operating mode at 848kbit/s. */

#define PHBAL_REG_PIPELIN_RXTX_TYPEB_106        0x04U   /**< ISO14443B Operating mode at 106kbit/s. */
#define PHBAL_REG_PIPELIN_RXTX_TYPEB_212        0x05U   /**< ISO14443B Operating mode at 212kbit/s. */
#define PHBAL_REG_PIPELIN_RXTX_TYPEB_424        0x06U   /**< ISO14443B Operating mode at 414kbit/s. */
#define PHBAL_REG_PIPELIN_RXTX_TYPEB_848        0x07U   /**< ISO14443B Operating mode at 848kbit/s. */

#define PHBAL_REG_PIPELIN_RXTX_FELICA_212       0x08U   /**< FeliCa Operating mode at 212kbit/s. */
#define PHBAL_REG_PIPELIN_RXTX_FELICA_424       0x09U   /**< FeliCa Operating mode at 424kbit/s. */

#define PHBAL_REG_PIPELIN_RXTX_I15693_1OO4      0x0AU   /**< ISO15693 One-Out-Of-Four Operating mode. */
#define PHBAL_REG_PIPELIN_RXTX_I15693_1OO256    0x0BU   /**< ISO15693 One-Out-Of-256 Operating mode. */

#define PHBAL_REG_PIPELIN_RXTX_EPCV2_424_DUAL   0x0FU   /**< ICode EPC V2 424 kBit/s (2 Subcarriers). */
#define PHBAL_REG_PIPELIN_RXTX_EPCV2_424_QUAD   0x10U   /**< ICode EPC V2 424 kBit/s (4 Subcarriers). */
#define PHBAL_REG_PIPELIN_RXTX_EPCV2_848_DUAL   0x11U   /**< ICode EPC V2 848 kBit/s (2 Subcarriers). */
#define PHBAL_REG_PIPELIN_RXTX_EPCV2_848_QUAD   0x12U   /**< ICode EPC V2 848 kBit/s (4 Subcarriers). */
/*@}*/

/**
* \brief BAL PipeLin parameter structure
*/
typedef struct
{
    uint16_t wId;           /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint8_t bMode;          /**< Operating mode. */
    uint8_t * pPipeNameRx;  /**< Identifier of the named Rx-Pipe. */
    uint8_t * pPipeNameTx;  /**< Identifier of the named Tx-Pipe. */
    void * pPipeRx;         /**< Pointer to RX communication pipe. */
    void * pPipeTx;         /**< Pointer to TX communication pipe. */
    uint8_t * pTxBuffer;    /**< Pointer to global transmit buffer used by the Exchange() function. */
    uint16_t wTxBufSize;    /**< Size of the global transmit buffer. */
    uint8_t bI2cSlaveAddr;  /**< I2C slave address. */
    uint8_t bConfigValue;   /**< current configuration value. */
} phbalReg_PipeLin_DataParams_t;

/**
* \brief Initialize the BAL PipeLinux layer.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER \b bMode or \b wTxBufSize is invalid.
*/
phStatus_t phbalReg_PipeLin_Init(
                                 phbalReg_PipeLin_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                 uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                                 uint8_t bMode,                                 /**< [In] Initial operating mode; \see #PHBAL_REG_PIPELIN_CONFIG_MODE. */
                                 uint16_t wTxBufSize,                           /**< [In] Size of the global transmit buffer. */
                                 uint8_t * pTxBuffer                            /**< [In] Pointer to global transmit buffer used by the Exchange() function. */
                                 );

/** @} \endcond */
#endif /* NXPBUILD__PHBAL_REG_PIPELIN */

#ifdef NXPBUILD__PHBAL_REG_STUB

/** \defgroup phbalReg_Stub Component : Stub
* \brief Component without functionality to ease implementation of additional busses.
* @{
*/

#define PHBAL_REG_STUB_ID               0x08U       /**< ID for Stub BAL component */

/**
* \brief BAL Stub parameter structure
*/
typedef struct
{
    uint16_t       wId;         /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint16_t       wHalType;    /**< HAL HW type (Rc523, Rc663, ... ) */
    uint8_t        bMultiRegRW; /**< Flag to indicate Multiple Register Read or Write. */
} phbalReg_Stub_DataParams_t;

/**
* \brief Initialize the BAL Stub layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_Stub_Init(
                              phbalReg_Stub_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wSizeOfDataParams                /**< [In] Specifies the size of the data parameter structure. */
                              );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_STUB */

#ifdef NXPBUILD__PHBAL_REG_SOCKETWIN

/** \cond INCLUDE_INTERNALS \defgroup phbalReg_SocketWin Component : Socket (Windows)
* \brief Socket BAL (Windows)
*
* Interfaces NCSim Simulation over Network Sockets.
* @{
*/

#define PHBAL_REG_SOCKETWIN_ID        0x09U       /**< ID for SOCKETWIN component */

/**
* \name SocketWin specific configs
*/
/*@{*/
#define PHBAL_REG_SOCKETWIN_CONFIG_MODE             (PH_CONFIG_CUSTOM_BEGIN)    /**< Set operating mode. (e.g. #PHBAL_REG_SOCKETWIN_MODE_RS232). */
#define PHBAL_REG_SOCKETWIN_CONFIG_BITRATE          (PH_CONFIG_CUSTOM_BEGIN+1)  /**< Configure communication speed. */
#define PHBAL_REG_SOCKETWIN_CONFIG_I2C_SLAVE_ADDR   (PH_CONFIG_CUSTOM_BEGIN+2)  /**< Configure slave address for I2C communication mod. */
#define PHBAL_REG_SOCKETWIN_CONFIG_SETCARD          (PH_CONFIG_CUSTOM_BEGIN+3)  /**< Set card configuration for simulation. */
#define PHBAL_REG_SOCKETWIN_CONFIG_RESET            (PH_CONFIG_CUSTOM_BEGIN+4)  /**< Reset the simulation. */
#define PHBAL_REG_SOCKETWIN_CONFIG_TERMINATION_CHARACTER     (PH_CONFIG_CUSTOM_BEGIN+5)  /**< Termination Character for mode PHBAL_REG_SOCKETWIN_MODE_RAW. */
/*@}*/

/**
* \name SocketWin operation mode values
*/
/*@{*/
#define PHBAL_REG_SOCKETWIN_MODE_RAW    0x00U   /**< RAW exchange mode. Exchanges null-terminated strings. */
#define PHBAL_REG_SOCKETWIN_MODE_RS232  0x01U   /**< RS232 exchange mode according to Maat DigiSpec; Exchanges binary data. */
#define PHBAL_REG_SOCKETWIN_MODE_I2C    0x02U   /**< I2C exchange mode according to Maat DigiSpec; Exchanges binary data. */
#define PHBAL_REG_SOCKETWIN_MODE_SPI    0x03U   /**< SPI exchange mode according to Maat DigiSpec; Exchanges binary data. */
#define PHBAL_REG_SOCKETWIN_MODE_PN512_ON_PN54X_TB    0x04U   /**< PN512/RC523 on PN548 TCL testbench mode. */
#define PHBAL_REG_SOCKETWIN_MODE_PN517_ON_PN54X_TB    0x05U   /**< PN517/518 on PN548 TCL testbench mode. */
/*@}*/

/**
* \name SocketWin Bitrate Values
*/
/*@{*/
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_9600        0x0000U /**< 9600 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_19200       0x0001U /**< 19200 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_38400       0x0002U /**< 38400 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_57600       0x0003U /**< 57600 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_115200      0x0004U /**< 115200 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_128000      0x0005U /**< 128000 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_230400      0x0006U /**< 230400 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_460800      0x0007U /**< 460800 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_921600      0x0008U /**< 921600 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_RS232_1228800     0x0009U /**< 1228800 kBit/s RS232. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_SPI_10K           0x0010U /**< 10 kBit/s SPI. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_SPI_100K          0x0011U /**< 100 kBit/s SPI. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_SPI_1M            0x0012U /**< 1 MBit/s SPI. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_SPI_3M            0x0013U /**< 3 MBit/s SPI. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_SPI_5M            0x0014U /**< 5 MBit/s SPI. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_SPI_8M            0x0015U /**< 8 MBit/s SPI. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_I2C_10K           0x0020U /**< 10 KBit/s I2C. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_I2C_100K          0x0021U /**< 100 KBit/s I2C. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_I2C_200K          0x0022U /**< 200 KBit/s I2C. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_I2C_400K          0x0023U /**< 400 KBit/s I2C. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_I2CL_1M           0x0044U /**< 1 MBit/s I2CL. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_I2CL_3M           0x0045U /**< 3 MBit/s I2CL. */
#define PHBAL_REG_SOCKETWIN_VALUE_BITRATE_I2CL_5M           0x0046U /**< 5 MBit/s I2CL. */
/*@}*/

/**
* \name SocketWin Card Values
*
* Note: Set in MSB(yte), they set the TX-Mode, in the LSB the RX-Mode.
*/
/*@{*/
#define PHBAL_REG_SOCKETWIN_TX_TWO_CARDS            0x80U   /**< Set this in the MSB(yte) to have two cards instead of one. */
#define PHBAL_REG_SOCKETWIN_RXTX_MIFARE_106         0x00U   /**< MIFARE(R) Operating mode at 106kbit/s. */
#define PHBAL_REG_SOCKETWIN_RXTX_MIFARE_212         0x01U   /**< MIFARE(R) Operating mode at 212kbit/s. */
#define PHBAL_REG_SOCKETWIN_RXTX_MIFARE_424         0x02U   /**< MIFARE(R) Operating mode at 424kbit/s. */
#define PHBAL_REG_SOCKETWIN_RXTX_MIFARE_848         0x03U   /**< MIFARE(R) Operating mode at 848kbit/s. */

#define PHBAL_REG_SOCKETWIN_RXTX_TYPEB_106          0x04U   /**< ISO14443B Operating mode at 106kbit/s. */
#define PHBAL_REG_SOCKETWIN_RXTX_TYPEB_212          0x05U   /**< ISO14443B Operating mode at 212kbit/s. */
#define PHBAL_REG_SOCKETWIN_RXTX_TYPEB_424          0x06U   /**< ISO14443B Operating mode at 424kbit/s. */
#define PHBAL_REG_SOCKETWIN_RXTX_TYPEB_848          0x07U   /**< ISO14443B Operating mode at 848kbit/s. */

#define PHBAL_REG_SOCKETWIN_RXTX_FELICA_212         0x08U   /**< FeliCa Operating mode at 212kbit/s. */
#define PHBAL_REG_SOCKETWIN_RXTX_FELICA_424         0x09U   /**< FeliCa Operating mode at 424kbit/s. */

#define PHBAL_REG_SOCKETWIN_RXTX_I15693_1OO4        0x0AU   /**< ISO15693 One-Out-Of-Four Operating mode. */
#define PHBAL_REG_SOCKETWIN_RXTX_I15693_1OO256      0x0BU   /**< ISO15693 One-Out-Of-256 Operating mode. */

#define PHBAL_REG_SOCKETWIN_RXTX_EPCV2_424_DUAL     0x0FU   /**< ICode EPC V2 424 kBit/s (2 Subcarriers). */
#define PHBAL_REG_SOCKETWIN_RXTX_EPCV2_424_QUAD     0x10U   /**< ICode EPC V2 424 kBit/s (4 Subcarriers). */
#define PHBAL_REG_SOCKETWIN_RXTX_EPCV2_848_DUAL     0x11U   /**< ICode EPC V2 848 kBit/s (2 Subcarriers). */
#define PHBAL_REG_SOCKETWIN_RXTX_EPCV2_848_QUAD     0x12U   /**< ICode EPC V2 848 kBit/s (4 Subcarriers). */
/*@}*/

/**
* \brief SocketWin(Windows) BAL parameter structure
*/
typedef struct
{
    uint16_t wId;               /**< Layer ID for this BAL component, NEVER MODIFY! */
    void * pWsaData;            /**< Pointer to WsaData buffer. */
    uint8_t * pSocketHostName;  /**< Socket Host Name String e.g. "localhost". */
    uint8_t * pSocketPortName;  /**< Socket Port Name String e.g. "1234". */
    uint8_t * pUserName;        /**< User Name String e.g. "nxp12607". */
    uint32_t dwServerSocket;    /**< Server Socket ID. */
    uint8_t * pTxBuffer;        /**< Pointer to global transmit buffer used by the Exchange() function. */
    uint16_t wTxBufSize;        /**< Size of the global transmit buffer. */
    uint8_t bMode;              /**< Operating mode. */
    uint8_t bConfigValue;       /**< Current configuration value. */
    uint8_t bI2cSlaveAddr;      /**< I2C slave address. */
} phbalReg_SocketWin_DataParams_t;

/**
* \brief Initialize the Socket Win BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER \b bMode or \b wTxBufSize is invalid.
* \retval #PH_ERR_BUFFER_OVERFLOW \b pWsaBuffer size is too small.
*/
phStatus_t phbalReg_SocketWin_Init(
                                   phbalReg_SocketWin_DataParams_t * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                                   uint16_t wSizeOfDataParams,                      /**< [In] Specifies the size of the data parameter structure. */
                                   uint8_t bMode,                                   /**< [In] Initial operating mode; \see #PHBAL_REG_SOCKETWIN_CONFIG_MODE. */
                                   uint16_t wTxBufSize,                             /**< [In] Size of the global transmit buffer. */
                                   uint8_t * pTxBuffer,                             /**< [In] Pointer to global transmit buffer used by the Exchange() function. */
                                   uint16_t wWsaBufferSize,                         /**< [In] Size of the Windsocket WSADATA structure buffer; Refer to WinSock documentation (should be about 400 bytes). */
                                   uint8_t * pWsaBuffer                             /**< [In] Pointer to the Winsocket WSADATA structure buffer. */
                                   );

/** @} \endcond */
#endif /* NXPBUILD__PHBAL_REG_SOCKETWIN */

#ifdef NXPBUILD__PHBAL_REG_RD710SAM

/** \defgroup phbalReg_Rd710Sam Component : Rd710 SAM
* \brief Rd710 SAM BAL
*
* Implements SAM interface for Direct-Mode Communication.\n
* Needs to be linked between SAM HAL and the BAL that interfaces with the Rd710.
* @{
*/

#define PHBAL_REG_RD710_SAM_ID         0x0AU       /**< ID for RD710_SAM component */

/**
* \brief RD710Sam (Windows) BAL parameter structure
*/
typedef struct
{
    uint16_t wId;               /**< Layer ID for this BAL component, NEVER MODIFY! */
    void * pLowerBalDataParams; /**< Pointer to the BAL which actually connects to Rd710 in direct mode. */
    uint8_t * pAtrBuffer;       /**< Pointer to buffer used for storing the ATR after card activation. */
    uint16_t wAtrBufSize;       /**< Size of the ATR buffer. */
    uint8_t * pTxBuffer;        /**< Pointer to global transmit buffer used by the Exchange() function. */
    uint16_t wTxBufSize;        /**< Size of the global transmit buffer. */
    uint8_t * pRxBuffer;        /**< Pointer to global receive buffer used by the Exchange() function. */
    uint16_t wRxBufSize;        /**< Size of the global receive buffer. */
    uint8_t bCardOpened;        /**< Indicator if card was already opened. */
} phbalReg_Rd710Sam_DataParams_t;

/**
* \brief Initialize the RD710SAM BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
* \retval #PH_ERR_INVALID_PARAMETER \b wAtrBufSize, \b wTxBufSize or \b wRxBufSize is invalid.
*/
phStatus_t phbalReg_Rd710Sam_Init(
                                  phbalReg_Rd710Sam_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                  uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
                                  void * pLowerBalDataParams,                   /**< [In] Pointer to the BAL which actually connects to Rd710 in direct mode. */
                                  uint8_t * pAtrBuffer,                         /**< [In] Pointer to buffer used for storing the ATR after card activation. */
                                  uint16_t wAtrBufSize,                         /**< [In] Size of the ATR buffer. */
                                  uint8_t * pTxBuffer,                          /**< [In] Pointer to global transmit buffer used by the Exchange() function. */
                                  uint16_t wTxBufSize,                          /**< [In] Size of the global transmit buffer. */
                                  uint8_t * pRxBuffer,                          /**< [In] Pointer to global receive buffer used by the Exchange() function. */
                                  uint16_t wRxBufSize                           /**< [In] Size of the global receive buffer. */
                                  );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_RD710SAM */

#ifdef NXPBUILD__PHBAL_REG_ABEND
/** \defgroup phbalReg_Abend Component : Abend
* \brief Abend BAL
*
* Implements Abend for communication with ARMBoard.
* @{
*/

#define PHBAL_REG_ABEND_ID         0x0BU       /**< ID for RD710_SAM component */

/**
* \brief Abend BAL parameter structure
*/
typedef struct
{
    uint16_t wId;               /**< Layer ID for this BAL component, NEVER MODIFY! */
#ifdef _WIN32
    HINSTANCE pLibrary;         /**< Pointer to the ARMBoard.dll library. */
#endif
#ifndef _WIN32
    void*     pFwCtx;            /**< Internal firmware context of ABEND secondary firmware */
#endif
    void * pDutTx;              /**< Pointetr to the Abend Tx call. */
    void * pDutRx;              /**< Pointetr to the Abend Rx call. */
    void * pAbendHandle;        /**< Pointer to Abend Handle. */
    void * pDutGetDevInfo;      /**< Pointer to get dev info */
    void * pDutDevIoCtl;        /**< Pointer to IO ctl */
    uint8_t * pLpcId;            /**< LPC ID */
#ifdef _WIN32
    HANDLE hdlIrqThread;         /**< Thread checking state of IRQ pin */
    DWORD dwThreadID;            /**< ID of the IRQ check thread */
    phOsal_EventObj_t BalEventObj;	/**< Bal Event Object. */
    phOsal_MutexObj_t BalMutexObj; /**< Mutex protecting concurrent access to ABEND functions */
    volatile uint8_t bIrqThreadStopRequested;   /**< flag indicating that stop is requested for IRQ thread */
#endif
} phbalReg_Abend_DataParams_t;

/**
* \brief Initialize the Abend BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_Abend_Init(
                               phbalReg_Abend_DataParams_t * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                               uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
#ifdef _WIN32
                               uint8_t * pArmBoardDllLocationString           /**< [In] Location of ABEND dll.  Example: "lib/ArmBoard.dll" */
#endif
#ifndef _WIN32
                               void*     pFwCtx                               /**< [In] Internal firmware context of ABEND secondary firmware */
#endif
                               );


/** @} */
#endif /* NXPBUILD__PHBAL_REG_ABEND */

#ifdef NXPBUILD__PHBAL_REG_ABENDSECFW
/** \defgroup phbalReg_Abend Component : Abend
* \brief Abend BAL
*
* Implements Abend for communication with ARMBoard.
* @{
*/

#define PHBAL_REG_ABENDSECFW_ID         0x0CU       /**< ID for Abend Sec FW BAL */

/**
* \brief Abend BAL parameter structure
*/
typedef struct
{
    uint16_t wId;               /**< Layer ID for this BAL component, NEVER MODIFY! */
    HINSTANCE pLibrary;         /**< Pointer to the ARMBoard.dll library. */
    void* pSecFwStart;              /**< Pointetr Secondarf Firmware Start call. */
    void* pSecFwStop;              /**< Pointetr Secondary Firmware stop call. */
    void* pSecFwTRx;              /**< Pointetr to the Secondary Firmware TRxcall. */
    void * pAbendHandle;        /**< Pointer to Abend Handle. */
    uint8_t * pLpcId;            /**< LPC ID */
} phbalReg_AbendSecFw_DataParams_t;

/**
* \brief Initialize the Abend Secondary Firmware BAL component.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_AbendSecFw_Init(
                               phbalReg_AbendSecFw_DataParams_t * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                               uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                               uint8_t * pArmBoardDllLocationString           /**< [In] Location of ABEND dll.  Example: "lib/ArmBoard.dll" */
                               );


/** @} */
#endif /* NXPBUILD__PHBAL_REG_ABENDSECFW */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_SPI

/** \defgroup phbalReg_LpcOpenSpi Component : LpcOpenSpi
* \brief Component abstracting the functionality of LPC Open stack SPI interface.
* @{
*/

#define PHBAL_REG_LPCOPEN_SPI_ID               0x0DU       /**< ID for LPC Open SPI BAL component */

/**
* \brief LPC Open SPI BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_LpcOpenSpi_DataParams_t;

/**
* \brief Initialize the LPC Open SPI BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LpcOpenSpi_Init(
                                    phbalReg_LpcOpenSpi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#ifdef NXPBUILD__PHBAL_REG_LPCOPEN_I2C

/** \defgroup phbalReg_LpcOpenI2c Component : LpcOpenI2c
* \brief Component abstracting the functionality of LPC Open stack I2C interface
* @{
*/

#define PHBAL_REG_LPCOPEN_I2C_ID               0x0EU       /**< ID for LPC Open I2C BAL component */

/**
* \brief LPC Open I2C BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_LpcOpenI2c_DataParams_t;


/**
* \brief Initialize the LPC Open I2C BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LpcOpenI2c_Init(
                                    phbalReg_LpcOpenI2c_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#ifdef NXPBUILD__PHBAL_REG_KINETIS_SPI

/** \defgroup phbalReg_KinetisSpi Component : KinetisSpi
* \brief Component abstracting the functionality of Kinetis KSDK 2.0 SPI interface.
* @{
*/

#define PHBAL_REG_KINETIS_SPI_ID               0x0FU       /**< ID for Kinetis SPI BAL component */

/**
* \brief Kinetis SPI BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_KinetisSpi_DataParams_t;

/**
* \brief Initialize the Kinetis SPI BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_KinetisSpi_Init(
                                    phbalReg_KinetisSpi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_KINETIS_SPI */


#ifdef NXPBUILD__PHBAL_REG_KINETIS_I2C

/** \defgroup phbalReg_KinetisI2C Component : KinetisI2C
* \brief Component abstracting the functionality of Kinetis KSDK 2.0 I2C interface.
* @{
*/

#define PHBAL_REG_KINETIS_I2C_ID               0x10U       /**< ID for Kinetis I2C BAL component */

/**
* \brief Kinetis I2C BAL parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;
} phbalReg_KinetisI2C_DataParams_t;

/**
* \brief Initialize the Kinetis I2C BAL layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_KinetisI2C_Init(
                                    phbalReg_KinetisI2C_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                                    uint16_t wSizeOfDataParams                      /**< [In] Specifies the size of the data parameter structure. */
                                    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_KINETIS_I2C */

#ifdef NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI

/** \defgroup phbalReg_LinuxKernelSpi Component : Linux Kernel Spi
* \brief Component without functionality to ease implementation of additional busses.
* @{
*/

#define PHBAL_REG_LINUX_KERNEL_SPI_ID               0x11U       /**< ID for Linux Kernel Spi BAL component */


typedef struct
{
    /* File descriptors */
    uint32_t spiFd;
}phbalReg_LinuxKernelSpiObj_t;

/**
* \brief BAL Linux Kernel Spi parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;         /**< Layer ID for this BAL component, NEVER MODIFY! */
    phbalReg_LinuxKernelSpiObj_t sSpiObj;
} phbalReg_LinuxKernelSpi_DataParams_t;

/**
* \brief Initialise the BAL Linux Kernel Spi layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LinuxKernelSpi_Init(
    phbalReg_LinuxKernelSpi_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams                          /**< [In] Specifies the size of the data parameter structure. */
);

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */


#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_SPI

/** \defgroup phbalReg_LinuxUserSpi Component : Linux User Spi
* \brief Component without functionality to ease implementation of additional busses.
* @{
*/

#define PHBAL_REG_LINUX_USER_SPI_ID               0x12U       /**< ID for Linux User Spi BAL component */

typedef struct
{
    /* Pin/bus numbers */
    uint32_t spiBus;
    uint32_t spiCsPinNum;

    /* File descriptors */
    uint32_t spiFd;
}phbalReg_LinuxUserSpiObj_t;

/**
* \brief BAL Linux User Spi parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;         /**< Layer ID for this BAL component, NEVER MODIFY! */
    phbalReg_LinuxUserSpiObj_t sSpiObj;
} phbalReg_LinuxUserSpi_DataParams_t;

/**
* \brief Initialise the BAL Linux User Spi layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LinuxUserSpi_Init(
    phbalReg_LinuxUserSpi_DataParams_t * pDataParams,
    uint16_t wSizeOfDataParams
    );


/** @} */
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#ifdef NXPBUILD__PHBAL_REG_LINUX_USER_I2C

/** \defgroup phbalReg_LinuxUserI2C Component : Linux User I2C
* \brief Component without functionality to ease implementation of additional busses.
* @{
*/

#define PHBAL_REG_LINUX_USER_I2C_ID               0x13U       /**< ID for Linux User I2C BAL component */

typedef struct
{
    /* Pin number */
    uint32_t I2CNum;

    /* File descriptors */
    uint32_t I2CFd;
}phbalReg_LinuxUserI2CObj_t;

/**
* \brief BAL Linux User I2C parameter structure
*/
typedef struct
{
    phbalReg_Type_t sBal_Struct;         /**< Layer ID for this BAL component, NEVER MODIFY! */
    phbalReg_LinuxUserI2CObj_t sI2CObj;
} phbalReg_LinuxUserI2C_DataParams_t;

/**
* \brief Initialise the BAL Linux User I2C layer.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Parameter structure size is invalid.
*/
phStatus_t phbalReg_LinuxUserI2C_Init(
    phbalReg_LinuxUserI2C_DataParams_t * pDataParams,
    uint16_t wSizeOfDataParams
    );

/** @} */
#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

#ifdef NXPBUILD__PHBAL_REG_DELEGATE

#define PHBAL_REG_DELEGATE_ID               0xDEU       /**< ID for Delegate BAL component */

typedef phStatus_t ( __stdcall * fpphbalReg_Delegate_OpenPort) (
    void * pCtx
);

typedef phStatus_t ( __stdcall * fpphbalReg_Delegate_ClosePort) (
    void * pCtx
);

typedef phStatus_t ( __stdcall * fpphbalReg_Delegate_Exchange) (
    void * pCtx,
    uint16_t wOption,
    uint8_t * pTxBuffer,
    uint16_t wTxLength,
    uint16_t wRxBufSize,
    uint8_t * pRxBuffer,
    uint16_t * pRxLength
);

typedef phStatus_t ( __stdcall * fpphbalReg_Delegate_SetConfig) (
    void * pCtx,
    uint16_t wConfig,
    uint16_t wValue
);

typedef phStatus_t ( __stdcall * fpphbalReg_Delegate_GetConfig) (
    void * pCtx,
    uint16_t wConfig,
    uint16_t * pValue
);

typedef struct
{
    uint16_t                        wId;              /**< Layer ID for this BAL component, NEVER MODIFY! */
    uint8_t                         bBalType;         /**< BAL type used by HAL to configure the BAL configured at runtime. */
    uint8_t                         pad0;             /**< For 32 bit alignment */
    void *                          pCtx;
    fpphbalReg_Delegate_OpenPort    fpOpenPort;
    fpphbalReg_Delegate_ClosePort   fpClosePort;
    fpphbalReg_Delegate_Exchange    fpExchange;
    fpphbalReg_Delegate_SetConfig   fpSetConfig;
    fpphbalReg_Delegate_GetConfig   fpGetConfig;
} phbalReg_Delegate_DataParams_t;

phStatus_t phbalReg_Delegate_Init(
    phbalReg_Delegate_DataParams_t * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
    uint16_t wSizeOfDataParams,                   /**< [In] Specifies the size of the data parameter structure. */
    void *                          pCtx,
    fpphbalReg_Delegate_OpenPort    fpOpenPort,
    fpphbalReg_Delegate_ClosePort   fpClosePort,
    fpphbalReg_Delegate_Exchange    fpExchange,
    fpphbalReg_Delegate_SetConfig   fpSetConfig,
    fpphbalReg_Delegate_GetConfig   fpGetConfig
);

#endif /* NXPBUILD__PHBAL_REG_DELEGATE */

#ifdef NXPBUILD__PHBAL_REG

/** \defgroup phbalReg Bus Abstraction Layer
* \brief These components implement the interfaces between physical host-device and physical reader-device.
* @{
*/

/**
* \name Generic BAL configuration parameters
*/
/*@{*/
#define PHBAL_REG_CONFIG_WRITE_TIMEOUT_MS   0x0000U /**< Configure transmission timeout in milliseconds. */
#define PHBAL_REG_CONFIG_READ_TIMEOUT_MS    0x0001U /**< Configure reception timeout in milliseconds. */

/**
* \name BAL types
*/
/*@{*/
#define PHBAL_REG_TYPE_SPI                  0x0001U /**< Configure transmission timeout in milliseconds. */
#define PHBAL_REG_TYPE_I2C                  0x0002U /**< Configure reception timeout in milliseconds. */
#define PHBAL_REG_TYPE_SERIAL_WIN           0x0003U /**< Configure reception timeout in milliseconds. */

/*@}*/

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LPCOPEN_SPI)

#include "../comps/phbalReg/src/LpcOpenSPI/phbalReg_LpcOpenSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
        phbalReg_LpcOpenSpi_GetPortList((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LpcOpenSpi_SetPort((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LpcOpenSpi_OpenPort((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LpcOpenSpi_ClosePort((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LpcOpenSpi_Exchange((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LpcOpenSpi_SetConfig((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LpcOpenSpi_GetConfig((phbalReg_LpcOpenSpi_DataParams_t*)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LPCOPEN_I2C)

#include "../comps/phbalReg/src/LpcOpenI2C/phbalReg_LpcOpenI2c.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
        phbalReg_LpcOpenI2c_GetPortList((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LpcOpenI2c_SetPort((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LpcOpenI2c_OpenPort((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LpcOpenI2c_ClosePort((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LpcOpenI2c_Exchange((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LpcOpenI2c_SetConfig((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LpcOpenI2c_GetConfig((phbalReg_LpcOpenI2c_DataParams_t*)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LPCOPEN_I2C */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LINUX_USER_SPI)

#include "../comps/phbalReg/src/LinuxUserSpi/phbalReg_LinuxUserSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
    phbalReg_LinuxUserSpi_GetPortList((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LinuxUserSpi_SetPort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LinuxUserSpi_OpenPort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LinuxUserSpi_ClosePort((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LinuxUserSpi_Exchange((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LinuxUserSpi_SetConfig((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LinuxUserSpi_GetConfig((phbalReg_LinuxUserSpi_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI)

#include "../comps/phbalReg/src/LinuxKernelSpi/phbalReg_LinuxKernelSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
    phbalReg_LinuxKernelSpi_GetPortList((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LinuxKernelSpi_SetPort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LinuxKernelSpi_OpenPort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LinuxKernelSpi_ClosePort((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LinuxKernelSpi_Exchange((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LinuxKernelSpi_SetConfig((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LinuxKernelSpi_GetConfig((phbalReg_LinuxKernelSpi_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_LINUX_USER_I2C)

#include "../comps/phbalReg/src/LinuxUserI2C/phbalReg_LinuxUserI2C.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
    phbalReg_LinuxUserI2C_GetPortList((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wPortBufSize, pPortNames, pNumOfPorts)

#define phbalReg_SetPort(pDataParams, pPortName) \
        phbalReg_LinuxUserI2C_SetPort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, pPortName)

#define phbalReg_OpenPort(pDataParams) \
        phbalReg_LinuxUserI2C_OpenPort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
        phbalReg_LinuxUserI2C_ClosePort((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
        phbalReg_LinuxUserI2C_Exchange((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
        phbalReg_LinuxUserI2C_SetConfig((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
        phbalReg_LinuxUserI2C_GetConfig((phbalReg_LinuxUserI2C_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_USER_I2C */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_KINETIS_SPI)

#include "../comps/phbalReg/src/KinetisSPI/phbalReg_KinetisSpi.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
		phbalReg_KinetisSpi_GetPortList((phbalReg_KinetisSpi_DataParams_t *)pDataParams)

#define phbalReg_SetPort(pDataParams, pPortName) \
		phbalReg_KinetisSpi_SetPort((phbalReg_KinetisSpi_DataParams_t *)pDataParams)

#define phbalReg_OpenPort(pDataParams) \
		phbalReg_KinetisSpi_OpenPort((phbalReg_KinetisSpi_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
		phbalReg_KinetisSpi_ClosePort((phbalReg_KinetisSpi_DataParams_t *) pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
		phbalReg_KinetisSpi_Exchange((phbalReg_KinetisSpi_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize,pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
		phbalReg_KinetisSpi_SetConfig((phbalReg_KinetisSpi_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
		phbalReg_KinetisSpi_GetConfig((phbalReg_KinetisSpi_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#if defined(NXPRDLIB_REM_GEN_INTFS) && defined (NXPBUILD__PHBAL_REG_KINETIS_I2C)

#include "../comps/phbalReg/src/KinetisI2C/phbalReg_KinetisI2C.h"

#define phbalReg_GetPortList(pDataParams, wPortBufSize, pPortNames, pNumOfPorts) \
		phbalReg_KinetisI2C_GetPortList((phbalReg_KinetisI2C_DataParams_t *)pDataParams)

#define phbalReg_SetPort(pDataParams, pPortName) \
		phbalReg_KinetisI2C_SetPort((phbalReg_KinetisI2C_DataParams_t *)pDataParams)

#define phbalReg_OpenPort(pDataParams) \
		phbalReg_KinetisI2C_OpenPort((phbalReg_KinetisI2C_DataParams_t *)pDataParams)

#define phbalReg_ClosePort(pDataParams) \
		phbalReg_KinetisI2C_ClosePort((phbalReg_KinetisI2C_DataParams_t *) pDataParams)

#define phbalReg_Exchange(pDataParams, wOption, pTxBuffer, wTxLength, wRxBufSize, pRxBuffer, pRxLength) \
		phbalReg_KinetisI2C_Exchange((phbalReg_KinetisI2C_DataParams_t *)pDataParams, pTxBuffer, wTxLength, wRxBufSize,pRxBuffer, pRxLength)

#define phbalReg_SetConfig(pDataParams, wConfig, wValue) \
		phbalReg_KinetisI2C_SetConfig((phbalReg_KinetisI2C_DataParams_t *)pDataParams, wConfig, wValue)

#define phbalReg_GetConfig(pDataParams, wConfig, pValue) \
		phbalReg_KinetisI2C_GetConfig((phbalReg_KinetisI2C_DataParams_t *)pDataParams, wConfig, pValue)

#endif /* NXPBUILD__PHBAL_REG_LINUX_KERNEL_SPI */

#ifndef NXPRDLIB_REM_GEN_INTFS

/**
* \brief List all available ports.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_BUFFER_OVERFLOW Given enumeration buffer is too small.
* \retval #PH_ERR_INTERFACE_ERROR Error while enumerating devices.
*/
phStatus_t phbalReg_GetPortList(
                                void * pDataParams,     /**< [In] Pointer to this layer's parameter structure. */
                                uint16_t wPortBufSize,  /**< [In] Buffer Size of Port Name String. */
                                uint8_t * pPortNames,   /**< [Out] Port Name as Multi-String. */
                                uint16_t * pNumOfPorts  /**< [Out] Number of found port strings. */
                                );

/**
* \brief Select port to be used.
*
* <em>Example SerialWin:</em> The caller has to ensure that \c pPortName
* is valid throughout the whole lifetime of \c pDataParams.\n
* Furthermore, the caller is responsible for prepending "\\.\" if COM ports above
* COM9 need to be accessed.\n\n
* \b Example:
\code
strcpy(pPortName, "COM9");         <-- correct
strcpy(pPortName, "\\\\.\\COM9");  <-- correct
strcpy(pPortName, "\\\\.\\COM10"); <-- correct
strcpy(pPortName, "COM10");        <-- wrong
\endcode
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phbalReg_SetPort(
                            void * pDataParams, /**< [In] Pointer to this layer's parameter structure. */
                            uint8_t * pPortName /**< [In] Port Name as String. */
                            );

/**
* \brief Open communication port.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION Communication port is already open.
* \retval #PH_ERR_INTERFACE_ERROR Error while opening port.
*/
phStatus_t phbalReg_OpenPort(
                             void * pDataParams /**< [In] Pointer to this layer's parameter structure. */
                             );

/**
* \brief Close communication port.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_USE_CONDITION Communication port is not open.
* \retval #PH_ERR_INTERFACE_ERROR Error while closing port.
*/
phStatus_t phbalReg_ClosePort(
                              void * pDataParams    /**< [In] Pointer to this layer's parameter structure. */
                              );

/**
* \brief Perform data exchange on the bus. This API is used for command exchange between front-end CLIF
* and HOST.
* SPI : This API will also perform required flow control for particular front-end.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER \b wOption is invalid.
* \retval #PH_ERR_IO_TIMEOUT No response received within given time frame.
* \retval #PH_ERR_BUFFER_OVERFLOW Response is too big for either given receive buffer or internal buffer.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phbalReg_Exchange(
                             void * pDataParams,    /**< [In] Pointer to this layer's parameter structure. */
                             uint16_t wOption,      /**< [In] Option parameter. */
                             uint8_t * pTxBuffer,   /**< [In] Data to transmit. */
                             uint16_t wTxLength,    /**< [In] Number of bytes to transmit. */
                             uint16_t wRxBufSize,   /**< [In] Size of receive buffer / Number of bytes to receive (depending on implementation). */
                             uint8_t * pRxBuffer,   /**< [Out] Received data. */
                             uint16_t * pRxLength   /**< [Out] Number of received data bytes. */
                             );

/**
* \brief Set configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INVALID_PARAMETER Parameter value is invalid.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phbalReg_SetConfig(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wConfig,     /**< [In] Configuration identifier. */
                              uint16_t wValue       /**< [In] Configuration value. */
                              );
/**
* \brief Get configuration parameter.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_UNSUPPORTED_PARAMETER Configuration is not supported or invalid.
* \retval #PH_ERR_INTERFACE_ERROR Communication error.
*/
phStatus_t phbalReg_GetConfig(
                              void * pDataParams,   /**< [In] Pointer to this layer's parameter structure. */
                              uint16_t wConfig,     /**< [In] Configuration identifier. */
                              uint16_t * pValue     /**< [Out] Configuration value. */
                              );

/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHBAL_REG */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHBALREG_H */
