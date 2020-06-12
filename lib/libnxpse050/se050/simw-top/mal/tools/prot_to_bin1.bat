
setlocal
set _filename=%~n1
set _extension=%~x1
set _pathname=%~dp1

echo pathname is %_pathname%

python cgtParser.py %1 ^
    %_pathname%%_filename%_proto.text ^
    %_pathname%%_filename%_mals.jcsh


@REM We use protoc to convert a data to binary
set PROTOC="C:\_ddm\iot\iot-tooldisk\nanopb\generator-bin\protoc.exe"

set PROTO_FILES_DIR=%~dp0..\..\nxp_iot_agent\doc\protobuf

cd /d %PROTO_FILES_DIR%

@REM Grammer comes from the *.proto files, data from _proto.txt, and then we generate a .bin file
call %PROTOC% --encode=nxp.iot.Requests Dispatcher.proto < %_pathname%%_filename%_proto.text > %_pathname%%_filename%_out_proto.bin

@REM Just for testing, we convert that .bin to text again.... just for testing
call %PROTOC% --decode=nxp.iot.Requests Dispatcher.proto < %_pathname%%_filename%_out_proto.bin >  %_pathname%%_filename%_out_proto_r.text

cd /d %~dp0

@REM That bin file is now converted to a hex array that can be used for testing
python binToHexArray.py %_pathname%%_filename%_out_proto.bin %_pathname%%_filename%_out_proto.hex.h
endlocal