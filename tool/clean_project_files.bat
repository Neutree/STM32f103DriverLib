
title 删除工程的中间文件
mode con cols=100 lines=30


COLOR  3F
cd ..

del JLinkLog.txt /s
del JLinkSettings.ini /s  don't delete jlink configuration file
del *.guild_log.htm /s
del *.uvguix.* /s
del *_uvoptx.bak /s
del *_uvprojx.bak /s
del *.dep /s
del *.uvoptx /s
del *.*.bak /s



del *.crf  /s
del  *.d  /s
del  *.o  /s
del  *.iex  /s
del  *.axf  /s
del  *.build_log.htm  /s
del  *.lnp  /s
del  *.sct  /s
del  *.htm  /s
del  *.lst  /s
del  *.map  /s

del  *.hex  /s
del RTE_Device.h.000* /s
del *.dbgconf /s

# delete empty folders
for /f "delims=" %%a in ('dir . /b /ad /s ^|sort /r' ) do rd /q "%%a" 2>nul

