@echo off
echo ..[nar]�t�@�C���쐬�����i�S�[�X�g�j

pushd %work_tmp_dir%
  zip -9 -r %narDir%\%ghost_id%_%version%.nar *.* 1>nul 2>&1
  zip -9 -r %narDir%\%ghost_id%.nar *.* 1>nul 2>&1
  popd
