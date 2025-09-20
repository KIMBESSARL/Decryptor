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
echo '-  [A1: Coder - Team: Kingsly / Jatin    -'
echo '--------------------------------------------'
set "arg=%1"
set "param=2"
if "%arg%"=="" set "arg=A1DEC.txt"
 del A1DEC.txt RESTORED.txt
gcc compilers.c main1coder.c step1coder.c -o Decryptor
timeout /t 1 /nobreak >nul 
 Decryptor 1 1 README.txt %arg% > out.txt 2> err.txt
::timeout /t 1 /nobreak >nul
Decryptor %param% %arg% > out.txt 2> err.txt
timeout /t 1 /nobreak >nul
dir out.txt err.txt
type out.txt