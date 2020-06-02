/* Copyright 2018,2020 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/*  Standard C Included Files */
#include <stdio.h>
#include <string.h>

#include "sm_types.h"
#include "sm_printf.h"

#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include "app_boot.h"
#include "test_T1oI2C_lessDataRead.h"
#include "global_platf.h"

#include "phNxpEseProto7816_3.h"
#include "phNxpEsePal_i2c.h"

#include "sm_apdu.h"
#include "sm_errors.h"

#include "a71ch_api.h"
#include "tst_sm_time.h"
#include "smComT1oI2C.h"
#include "sm_timer.h"
#if USE_RTOS
#include "FreeRTOS.h"
#include "task.h"
//#include "sm_demo_utils.h"
#endif

#define NX_LOG_ENABLE_APP_DEBUG 1
#include "nxLog_App.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EX_APP_VERSION "1.30:1.31" // App-version:Preferred-applet-version
#define MAX_DAT_LEN 256            // max short apdu lenth
#define APDU_HEADER 0x04

#if USE_RTOS
#define T1oI2C_TASK_PRIORITY (tskIDLE_PRIORITY)
#define T1oI2C_TASK_STACK_SIZE 11000
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern int app_boot_Init();
static U32 open_session();
static U32 close_session();
static uint16_t ComputeCRC(unsigned char *p_buff, uint32_t offset, uint32_t length);
static int Read_frame(bool_t flag);
static bool_t Decode_frame(uint8_t *p_data);
static ESESTATUS send_Iframe(uint8_t *APDU_header, uint8_t *APDU_data, uint8_t APDU_data_len);
/*******************************************************************************
 * Variables
 ******************************************************************************/
static uint8_t pcb_bit = 0;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */

int main()
{
    /* TODO - IC Boot*/
    //U8 result = 1;

#if defined(__gnu_linux__) || defined(_WIN32)
    time_t now;
#endif
#ifdef TDA8029_UART
    Scp03SessionState_t T1oi2cState;
#endif

#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
    app_boot_Init();

    sm_printf(DBGOUT, "T1oI2C protocol Incomplete Data Read test example application (Rev %s)\r\n", EX_APP_VERSION);
    sm_printf(DBGOUT, "**********************************************\r\n");
    sm_printf(DBGOUT, "Connect to A71CH-SM/SE050. Chunksize at link layer = %d.\r\n", MAX_CHUNK_LENGTH_LINK);

    APP_BOOT_RESUME_HANDLING(exBoot);

    /* Run RTOS */
    //    vTaskStartScheduler();

#if USE_RTOS
    //creat task
    if (xTaskCreate(
            &test_T1oi2c_IncompleteDataRead, "test_T1oi2c", T1oI2C_TASK_STACK_SIZE, NULL, T1oI2C_TASK_PRIORITY, NULL) !=
        pdPASS) {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }

    /* Run RTOS */
    vTaskStartScheduler();

#else
    test_T1oi2c_IncompleteDataRead();
#endif

#if USE_RTOS

    /* Should not reach this statement */
    for (;;)
        ;
#endif

    return 0;
}

void test_T1oi2c_IncompleteDataRead()
{
    U32 iCount = 1;
    U32 loop = 0;
    ESESTATUS ret = ESESTATUS_FAILED;
    /* /card APDU header */
    uint8_t CARD_header[] = {0x00, 0xA4, 0x04, 0x00}; /*CLA INS P1 P2*/
    uint8_t APDU_data[] = {0x00, 0x01, 0x02, 0x03};   /**/
    uint8_t APDU_data_len = sizeof(APDU_data);        /*LC*/

    //U8 PASS=0,FAIL =0;

#if defined(AX_EMBEDDED) || defined(ANDROID)

    if (SMCOM_OK != open_session()) {
        LOG_E("Unable to open the session");
#if defined(USE_RTOS) && USE_RTOS == 1
        assert(0);
#else
        //FAIL++;
        goto exit;
#endif
    }
    /*first send /card */
    ret = send_Iframe(CARD_header, APDU_data, 0);
    if (ret != ESESTATUS_SUCCESS) {
        LOG_E("Fail to get /card response\n");
        goto exit;
    }
    Read_frame(FALSE);
    /*Send Incomplete I frame multiple times and check response*/
    do {
        for (iCount = 1; iCount < 4; iCount++) {
            LOG_W("sending Incomplete APDU");
            ret = send_Iframe(CARD_header, APDU_data, 0);
            if (ret != ESESTATUS_SUCCESS) {
                LOG_E("Test Fails\n");
                goto exit;
            }
            Read_frame(TRUE);
            phNxpEse_clearReadBuffer(NULL);
        }

        ret = send_Iframe(CARD_header, APDU_data, APDU_data_len - 2);
        if (ret != ESESTATUS_SUCCESS) {
            LOG_E("Test Fails\n");
            goto exit;
        }
        Read_frame(TRUE);
        phNxpEse_clearReadBuffer(NULL);
        loop++;
    } while (loop < 4);
    /*send Valid APDU frame and Read complete frame*/
    ret = send_Iframe(CARD_header, APDU_data, 0);
    if (ret != ESESTATUS_SUCCESS) {
        LOG_E("Fail to get /card response\n");
        goto exit;
    }
    Read_frame(FALSE);
    close_session(); //close previously opened Session

exit:

    LOG_I("****************************** T1oI2C protocol Negative test Completed  ******************************\r\n");
    LOG_I(" Press Reset to restart \r\n");

#if USE_RTOS
    /* This task has finished.  FreeRTOS does not allow a task to run off the
        * end of its implementing function, so the task must be deleted. */
    vTaskDelete(NULL);
#endif
#endif
}

static U32 open_session()
{
    U32 ret = 0;
    U8 Atr[64];
    U16 AtrLen = sizeof(Atr);
    ret = smComT1oI2C_Open(NULL, ESE_MODE_NORMAL, 0x00, Atr, &AtrLen);
    if (ret != SMCOM_OK)
        return ret;
    LOG_I("SE session opened \r\n");
    return ret;
}

static U32 close_session()
{
    U32 ret = 0;
    ret = smComT1oI2C_Close(NULL, SMCOM_CLOSE_MODE_STD);
    if (ret != SMCOM_OK)
        return ret;
    LOG_I("SE T1oi2c closed \r\n");
    return ret;
}

static uint16_t ComputeCRC(unsigned char *p_buff, uint32_t offset, uint32_t length)
{
    uint16_t CRC = 0xFFFF, i = 0;
    for (i = offset; i < length; i++) {
        CRC ^= p_buff[i];
        for (int bit = 8; bit > 0; --bit) {
            if ((CRC & 0x0001) == 0x0001) {
                CRC = (unsigned short)((CRC >> 1) ^ 0x8408);
            }
            else {
                CRC >>= 1;
            }
        }
    }
    CRC ^= 0xFFFF;
#if defined(T1oI2C_UM11225)
    uint8_t new_byte[2] = {0};
    new_byte[0] = (CRC & 0xFF);
    new_byte[1] = ((CRC >> 8) & 0xFF);
    CRC = new_byte[0] << 8 | new_byte[1];
#endif
    return (uint16_t)CRC;
}

static int Read_frame(bool_t flag)
{
    uint8_t pBuffer[256] = {0};
    int sof_counter = 0;
    int nNbBytesToRead = 0, total_count = 0;
    int numBytesToRead =0;
    //memset(pBuffer,0,256);
    int ret = -1;

    do {
        sof_counter++;
        ret = -1;
        ret = phPalEse_i2c_read(NULL, &pBuffer[0], 2);
        if (ret < 0) {
            /*Polling for read on spi, hence Debug log*/
            LOG_D("_i2c_read() [HDR]errno : %x ret : %X", errno, ret);
        }
        else if (pBuffer[0] == 0xA5) {
            /* Read the HEADR of one byte*/
            LOG_D("%s Read HDR", __FUNCTION__);
            break;
        }

    } while (sof_counter < 200);
    if (pBuffer[0] == 0xA5) {
        LOG_I("%s SOF FOUND", __FUNCTION__);
#if defined(T1oI2C_UM11225)
            numBytesToRead = 1;
#elif defined(T1oI2C_GP1_0)
            numBytesToRead = 2;
#endif
        /* Read the HEADR of one/Two bytes based on how two bytes read A5 PCB or 00 A5*/
        ret = phPalEse_i2c_read(NULL, &pBuffer[2], numBytesToRead);
#if defined(T1oI2C_UM11225)
        total_count = 3;
        nNbBytesToRead = pBuffer[2];
#elif defined(T1oI2C_GP1_0)
        total_count = 4;
        nNbBytesToRead = (pBuffer[2] << 8 & 0xFF) | (pBuffer[3] & 0xFF) ;
#endif
        /* Read the Complete data + two byte CRC*/
        if (flag)
            /*read 2 bytes less data*/
            ret = phPalEse_i2c_read(NULL, &pBuffer[PH_PROTO_7816_HEADER_LEN], (nNbBytesToRead + PH_PROTO_7816_CRC_LEN) - 2);
        else
            ret = phPalEse_i2c_read(NULL, &pBuffer[PH_PROTO_7816_HEADER_LEN], (nNbBytesToRead + PH_PROTO_7816_CRC_LEN));

        if (ret < 0) {
            LOG_D("_i2c_read() [HDR]errno : %x ret : %X", errno, ret);
            return -1;
        }
        LOG_MAU8_D("Rx->", pBuffer, total_count + ret);
        Decode_frame(pBuffer);
    }
    else {
        ret = -1;
    }
    return ret;
}

static bool_t Decode_frame(uint8_t *p_data)
{
    bool_t status = TRUE;
    uint8_t pcb;
    //uint8_t sw[2]={0};
    phNxpEseProto7816_PCB_bits_t pcb_bits;
    pcb = p_data[PH_PROPTO_7816_PCB_OFFSET];
    phNxpEse_memset(&pcb_bits, 0x00, sizeof(phNxpEseProto7816_PCB_bits_t));
    phNxpEse_memcpy(&pcb_bits, &pcb, sizeof(uint8_t));
    if (0x00 == pcb_bits.msb) {
        LOG_I("%s I-Frame Received ", __FUNCTION__);
        /*update sequence counter for next I frame*/
        pcb_bit ^= 1;
    }
    else if ((0x01 == pcb_bits.msb) && (0x00 == pcb_bits.bit7)) /* R-FRAME decoded should come here */
    {
        LOG_I("%s R-Frame Received", __FUNCTION__);
        if ((pcb_bits.lsb == 0) && (pcb_bits.bit2 == 0))
            LOG_E("Error Free Acknowledgement");
        if ((pcb_bits.lsb == 1) && (pcb_bits.bit2 == 0))
            LOG_E("CRC Error");
        if ((pcb_bits.lsb == 0) && (pcb_bits.bit2 == 1)) {
            /*retransmit previously Tx frame with same sequence no*/
            LOG_E("Other Error");
            LOG_I("Retransmit previously Tx frame with same Sequence no.");
        }
        if ((pcb_bits.lsb == 1) && (pcb_bits.bit2 == 1))
            LOG_E("NAD Error (Unsupported SAD/DAD)");
    }
    else if ((0x01 == pcb_bits.msb) && (0x01 == pcb_bits.bit7)) /* S-FRAME decoded should come here */
    {
        LOG_I("%s S-Frame Received", __FUNCTION__);
    }
    else {
        LOG_I("%s Unknown frame", __FUNCTION__);
    }

    return status;
}

static ESESTATUS send_Iframe(uint8_t *APDU_header, uint8_t *APDU_data, uint8_t APDU_data_len)
{
    /*send Incomplete I frame data*/
    ESESTATUS status = ESESTATUS_FAILED;
    uint8_t APDU_HEADER_LEN = 0x05; /*CLA INS P1 P2 LC*/

    //uint8_t le_byte = sizeof(APDU_data);/*LE*/

    uint32_t frame_len = 0;
    uint8_t pcb_byte = 0;
    uint8_t p_framebuff[256] = {0};
    uint16_t calc_crc = 0;
    /*Send only NAD PCB CRC make LEN=0*/
    frame_len = (PH_PROTO_7816_HEADER_LEN + PH_PROTO_7816_CRC_LEN + APDU_HEADER_LEN + APDU_data_len);

    /* frame the packet */
    p_framebuff[PH_PROPTO_7816_NAD_OFFSET] = SEND_PACKET_SOF; /* NAD Byte */

    /* Update the send seq no */
    pcb_byte |= (pcb_bit << 6);

    /* store the pcb byte */
    p_framebuff[PH_PROPTO_7816_PCB_OFFSET] = pcb_byte;
    /* store I frame length */
    p_framebuff[PH_PROPTO_7816_LEN_UPPER_OFFSET] = APDU_data_len + APDU_HEADER_LEN;
#if defined(T1oI2C_GP1_0)
    p_framebuff[PH_PROPTO_7816_LEN_LOWER_OFFSET] = p_framebuff[PH_PROPTO_7816_LEN_UPPER_OFFSET];
    p_framebuff[PH_PROPTO_7816_LEN_UPPER_OFFSET] = 0;
#endif
    memcpy(&p_framebuff[PH_PROTO_7816_HEADER_LEN], APDU_header, 4);
#if defined(T1oI2C_UM11225)
    p_framebuff[7] = APDU_data_len; /*copy LC byte data*/
    if (APDU_data_len != 0) {
        memcpy(&p_framebuff[8], APDU_data, APDU_data_len); /*copy APDU data*/
    }
#elif defined(T1oI2C_GP1_0)
    p_framebuff[8] = APDU_data_len; /*copy LC byte data*/
    if (APDU_data_len != 0) {
        memcpy(&p_framebuff[9], APDU_data, APDU_data_len); /*copy APDU data*/
    }
#endif
    //p_framebuff[frame_len-3] = le_byte;

    calc_crc = ComputeCRC(p_framebuff, 0, (frame_len - 2));

    p_framebuff[frame_len - 2] = (calc_crc >> 8) & 0xff;
    p_framebuff[frame_len - 1] = calc_crc & 0xff;
    status = phNxpEse_WriteFrame(NULL, frame_len, p_framebuff);
    if (ESESTATUS_SUCCESS != status) {
        LOG_E("%s Error WriteFrame ", __FUNCTION__);
        return status;
    }
    LOG_MAU8_D("Tx->", p_framebuff, frame_len);

    return ESESTATUS_SUCCESS;
}
