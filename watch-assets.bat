
@echo OFF
REM Copies assets to out directories when files are different.

set fatal=A fatal error occured.
set okCopy=Copied files successfully.
set mismatches=Files mismatched.
set fail=Copy Failed.
set extra=Extra files are located in the destination directory.

set x86DebugMsg=Copying assets to x86-Debug:
set x86ReleaseMsg=Copying assets to x86-Release:
set x64DebugMsg=Copying assets to x64-Debug:
set x64ReleaseMsg=Copying assets to x64-Release:

set itemBorder=-----------------------------------------------------

:loop


timeout /t 1 > NUL
echo|set /p="."
timeout /t 1 > NUL
echo|set /p="."
timeout /t 1 > NUL
echo|set /p="."
timeout /t 1 > NUL
echo|set /p="."
timeout /t 1 > NUL
echo|set /p="."
timeout /t 1 > NUL

cls

echo|set /p="Watching"
set hasCopied=false

robocopy assets out/build/x86-Debug/assets /e /NFL /NDL /NJH /NJS /nc /ns /np > nul
if %ERRORLEVEL% GTR 0 set hasCopied=true & echo. & echo %itemBorder% & echo %x86DebugMsg%
if %ERRORLEVEL% EQU 16 echo %fatal%  & goto 1
if %ERRORLEVEL% EQU 15 echo %okCopy% + %fail% + %mismatches% + %extra%  & goto 1
if %ERRORLEVEL% EQU 14 echo %fail% + %mismatches% + %extra%  & goto 1
if %ERRORLEVEL% EQU 13 echo %okCopy% + %fail% + %mismatches%  & goto 1
if %ERRORLEVEL% EQU 12 echo %fail% + %mismatches%  & goto 1
if %ERRORLEVEL% EQU 11 echo %okCopy% + %fail% + %extra%  & goto 1
if %ERRORLEVEL% EQU 10 echo %fail% + %extra%  & goto 1
if %ERRORLEVEL% EQU 9 echo %okCopy% + %fail%  & goto 1
if %ERRORLEVEL% EQU 8 echo %fail%  & goto 1
if %ERRORLEVEL% EQU 7 echo %okCopy% + %mismatches% + %extra%  & goto 1
if %ERRORLEVEL% EQU 6 echo %mismatches% + %extra%  & goto 1
if %ERRORLEVEL% EQU 5 echo %okCopy% + %mismatches%  & goto 1
if %ERRORLEVEL% EQU 4 echo %mismatches%  & goto 1
if %ERRORLEVEL% EQU 3 echo %okCopy% + %extra%  & goto 1
if %ERRORLEVEL% EQU 2 echo %extra%  & goto 1
if %ERRORLEVEL% EQU 1 echo %okCopy%  & goto 1
if %ERRORLEVEL% EQU 0 goto 1
:1
robocopy assets out/build/x86-Release/assets /e /NFL /NDL /NJH /NJS /nc /ns /np > nul
if %ERRORLEVEL% GTR 0 set hasCopied=true & echo. & echo %itemBorder% &echo %x86ReleaseMsg%
if %ERRORLEVEL% EQU 16 echo %fatal%  & goto 2
if %ERRORLEVEL% EQU 15 echo %okCopy% + %fail% + %mismatches% + %extra%  & goto 2
if %ERRORLEVEL% EQU 14 echo %fail% + %mismatches% + %extra%  & goto 2
if %ERRORLEVEL% EQU 13 echo %okCopy% + %fail% + %mismatches%  & goto 2
if %ERRORLEVEL% EQU 12 echo %fail% + %mismatches%  & goto 2
if %ERRORLEVEL% EQU 11 echo %okCopy% + %fail% + %extra%  & goto 2
if %ERRORLEVEL% EQU 10 echo %fail% + %extra%  & goto 2
if %ERRORLEVEL% EQU 9 echo %okCopy% + %fail%  & goto 2
if %ERRORLEVEL% EQU 8 echo %fail%  & goto 2
if %ERRORLEVEL% EQU 7 echo %okCopy% + %mismatches% + %extra%  & goto 2
if %ERRORLEVEL% EQU 6 echo %mismatches% + %extra%  & goto 2
if %ERRORLEVEL% EQU 5 echo %okCopy% + %mismatches%  & goto 2
if %ERRORLEVEL% EQU 4 echo %mismatches%  & goto 2
if %ERRORLEVEL% EQU 3 echo %okCopy% + %extra%  & goto 2
if %ERRORLEVEL% EQU 2 echo %extra%  & goto 2
if %ERRORLEVEL% EQU 1 echo %okCopy%  & goto 2
if %ERRORLEVEL% EQU 0 goto 2
:2
robocopy assets out/build/x64-Release/assets /e /NFL /NDL /NJH /NJS /nc /ns /np > nul
if %ERRORLEVEL% GTR 0 set hasCopied=true & echo. & echo %itemBorder% & echo %x64ReleaseMsg%
if %ERRORLEVEL% EQU 16 echo %fatal%  & goto 3
if %ERRORLEVEL% EQU 15 echo %okCopy% + %fail% + %mismatches% + %extra%  & goto 3
if %ERRORLEVEL% EQU 14 echo %fail% + %mismatches% + %extra%  & goto 3
if %ERRORLEVEL% EQU 13 echo %okCopy% + %fail% + %mismatches%  & goto 3
if %ERRORLEVEL% EQU 12 echo %fail% + %mismatches%  & goto 3
if %ERRORLEVEL% EQU 11 echo %okCopy% + %fail% + %extra%  & goto 3
if %ERRORLEVEL% EQU 10 echo %fail% + %extra%  & goto 3
if %ERRORLEVEL% EQU 9 echo %okCopy% + %fail%  & goto 3
if %ERRORLEVEL% EQU 8 echo %fail%  & goto 3
if %ERRORLEVEL% EQU 7 echo %okCopy% + %mismatches% + %extra%  & goto 3
if %ERRORLEVEL% EQU 6 echo %mismatches% + %extra%  & goto 3
if %ERRORLEVEL% EQU 5 echo %okCopy% + %mismatches%  & goto 3
if %ERRORLEVEL% EQU 4 echo %mismatches%  & goto 3
if %ERRORLEVEL% EQU 3 echo %okCopy% + %extra%  & goto 3
if %ERRORLEVEL% EQU 2 echo %extra%  & goto 3
if %ERRORLEVEL% EQU 1 echo %okCopy%  & goto 3
if %ERRORLEVEL% EQU 0 goto 3
:3
robocopy assets out/build/x64-Debug/assets /e /NFL /NDL /NJH /NJS /nc /ns /np > nul
if %ERRORLEVEL% GTR 0 set hasCopied=true & echo. & echo %itemBorder% & echo %x64DebugMsg%
if %ERRORLEVEL% EQU 16 echo %fatal% & goto end
if %ERRORLEVEL% EQU 15 echo %okCopy% + %fail% + %mismatches% + %extra% & goto end
if %ERRORLEVEL% EQU 14 echo %fail% + %mismatches% + %extra% & goto end
if %ERRORLEVEL% EQU 13 echo %okCopy% + %fail% + %mismatches% & goto end
if %ERRORLEVEL% EQU 12 echo %fail% + %mismatches% & goto end
if %ERRORLEVEL% EQU 11 echo %okCopy% + %fail% + %extra% & goto end
if %ERRORLEVEL% EQU 10 echo %fail% + %extra% & goto end
if %ERRORLEVEL% EQU 9 echo %okCopy% + %fail% & goto end
if %ERRORLEVEL% EQU 8 echo %fail% & goto end
if %ERRORLEVEL% EQU 7 echo %okCopy% + %mismatches% + %extra% & goto end
if %ERRORLEVEL% EQU 6 echo %mismatches% + %extra% & goto end
if %ERRORLEVEL% EQU 5 echo %okCopy% + %mismatches% & goto end
if %ERRORLEVEL% EQU 4 echo %mismatches% & goto end
if %ERRORLEVEL% EQU 3 echo %okCopy% + %extra% & goto end
if %ERRORLEVEL% EQU 2 echo %extra% & goto end
if %ERRORLEVEL% EQU 1 echo %okCopy% & goto end
if %ERRORLEVEL% EQU 0 goto end
:END

if %hasCopied% EQU true echo. & echo %itemBorder%
goto loop

REM END OF BATCH FILE