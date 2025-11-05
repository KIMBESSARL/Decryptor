:: A3 SCRIPT
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
echo '-  [A3: Coder - Team: Kingsly / Jatin]    -'
echo '--------------------------------------------'
set "arg=%1"
::set "arg=%1"
if "%arg%"=="" set "arg=CODED.txt"
gcc compilers.c main1coder.c step1coder.c main2reader.c step2reader.c main3scanner.c step3scanner.c -o Decriptor
timeout /t 1 /nobreak >nul
::Decriptor 1 1 PROL.txt CODED.txt
Decriptor 3 %arg% > out.txt 2> err.txt
timeout /t 1 /nobreak >nul
dir out.txt err.txt
type out.txt 