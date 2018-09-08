
//명령어		옵션		복사할 대상의 폴더나 파일		복사할 곳의 경로
xcopy /y	.\Engine\Headers\*.h					.\Reference\Headers\
xcopy /y	.\Engine\Export\*.*						.\Reference\Headers\

xcopy /y	.\Engine\System\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\System\bin\System.lib			.\Reference\Librarys\
xcopy /y	.\Engine\System\bin\System.dll			.\Client\bin\

xcopy /y	.\Engine\Utility\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Utility\bin\Utility.lib		.\Reference\Librarys\
xcopy /y	.\Engine\Utility\bin\Utility.dll		.\Client\bin\

xcopy /y	.\Engine\Resources\Code\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Resources\bin\Resources.lib	.\Reference\Librarys\
xcopy /y	.\Engine\Resources\bin\Resources.dll	.\Client\bin\
