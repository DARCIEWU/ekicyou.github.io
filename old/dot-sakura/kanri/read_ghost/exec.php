<?php
//####################################################################
//
//    [�ե�����̾]
//    log_exec.php
//
//    [����]
//    �ܥȥ��������
//
//    [�õ�����]
//    �ʤ�
//
//####################################################################
//====================================================================
// �إå��ȣԣͣ�
//====================================================================
  require_once( "./login.pplb" );
  require_once( "./exec_main.pplb" );
  $err->init("�ǡ��������� | �֡�������ץǡ��������� | �¹�");

  $buf[TITLE ] = $err->title;
  $buf[HEADER] =TRUE;
  $buf[BODY  ] =TRUE;
  $buf[MESSAGE] ="�����߽�����Ǥ���\n���Ф餯���Ԥ�����������";
  TmplUty::out("../base.tmpl" ,$buf);

//====================================================================
// �ᥤ��롼����
//====================================================================

  while (@ob_end_clean());
  $conn =logonDB();
  $jobList[] =array(JOB_ID=>1, NAME=>"��������");

  DButil::begin($conn);
  foreach($jobList as $job){
    set_time_limit(120);
    $JOB_ID =$job[JOB_ID];
    echo "��{$job[NAME]}....<br>\n";
    flush();
    $eval_str="job{$JOB_ID}" .'($conn, $err);';
    eval($eval_str);
    if($err->isError()) break;
  }
  if($err->isError()) DButil::rollback($conn);
  else                DButil::commit($conn);
  logoffDB($conn);
  $err->isErrorExit();

//####################################################################
// �����������Ρ������������ޤǡ���
//####################################################################
  $err->isErrorExit();
  unset($buf);
  $buf[BODY  ] =TRUE;
  $buf[FOOTER ] =TRUE;
  $buf[MESSAGE] ="�������߽�λ���ޤ�������";
  TmplUty::out("../base.tmpl" ,$buf);
?>