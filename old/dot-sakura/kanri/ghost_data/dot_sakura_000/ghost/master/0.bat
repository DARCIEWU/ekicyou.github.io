rem ## �f�o�b�O�S�[�X�g ##
set nar_name=dot_sakura
set ssp_base_dir=c:\wintools\����\ssp
set xcopy_opt=
set mvdir=%ssp_base_dir%\ghost\dot_sakura_debug
cmd /C up1.bat

del %mvdir%\ghost\master\descript.txt
del %mvdir%\ghost\master\kawari.ini

ren %mvdir%\ghost\master\descriptDebug.txt              descript.txt
ren %mvdir%\ghost\master\kawariDebug.ini                kawari.ini

