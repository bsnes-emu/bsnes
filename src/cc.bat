::@make -r PLATFORM=win-visualc-lui
@make -r PLATFORM=win-visualc-lui GZIP_SUPPORT=true JMA_SUPPORT=true
@move bsnes.exe ../bsnes.exe>nul
@pause