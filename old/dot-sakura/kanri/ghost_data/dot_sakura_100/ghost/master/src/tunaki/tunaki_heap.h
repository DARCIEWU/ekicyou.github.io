/* ----------------------------------------------------------------------------
 * tunaki_heap.h
 *   �Ȃ��E���C���w�b�_�[.
 * ----------------------------------------------------------------------------
 * Mastering programed by Dot-Station Mastor
 *
 * Copyright 2004 Dot-Station.
 * ----------------------------------------------------------------------------
 * $Id: tunaki_heap.h,v 1.1 2004/03/30 23:10:55 cvs Exp $
 * ----------------------------------------------------------------------------
 */
#ifndef TUNAKI_HEAP_H__
#define TUNAKI_HEAP_H__

#ifndef _WIN32
#error written for win32 only
#endif

#include "tunaki_def.h"


/* ----------------------------------------------------------------------------
 * �֐��錾
 */

/* �R���X�g���N�^ */
extern BOOL tunaki_heap_construct(
      PTUNAKI t // TUNAKI �\����
  );

/* �f�X�g���N�^ */
extern void tunaki_heap_destruct(
      PTUNAKI t // TUNAKI �\����
  );

/* �q�[�v���� -alloc */
extern LPVOID tunaki_heapAlloc(
      PTUNAKI t,  // TUNAKI �\����
      SIZE_T size // �m�ۂ���T�C�Y
  );

/* �q�[�v���� -free */
extern void tunaki_heapFree(
      PTUNAKI t,  // TUNAKI �\����
      LPVOID ptr  // �Ώۂ̃��[�J���q�[�v
  );

/* �q�[�v���� -reAlloc */
extern LPVOID tunaki_heapReAlloc(
      PTUNAKI t,  // TUNAKI �\����
      LPVOID ptr, // �Ώۂ̃��[�J���q�[�v
      SIZE_T size // �m�ۂ���T�C�Y
  );

/* �q�[�v���� -size */
extern SIZE_T tunaki_heapSize(
      PTUNAKI t,  // TUNAKI �\����
      LPVOID ptr  // �Ώۂ̃��[�J���q�[�v
  );


/* ----------------------------------------------------------------------------
 * �}�N���֐�
 */
#define HALLOC(s)     (tunaki_heapAlloc(t,s))
#define HFREE(p)      (tunaki_heapFree(t,p))
#define HREALLOC(p,s) (tunaki_heapReAlloc(t,p,s))
#define HSIZE(p)      (tunaki_heapSize(t,p))


/* ----------------------------------------------------------------------------
 * EOF
 */
#endif
