@echo off
:: Запрашиваем имя папки у пользователя
set /p folderName=Enter the folder name to search for:

:: Формируем полный путь к искомой папке
set "basePath=C:\Users\Николай\source\repos\student\pioivis\lab5"
set "targetPath=%basePath%\%folderName%"

:: Проверяем, существует ли папка
if not exist "%targetPath%" (
    echo The folder "%folderName%" does not exist in the path.
    pause
    exit /b
)

:: Переходим в указанную папку
cd /d "%targetPath%"

:: Удаляем старый result.txt, если он существует
if exist "%basePath%\result.txt" del "%basePath%\result.txt"

:: Ищем слова, начинающиеся с v или V, и записываем их в result.txt
for /r %%f in (*.txt) do (
    findstr /r "\<v.* \V.*" "%%f" >> "%basePath%\result
