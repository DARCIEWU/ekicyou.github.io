<?php
//####################################################################
//
//    [�ե�����̾]
//    edit.php
//
//    [����]
//    ������Խ���ID��¸�ߤ�����Ͻ����⡼��
//    �ޤ���UPDATE������¸�ߤ�����ϡ�
//    �����ͤ�ɽ��������ܥ����ɽ��
//
//    [����]
//    ID:�Խ�����ǡ���
//    UPDATE[]:�������줿����
//
//    [�õ�����]
//    �äˤʤ�
//
//####################################################################
  require_once( "./login.pplb" );
  $err =new ErrorSSTP("������ץ�ɽ��");

  //==================================================================
  // ���������å�
  //==================================================================
  $ID     =$_REQUEST[ID];
  $UPDATE =$_REQUEST[UPDATE];
  $el     =$_REQUEST[el];
  $COMMIT =FALSE;
  $MODE   ="NEW";

  //==================================================================
  // ��������
  //==================================================================
  if(is_array($UPDATE)){
    // ��ǧ�⡼��
    $MODE="UPD";
    $el =array_merge($el, $UPDATE);
    $ID =$el[ID];
    $buf[EDIT]   =EditItem::getEditItem($err, $el, $FT);
  }
  else if($ID>0){
    // �����⡼��
    // �Խ�������������
    $MODE ="NEW";
    $buf[EDIT]   =EditItem::getEditItem($err, $el, $FT);
    $buf[HIDDEN] =setHiddenHTML2("MODE", "COMMIT");
  }
  else{
    // �����⡼��
    $MODE ="UPD";
    $buf[EDIT]   =EditItem::getEditItem($err, $el, $FT);
    $buf[HIDDEN] =setHiddenHTML2("MODE", "COMMIT");
  }

  //==================================================================
  // �����饹����ץ�����
  //==================================================================
  if(strlen($el[SCRIPT])>0){
    $sstp =new SSTP($_SERVER[REMOTE_ADDR],"DOT-STATION");
    $script =Dot2Sakura($el[SCRIPT], $el[GHOST]);
//   $sstp->gSend($script, $el[GHOST]);
  }


  //==================================================================
  // ɽ��
  //==================================================================
  switch($MODE){
    case 'NEW'    :
    case 'UPD'    :
      $buf[INPUT] =TRUE;
      break;
    case 'CONFIRM':
    case 'COMMIT' :
  }


  $TMPL_FILE ="template.tmpl";
  $buf[NAME] =$err->title;
  TmplUty::out( $TMPL_FILE, $buf );

//====================================================================
// DOT������ץ�
//====================================================================
function Dot2Sakura($text, $ghost){
  // �Ԥ��Ȥ������ʬ��
  $text =str_replace("\r\n", "\n", $text);
  $text =str_replace("\r"  , "\n", $text);
  $lines =explode("\n", $text);

  // ����٥룱���֥�å��̤�ʬ��
  $blocks =array();
  $b ="__start__";
  $cnt =0;

  foreach($lines as $line){
    switch(mb_substr($line, 0, 1)){
      // �����ȹԤ�̵��
      case '��':
        continue;

      // �����������֥�å�
      case '��':
        $b =$line;
        $cnt++;
        continue;

      // �֥�å�����Ͽ
      default:
        unset($x);
        $x[TEXT] =$line;
        $blocks[$b][$cnt][] =$x;
    }
  }


  // ����٥룲����Ƭ�����ʺǽ�ˡ��򸫤Ĥ���ޤǡ�
  foreach($blocks as $bkey=>$blockBase){
    foreach($blockBase as $bCnt=>$block){
      foreach($block as $lkey=>$line){
        if(preg_match("/^((��|��|��|��|��|��)*)��(.*)$/", $line[TEXT], $match)){
          $line[MODE]  =$match[1];
          $line[TEXT] =$match[3];
        }
        else{
          $line[MODE]  ="��"; // �������Ȥ��ڤ��ؤ����˼��ι�
          $line[TEXT] =$line[TEXT];
        }
        $blocks[$bkey][$bCnt][$lkey] =$line;
      }
    }
  }


  // ����٥룳����̤򸫤Ĥ��ƥȡ������ʬ����
  foreach($blocks as $bkey=>$blockBase){
    foreach($blockBase as $bCnt=>$block){
      foreach($block as $lkey=>$line){
        // �ʡ����ˤ򸡽�
        $s =$line[TEXT];
        $t1 =array();
        unset($tt);
        $len =mb_strlen($s);
        for($ii=0; $ii<$len; $ii++){
          $s1 =mb_substr($s, $ii, 1);
          if($s1 == '��'){
            if(mb_substr($s, $ii+1, 1)=='��'){
              if(isset($tt))  $t1[] =$tt;
              unset($tt);
              $ii++;
              $t1[] ="�ʡ�";
              continue;
            }
          }
          if($s1 == "��"){
            if(isset($tt))  $t1[] =$tt;
            unset($tt);
            $t1[] ="��";
            continue;
          }
          $tt .=$s1;
        }
        if(isset($tt))  $t1[] =$tt;

        // �ʡ����ˤΥ֥�å���ȴ���Ф�
        $t2 =array();
        $chk=FALSE;
        unset($x);
        foreach($t1 as $k=>$v){
          if($v=="�ʡ�"){
            if(isset($x)) $t2[] =$x;
            unset($x);
            $x[TYPE]=CALL_MODE;
            $x[TEXT]="��";
            $chk=TRUE;
            continue;
          }
          if($chk && ($v=="��")){
            $chk=TRUE;
            if(isset($x)) $t2[] =$x;
            unset($x);
            continue;
          }
          if(! isset($x)) $x[TYPE]=TALK_MODE;
          $x[TEXT] .=$v;
        }
        if(isset($x)) $t2[] =$x;
        $line[TOKEN] =$t2;
        $blocks[$bkey][$bCnt][$lkey] =$line;
      }
    }
  }








d_print_r($blocks);



  return "\t���ϴ�λ\e";



  return $script;
}




?>