rem ## nar�t�@�C���쐬 ##
rem nar�t�@�C����
set nar_name=dot_sakura
set nar_version=040321a
set ssp_base_dir=c:\wintools\����\ssp
set tmpDir=c:\wrkdir\download
set netDir=%tmpDir%\ghost\%nar_name%_000
set xcopy_opt=

rem #�z�z��t�@�C��
set narDir=%tmpDir%\nar


rem # �쐬��Dir�쐬
rmdir /s /q  c:\wrkdir\download\nar
mkdir c:\wrkdir\download
mkdir c:\wrkdir\download\nar



rem ###################################################################
rem # �z�z�p�S�[�X�g�f�[�^�쐬
set mvdir=%narDir%\%nar_name%
cmd /C up1.bat
del %mvdir%\ghost\master\descriptDebug.txt
del %mvdir%\ghost\master\kawariDebug.ini
copy %netDir%\updates2.dau %mvdir%\ghost\master\
copy %netDir%\updates2.dau %mvdir%\




rem ###################################################################
rem # �o���[���z�z�f�[�^�쐬
pushd %mvdir%\bottle
  c:\bin\zip -9 -q -r ..\..\balloon_bottle.nar *.*
  popd




rem ###################################################################
rem # crow�p�t�@�C���쐬
set crowDir=c:\wrkdir\download\nar\crow
set crowGhostDir=%crowDir%\ghost
set crowBalloonDir=%crowDir%\balloon

mkdir %crowDir%
mkdir %crowGhostDir%
mkdir %crowGhostDir%\%nar_name%
mkdir %crowBalloonDir%
mkdir %crowBalloonDir%\bottle

copy ..\..\..\crow\*.* %crowDir%\
xcopy /Q /S %mvdir%\*.*        %crowGhostDir%\%nar_name%\
xcopy /Q /S %mvdir%\bottle\*.* %crowBalloonDir%\bottle\
rmdir /s /q %crowGhostDir%\%nar_name%\bottle

rem # crow�z�z�t�@�C���쐬
pushd %narDir%
  c:\bin\zip -9 -q -r crow_%nar_name%.zip crow\*.*
  popd
rmdir /s /q %crowDir%




rem ###################################################################
rem # .nar�z�z�t�@�C���쐬
pushd %mvdir%
  c:\bin\zip -9 -q -r ..\%nar_name%_%nar_version%.nar *.*
  c:\bin\zip -9 -q -r ..\%nar_name%.nar *.*
  cd ..\
  popd
rmdir /s /q %mvdir%



