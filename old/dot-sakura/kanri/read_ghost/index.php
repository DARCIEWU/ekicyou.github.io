<?php
//####################################################################
//
//    [�ե�����̾]
//    log.php
//
//    [����]
//    �ܥȥ����������
//
//    [�õ�����]
//    �äˤʤ�
//
//####################################################################
  require_once( "./login.pplb" );
  $err->init("�ǡ��������� | �֡�������ץǡ���������");

//====================================================================
// ɽ���������
//====================================================================
  $bb[URL] =$bb[URL2];
  $buf[HTML] = TmplUty::getText("index.tmpl", $bb);

//====================================================================
// ɽ��
//====================================================================
  $buf[MESSAGE] ="�֡�������̤���ʤ��ˡפΥǡ�����ģ¤���Ͽ���ޤ���";
  outputTmpl(&$err, $buf);
?>