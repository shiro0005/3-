//=============================================================================
//
// �e���� [shadow.h]
//
//=============================================================================
#pragma once

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Shadow_Initialize(void);
void Shadow_Finalize(void);
void Shadow_Update(void);
void Shadow_Draw(void);

int Shadow_Create(D3DXVECTOR3 pos, D3DXVECTOR3 scl);
void Shadow_Release(int nIdxShadow);
void Shadow_SetPosition(int nIdxShadow, D3DXVECTOR3 pos);
void Shadow_SetScale(int nIdxShadow, D3DXVECTOR3 scl);
