@echo off
echo ..[nar]�t�@�C���쐬�����i�o���[���j

pushd %work_tmp_dir%\%balloon_name%
  zip -9 -q -r %narDir%\balloon_%balloon_name%.nar *.*
  popd
