
rd /s /q .\Client\Default\Debug
rd /s /q .\Client\Default\Release

rd /s /q .\Engine\System\Default\Debug
rd /s /q .\Engine\System\Default\Release

rd /s /q .\Engine\Utility\Default\Debug
rd /s /q .\Engine\Utility\Default\Release

rd /s /q .\Engine\Resource\Default\Debug
rd /s /q .\Engine\Resource\Default\Release

rd /s /q .\Tool\Debug
rd /s /q .\Tool\Release

rd /s /q .\Tool\MapTool\Debug
rd /s /q .\Tool\MapTool\Release

rd /s /q .\Tool\AnimationTool\Debug
rd /s /q .\Tool\AnimationTool\Release

rd /s /q .\Tool\UITool\Debug
rd /s /q .\Tool\UITool\Release

rd /s /q .\Tool\EffectTool\Debug
rd /s /q .\Tool\EffectTool\Release

rd /s /q .\Tool\CameraTool\Debug
rd /s /q .\Tool\CameraTool\Release

rd /s /q .\Debug
rd /s /q .\Release

del .\*.ncb

del .\Client\Default\*.user

del .\Engine\System\Default\*.user
del .\Engine\Utility\Default\*.user
del .\Engine\Resource\Default\*.user

del .\Tool\MapTool\*.user
del .\Tool\AnimationTool\*.user
del .\Tool\UITool\*.user
del .\Tool\EffectTool\*.user
del .\Tool\CameraTool\*.user

