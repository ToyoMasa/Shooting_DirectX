#include "common.h"
#include "main.h"
#include "SkinMeshFile.h"

SkinMeshFile::~SkinMeshFile()
{
	if (m_SkinMeshData != NULL)
	{
		m_SkinMeshData->DestroyFrame(m_RootFrame);
		delete (m_SkinMeshData);
	}
	if (m_AnimController != NULL)
	{
		m_AnimController->Release();
	}
	for (int i = 0; i < MAX_ANIMATION; i++)
	{
		if (m_AnimSet[i] != NULL)
		{
			m_AnimSet[i]->Release();
		}
	}
}

bool SkinMeshFile::Load(std::string file_name)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return false;
	}

	m_SkinMeshData = new SkinMeshData();

	if (FAILED(D3DXLoadMeshHierarchyFromXA(
		file_name.c_str(),
		D3DXMESH_MANAGED,
		pDevice,
		m_SkinMeshData,
		NULL,
		&m_RootFrame,
		&m_AnimController) ) )
	{
		return false;
	}

	// �{�[���̍s��̈���m��
	AllocateAllBoneMatrix(m_RootFrame);
	m_AnimController->SetTrackSpeed(0, 0.5f);

	// �ǉ�
	//�A�j���[�V�����g���b�N�̎擾
	for (int i = 0; i < m_AnimController->GetNumAnimationSets(); i++)
	{
		//�A�j���[�V�����擾
		m_AnimController->GetAnimationSet(i, &(m_AnimSet[i]));
	}
	m_CurrentAnim = 0;
	m_AnimController->GetTrackDesc(0, &m_CurrentTrackDesc);

	m_bPlayMontage = false;

	// �S�Ẵt���[���Q�ƕϐ��̐���
	m_FrameArray.clear();
	m_IntoMeshFrameArray.clear();
	CreateFrameArray(m_RootFrame);

	// �t���[���z��ɃI�t�Z�b�g���쐬
	for (DWORD i = 0; i < m_IntoMeshFrameArray.size(); i++)
	{
		MeshContainer* pMyMeshContainer = (MeshContainer*)m_IntoMeshFrameArray[i]->pMeshContainer;

		while (pMyMeshContainer)
		{
			// �X�L�����
			if (pMyMeshContainer->pSkinInfo)
			{
				DWORD cBones = pMyMeshContainer->pSkinInfo->GetNumBones();
				for (DWORD iBone = 0; iBone < cBones; iBone++)
				{
					// �t���[�������瓯�����O�̃t���[��������
					for (DWORD Idx = 0; Idx < m_FrameArray.size(); Idx++)
					{
						if (strcmp(pMyMeshContainer->pSkinInfo->GetBoneName(iBone), m_FrameArray[Idx]->Name) == 0)
						{
							// �I�t�Z�b�g�s��
							m_FrameArray[Idx]->m_OffsetMat = *(pMyMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
							m_FrameArray[Idx]->m_OffsetID = Idx;
							break;
						}
					}
				}
			}
			// ����
			pMyMeshContainer = (MeshContainer*)pMyMeshContainer->pNextMeshContainer;
		}
	}
	return true;
}

HRESULT SkinMeshFile::AllocateBoneMatrix(LPD3DXMESHCONTAINER container)
{
    FrameData *pFrame=NULL;
	DWORD bone_num = 0;

    MeshContainer *original_container = (MeshContainer*)container;
    if (original_container->pSkinInfo == NULL)
	{
		return S_OK;
	}

	bone_num = original_container->pSkinInfo->GetNumBones();
	// �{�[���̃}�g���N�X���̗̈���m�ۂ��ăf�[�^��ۑ�����
    original_container->m_BoneMatrix = new D3DXMATRIX*[bone_num];
	for (DWORD i = 0; i < bone_num; i++)
    {
		pFrame = (FrameData*)D3DXFrameFind( m_RootFrame, container->pSkinInfo->GetBoneName(i) );
        if (pFrame == NULL)
		{
			return E_FAIL;
		}
		original_container->m_BoneMatrix[i] = &pFrame->m_CombinedTransformationMatrix;
	}   

	return S_OK;
}

HRESULT SkinMeshFile::AllocateAllBoneMatrix(LPD3DXFRAME frame)
{
	if (frame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(frame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}

	// �q�������邩�ǂ���
	if (frame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}

	// �Z�킪���邩�ǂ���
	if (frame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrix(frame->pFrameSibling)))
		{
			return E_FAIL;
		}		
	}

	return S_OK;
}


void SkinMeshFile::Draw(LPD3DXMATRIX matrix)
{
	// �t���[���̍s����X�V
	//UpdateFrame(m_RootFrame, matrix);
	// �t���[���`��
	DrawFrame(m_RootFrame);
}

void SkinMeshFile::DrawFrame(LPD3DXFRAME frame)
{
	FrameData *frame_data = (FrameData*)frame;
	LPD3DXMESHCONTAINER container_data = frame_data->pMeshContainer;

	// �R���e�i�̐������`�悷��
	while (container_data != NULL)
	{
		DrawMeshContainer(frame, container_data);

		container_data = container_data->pNextMeshContainer;
	}
	
	// �Z�킪����΍ċA�ŌĂяo��
	if (frame_data->pFrameSibling != NULL)
	{
		DrawFrame(frame_data->pFrameSibling);
	}

	// �q������΍ċA�ŌĂяo��
	if (frame_data->pFrameFirstChild != NULL)
	{
		DrawFrame(frame_data->pFrameFirstChild);
	}
}
	
void SkinMeshFile::DrawMeshContainer(LPD3DXFRAME frame, LPD3DXMESHCONTAINER container)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetDevice();
	if (pDevice == NULL)
	{
		return;
	}

    FrameData *frame_data = (FrameData*)frame;
    MeshContainer *original_container = (MeshContainer*)container;

	if (original_container->pSkinInfo != NULL)
	{
		LPD3DXBONECOMBINATION bone_buffer = (LPD3DXBONECOMBINATION)original_container->m_BoneBuffer->GetBufferPointer();	

		// �{�[���̐��܂킷
		for(DWORD i = 0; i < original_container->m_BoneNum; i++)
		{
			// �u�����h����{�[���̐�
			DWORD bone_blend_num = 0;

			// �{�[��ID����u�����h�����������o��
			for (DWORD j = 0; j < original_container->m_BoneWeightNum; j++)
			{
				if (bone_buffer[i].BoneId[j] != UINT_MAX)
				{
					bone_blend_num++;
				}
			}
			
			// ���_�u�����h�̐ݒ�
			// �������ɂ�_D3DVERTEXBLENDFLAGS���g���Ă���̂�
			// �ΏۂƂȂ��Ă���{�[���̐��ƍ��قɒ���
			pDevice->SetRenderState(D3DRS_VERTEXBLEND, bone_blend_num - 1);

			for(DWORD j = 0; j < original_container->m_BoneWeightNum; j++) 
			{
				DWORD matrix_index = bone_buffer[i].BoneId[j];
				D3DXMATRIX matrix;

				if (matrix_index != UINT_MAX)
				{
					// �I�t�Z�b�g�s��(m_BoneOffsetMatrix) * �{�[���̍s��(m_BoneMatrix)�ōŐV�̈ʒu������o��
					matrix = original_container->m_BoneOffsetMatrix[matrix_index] * (*original_container->m_BoneMatrix[matrix_index]);
					pDevice->SetTransform( D3DTS_WORLDMATRIX(j), &matrix );
				}
			}

			pDevice->SetMaterial( &original_container->pMaterials[bone_buffer[i].AttribId].MatD3D );
			pDevice->SetTexture( 0, original_container->m_TextureList[bone_buffer[i].AttribId] );
			original_container->MeshData.pMesh->DrawSubset(i);	
		}
	// �ʏ��XFile�`��
	} else {
		// �`��ʒu�s��̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &frame_data->m_CombinedTransformationMatrix);

		// ���b�V���̕`��
		for (int i = 0; i < original_container->NumMaterials; i++)
		{
			pDevice->SetMaterial( &original_container->pMaterials[i].MatD3D );
			pDevice->SetTexture( 0, original_container->m_TextureList[i] );
			original_container->MeshData.pMesh->DrawSubset(i);
		}
	}

}

void SkinMeshFile::UpdateFrame(LPD3DXFRAME base, LPD3DXMATRIX parent_matrix)
{
	FrameData *frame = (FrameData*)base;

	// �p���s��̍X�V
    if (parent_matrix != NULL)
	{
        D3DXMatrixMultiply(&frame->m_CombinedTransformationMatrix, 
						&frame->TransformationMatrix,
						parent_matrix);
	}
    else
	{
        frame->m_CombinedTransformationMatrix = frame->TransformationMatrix;
	}

	// �Z�킪����΍ċA�ŌĂяo��
    if (frame->pFrameSibling != NULL)
    {
        UpdateFrame(frame->pFrameSibling, parent_matrix);
    }

	// �q��������΍ċA�ŌĂяo��
    if (frame->pFrameFirstChild != NULL)
    {
        UpdateFrame(frame->pFrameFirstChild,
							&frame->m_CombinedTransformationMatrix);
    }
}

void SkinMeshFile::UpdateAnim(float time)
{
	if (m_AnimController != NULL)
	{
		// ���������ۂ��𔻒�
		m_CurrentWeightTime += time;
		if (m_CurrentWeightTime <= m_ShiftTime)
		{
			// �������B�E�F�C�g���Z�o
			float Weight = m_CurrentWeightTime / m_ShiftTime;
			// �E�F�C�g��o�^
			m_AnimController->SetTrackWeight(0, Weight);       // ���݂̃A�j���[�V����
			m_AnimController->SetTrackWeight(1, 1 - Weight);   // �O�̃A�j���[�V����
		}
		else
		{
			// �����I�����B�ʏ�A�j���[�V����������Track0�̃E�F�C�g���ő�l��
			m_AnimController->SetTrackWeight(0, 1.0f);       // ���݂̃A�j���[�V����
			m_AnimController->SetTrackEnable(1, false);      // �O�̃A�j���[�V�����𖳌��ɂ���
		}

		if (m_bPlayMontage)
		{
			if (m_CurrentWeightTime >= m_MontageTime)
			{
				ChangeAnim(m_NextAnim, 0.3f);
				m_bPlayMontage = false;
			}
		}

		// 0.016�b���A�j���[�V������i�߂�
		m_AnimController->AdvanceTime(time, NULL);
	}
}

// �ǉ�
void SkinMeshFile::ChangeAnim(UINT animID, float shiftTime)
{
	// ���݂���A�j���[�V�����ԍ���
	if (animID < m_AnimController->GetNumAnimationSets())
	{
		// ���̃A�j���[�V�����ƈႤ��
		if (animID != m_CurrentAnim)
		{
			// ���݂̃A�j���[�V�����Z�b�g�̐ݒ�l���擾
			D3DXTRACK_DESC TD;   // �g���b�N�̔\��
			m_AnimController->GetTrackDesc(0, &TD);

			// ���̃A�j���[�V�������g���b�N1�Ɉڍs��
			// �g���b�N�̐ݒ�l���ڍs
			m_AnimController->SetTrackAnimationSet(1, m_AnimSet[m_CurrentAnim]);
			m_AnimController->SetTrackDesc(1, &TD);

			m_CurrentAnim = animID;
			m_AnimController->SetTrackAnimationSet(0, m_AnimSet[m_CurrentAnim]);

			// �g���b�N�̍���������
			m_AnimController->SetTrackEnable(0, true);
			m_AnimController->SetTrackEnable(1, true);

			m_CurrentWeightTime = 0.0f;

			SetShiftTime(shiftTime);

			D3DXTRACK_DESC CurrentTrackDesc;
			m_AnimController->GetTrackDesc(0, &CurrentTrackDesc);
			CurrentTrackDesc.Position = 0;
			m_AnimController->SetTrackDesc(0, &CurrentTrackDesc);
		}
	}
}

bool SkinMeshFile::SetLoopTime(UINT animID, FLOAT time)
{
	// �w��̃A�j���[�V����ID�̑��݂��`�F�b�N
	if (animID < m_AnimController->GetNumAnimationSets())
		return false;

	// �g���b�N�X�s�[�h���ߒl���Z�o
	FLOAT DefTime = m_AnimSet[animID]->GetPeriod();
	m_AnimController->SetTrackSpeed(0, DefTime * time);

	return true;
}

void SkinMeshFile::PlayMontage(UINT animID, float shiftTime, float playTime, UINT nextAnimID)
{
	// ���݂���A�j���[�V�����ԍ���
	if (animID < m_AnimController->GetNumAnimationSets())
	{
		if (nextAnimID < m_AnimController->GetNumAnimationSets())
		{
			ChangeAnim(animID, shiftTime);

			m_bPlayMontage = true;
			m_MontageTime = playTime;
			m_NextAnim = nextAnimID;
		}
	}
}

// �S�Ẵt���[���|�C���^�i�[�����֐�
void SkinMeshFile::CreateFrameArray(LPD3DXFRAME _pFrame)
{
	if (_pFrame == NULL)
	{
		return;
	}

	// �t���[���A�h���X�i�[
	FrameData* pMyFrame = (FrameData*)_pFrame;

	// ���b�V���R���e�i������ꍇ��intoMeshFrameArray�ɃA�h���X���i�[
	if (pMyFrame->pMeshContainer != NULL)
	{
		m_IntoMeshFrameArray.push_back(pMyFrame);
	}

	// �q�t���[������
	if (pMyFrame->pFrameFirstChild != NULL)
	{
		CreateFrameArray(pMyFrame->pFrameFirstChild);
	}

	// �Z��t���[������
	if (pMyFrame->pFrameSibling != NULL)
	{
		CreateFrameArray(pMyFrame->pFrameSibling);
	}
}

FrameData* SkinMeshFile::SearchBoneFrame(LPSTR _BoneName, D3DXFRAME* _pFrame)
{
	FrameData* pFrame = (FrameData*)_pFrame;

	if (strcmp(pFrame->Name, _BoneName) == 0)
	{
		return pFrame;
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameFirstChild);
		if (pFrame != NULL)
		{
			return pFrame;
		}
	}

	if (pFrame->pFrameSibling != NULL)
	{
		pFrame = SearchBoneFrame(_BoneName, _pFrame->pFrameSibling);
		if (pFrame != NULL)
		{
			return pFrame;
		}
	}
}

// �{�[���̃}�g���b�N�X�擾�i�{�[���̖��O�j
D3DXMATRIX SkinMeshFile::GetBoneMatrix(LPSTR _BoneName)
{
	FrameData* pFrame = SearchBoneFrame(_BoneName, m_RootFrame);

	// �{�[�����������
	if (pFrame != NULL)
	{
		return pFrame->m_CombinedTransformationMatrix;
	}
	// �{�[����������Ȃ����
	else
	{
		D3DXMATRIX TmpMatrix;
		D3DXMatrixIdentity(&TmpMatrix);
		return TmpMatrix;
	}
}