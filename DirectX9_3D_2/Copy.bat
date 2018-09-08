

xcopy /y	.\Engine\Headers\*.h					.\Reference\Headers\
xcopy /y	.\Engine\Export\*.*						.\Reference\Headers\

xcopy /y	.\Engine\System\Codes\*.h				.\Reference\Headers\
xcopy /y	.\Engine\System\bin\System.lib			.\Reference\Librarys\
xcopy /y	.\Engine\System\bin\System.dll			.\Client\Bin\

xcopy /y	.\Engine\Utility\Codes\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Utility\Codes\*.hpp			.\Reference\Headers\
xcopy /y	.\Engine\Utility\bin\Utility.lib		.\Reference\Librarys\
xcopy /y	.\Engine\Utility\bin\Utility.dll		.\Client\Bin\

xcopy /y	.\Engine\Resource\Codes\*.h				.\Reference\Headers\
xcopy /y	.\Engine\Resource\bin\Resource.lib		.\Reference\Librarys\
xcopy /y	.\Engine\Resource\bin\Resource.dll		.\Client\Bin\