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
?>
<html>
<head>
<title>DOT-STATION��������˥塼</title>
<meta http-equiv="Content-Type" content="text/html;charset=UTP-8">
<link rel="stylesheet" href="/common/style.css" type="text/css">
<link rel="stylesheet" href="/common/kanri.css" type="text/css">
<link rel="stylesheet" href="/common/item.css" type="text/css" />
</head>

<body bgcolor="#FFFFFF">
<table width="700" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center" height="49">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td>
            <table border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="600" class="KANRI_TITLE_A">����������</td>
                <td width="100" align="right"><a href="/kanri/index.php">�ԣϣФ����</a></td>
              </tr>
            </table>
          </td>
        </tr>
        <tr>
          <td class="KANRI_TITLE_B">DOT-STATION��������˥塼</td>
        </tr>
        <tr>
          <td class="KANRI_TITLE_C">���ܥȥ�� | �����߽��� | �¹�</td>
        </tr>
      </table>
    </td>
  </tr>
  <tr>
    <td align="center">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="left"> <br>
            �����߽�����Ǥ���<br>���Ф餯���Ԥ�����������</td>
        </tr>
      </table>
    </td>
  </tr>
</table>
<?php
//====================================================================
// �ᥤ��롼����
//====================================================================
  require_once("local/dot/db.pplb");
  require_once("HTML/ErrorSSTP.pplb");
  require_once("HTML/string.pplb");
  require_once("HTML/uty.pplb");
  require_once("log_exec_main.pplb");
  $err =new ErrorSSTP("�ܥȥ�� | �����߽��� | �¹�");

  while (@ob_end_clean());
  $conn =logonDB();
  $jobList[] =array(JOB_ID=>2, NAME=>"�����");
  $jobList[] =array(JOB_ID=>3, NAME=>"�������ȥ����ͥ륨�ꥢ���ǡ����ɹ�");
  $jobList[] =array(JOB_ID=>1, NAME=>"��������");

  DButil::begin($conn);

  foreach($jobList as $job){
    set_time_limit(120);
    $JOB_ID =$job[JOB_ID];
    echo "��{$job[NAME]}....<br>\n";
    flush();
    $eval_str="job{$JOB_ID}" .'($conn, $err);';
    echo "COMMAND: $eval_str<br>\n";
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
?>
<table width="700" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center">
      <table width="700" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td align="left"> <br>
            �������߽�λ���ޤ�����</td>
        </tr>
      </table>
    </td>
  </tr>
</table>


</body>

</html>