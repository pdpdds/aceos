@echo off
echo                     This program will create Ace Boot Disk. 
echo  		Press any key to continue or Ctrl+C to exit...
pause >> NUL
echo Writting boot code ::
boot\makeboot boot.bin a: 
echo Copying OS Loader ...
copy boot\osloader.bin a: /v >> NUL
echo Copying Kernel ... 
copy kernel.exe a: /v >> NUL
echo done.
