/* ----------------------------------------------------------------------------
 * tunaki.c
 *   �Ȃ��E�x�[�X�֐�.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_heap.c,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#include "tunaki_heap.h"


/* ----------------------------------------------------------------------------
 * �R���X�g���N�^
 */
BOOL tunaki_heap_construct(
      PTUNAKI t // TUNAKI �\����
  )
{
  if(! t) return FALSE;

  // Heap�m��
  if(! (t->hHeap=HeapCreate(0, 64 *1024, 0))) return FALSE;
  
  return TRUE;
}


/* ----------------------------------------------------------------------------
 * �f�X�g���N�^
 */
void tunaki_heap_destruct(
      PTUNAKI t // TUNAKI �\����
  )
{
  if(! t) return;

  // Heap���
  if(t->hHeap){
    HeapDestroy(t->hHeap);
    t->hHeap =NULL;
  }
}


/* ----------------------------------------------------------------------------
 * �q�[�v���� -alloc
 */
LPVOID tunaki_heapAlloc(
      PTUNAKI t,  // TUNAKI �\����
      SIZE_T size // �m�ۂ���T�C�Y
  )
{
  LPVOID ptr;

  if(! t)         return NULL;
  if(! t->hHeap)  return NULL;

  // �������m��
  ptr = HeapAlloc(t->hHeap, 0, size);
  
  // @maz �G���[�̏ꍇ�̃��O�o��
  
  return ptr;
}


/* ----------------------------------------------------------------------------
 * �q�[�v���� -free
 */
void tunaki_heapFree(
      PTUNAKI t,  // TUNAKI �\����
      LPVOID ptr  // �Ώۂ̃��[�J���q�[�v
  )
{
  if(! t)         return;
  if(! t->hHeap)  return;

  // ���������
  HeapFree(t->hHeap, 0, ptr);

  return;
}


/* ----------------------------------------------------------------------------
 * �q�[�v���� -reAlloc
 */
LPVOID tunaki_heapReAlloc(
      PTUNAKI t,  // TUNAKI �\����
      LPVOID ptr, // �Ώۂ̃��[�J���q�[�v
      SIZE_T size // �m�ۂ���T�C�Y
  )
{
  LPVOID newPtr;
  
  if(! t)         return;
  if(! t->hHeap)  return;

  // �������Ċ��蓖��
  newPtr =HeapReAlloc(t->hHeap, 0, ptr, size);

  // @maz �G���[�̏ꍇ�̃��O�o��

  return newPtr;
}


/* ----------------------------------------------------------------------------
 * �q�[�v���� -size
 */
SIZE_T tunaki_heapSize(
      PTUNAKI t,  // TUNAKI �\����
      LPVOID ptr  // �Ώۂ̃��[�J���q�[�v
  )
{
  if(! t)         return 0;
  if(! t->hHeap)  return 0;

  // �T�C�Y�擾
  return HeapSize(t->hHeap, 0, ptr);
}


/* ----------------------------------------------------------------------------
 * EOF
 */
