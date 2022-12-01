@echo off
echo                     This program will create Ace Boot Disk. 
echo  		Press any key to continue or Ctrl+C to exit...
pause >> NUL
echo Writting boot code ::
makeboot boot.bin a: >> NUL
echo Copying OS Loader ...
copy osloader.bin a: /v >> NUL
echo Copying Kernel ... 
copy kernel.bin a: /v >> NUL
echo done.