@echo off
echo ..�p�����[�^�Z�b�g
set version=040509a


set ghost_id=nise_hokan_sakura

set ghost_select=nise_hokan_sakura
set shell_select=nise_hokan_sakura
set balloon_select=nise_hokan_sakura
set balloon_name=bottle
set crow_select=crow

set work_base_dir=c:\wrkdir\download
set work_tmp_dir=%work_base_dir%\tmp
set ssp_dir=c:\wintools\����\ssp

pushd ..\..\..
  call packUtil\%1.bat
  popd
