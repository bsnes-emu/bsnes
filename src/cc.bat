@make -r PLATFORM=win-mingw-x86
::@make -r PLATFORM=win-mingw-x86 GZIP_SUPPORT=true JMA_SUPPORT=true
::@make -r PLATFORM=win-visualc-x86
::@make -r PLATFORM=win-visualc-x86 GZIP_SUPPORT=true JMA_SUPPORT=true
@move bsnes.exe ../bsnes.exe>nul
@pause