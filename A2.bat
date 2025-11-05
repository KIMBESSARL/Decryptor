:: A1 SCRIPT
echo off
echo '--------------------------------------------'
echo '-      ALGONQUIN COLLEGE - COM - 25F       -'
echo '--------------------------------------------'
echo '-                                          -'
echo '-    ====                                  -'
echo '-   =                                      -'
echo '-  =       ==   = =  ==  = =   =   ==  ==  -'
echo '-   =     =  = = = = = = = =   ==  =  ==   -'
echo '-    ====  ==  = = = =   = === === =   ==  -'
echo '-                                          -'
echo '--------------------------------------------'
echo '-  [A2: Reader - Team: Paulo Sousa / God]  -'
echo '--------------------------------------------'
set "arg=%1"
set "param=2"
if "%arg%"=="" set "arg=CODED.txt"
::if created delete and create a new one
 del CODED.txt RESTORED.txt

gcc compilers.c main1coder.c step1coder.c main2reader.c step2reader.c -o Decryptor
timeout /t 1 /nobreak >nul 

Decryptor 1 1 Big.sof %arg%
timeout /t 1 /nobreak >nul

Decryptor 2 %arg% > out.txt 2> err.txt
timeout /t 1 /nobreak >nul

dir out.txt err.txt
type out.txt
