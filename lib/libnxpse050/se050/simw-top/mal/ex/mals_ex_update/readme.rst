..
    Copyright 2019,2020 NXP

    This software is owned or controlled by NXP and may only be used
    strictly in accordance with the applicable license terms.  By expressly
    accepting such terms or by downloading, installing, activating and/or
    otherwise using the software, you are agreeing that you have read, and
    that you agree to comply with and are bound by, such license terms.  If
    you do not agree to be bound by the applicable license terms, then you
    may not retain, install, activate or otherwise use the software.

.. highlight:: bat

.. _ex-mals-agent-demo:

=======================================================================
 MALS Agent Demo (mals_ex_update)
=======================================================================

This application will update Applet through MALS agent.

In this demo, an update package, is pre-compiled into the example
binary.  See :file:`MALS_UpgrateTo_iotDev-4.4.0-20200128-00.h`.   This file contans a byte array
that is encoded in protobuf format. This
demo will call API :cpp:type:`mal_agent_load_package()` which will decode the
stream and send update commands command to SE.


.. only:: nxp

    .. note:: NXP internal section


    **Generate Request Packages**

    The request packages to be loaded is coded in out_proto.hex.h. It's
    generated from upgrade script which is encoded with protobuf format.
    Tools have been provided in mal/tools for this process. Following is
    steps to generate the package. Refer to simw-top\\mal\\tools\\README.txt for detail.

    - Create a folder for your upgrade script (e.g C:\\test_script)
    - Put your upgrade script into the created folder (C:\\test_script\\01_MALS_IoT_Applet_Load_Install_DAP-encrypted.txt)
    - Set "PROTOC" in simw-top\\mal\\tools\\prot_to_bin1.bat with correct path for protoc.exe
    - Run "simw-top\\mal\\tools\\prot_to_bin_folder.bat" with upgrade script folder as parameter (e.g. prot_to_bin_folder.bat C:\\test_script)
    - Copy the generated *.hex.h file (e.g. 01_MALS_IoT_Applet_Load_Install_DAP-encrypted_out_proto.hex.h) to simw-top\\mal\\demo\update\\ as out_proto.hex.h


Prerequisites
=====================
- Micro USB cable
- Kinetis FRDM-K64F/imx-RT1050 board
- Personal Computer
- SE051 Board
- Build Plug & Trust middleware stack. (Refer :ref:`building`)
- Build Project: ``mals_ex_update``


Running the Example
=======================================================================

- In case the a new update package is available, replace :file:`MALS_UpgrateTo_iotDev-4.4.0-20200128-00.h`
- Recompile the example for your platform
- If you have built a binary for an embedded target,
  flash the ``mals_ex_update`` binary on to the
  board and reset the board.

- If you have built an *exe* to be run from PC using VCOM, run as::

        mals_ex_update.exe <PORT NAME>

  Where **<PORT NAME>** is the VCOM COM port.


