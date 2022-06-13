#pragma once
#include"stdafx.h"
#include"DrawData/IDrawObject.h"
namespace ButiEngine {
namespace ButiRendering {
class IDrawLayer;
/// <summary>
/// �V�[������������`��p�N���X
/// </summary>
class IRenderer :public IObject {
public:
	void PreInitialize()override {}
	virtual void Update() = 0;

	/// <summary>
	/// �S�̂̃����_�����O�J�n
	/// </summary>
	virtual void RenderingStart() = 0;
	/// <summary>
	/// �����_�����O�J�n�O�̏���
	/// </summary>
	virtual void BefRendering() = 0;
	/// <summary>
	/// �`�惌�C���̒ǉ�
	/// </summary>
	virtual void AddLayer() = 0;
	/// <summary>
	/// ���C���[�̐��̎擾
	/// </summary>
	/// <returns>���C���[��</returns>
	virtual std::uint32_t GetLayerCount() = 0;
	virtual Value_ptr<IDrawLayer> GetDrawLayer(const std::int32_t arg_index)const=0;
	virtual void RemoveDrawLayer(const std::int32_t arg_index)=0;
	/// <summary>
	/// �`�惌�C���[�̃����_�����O
	/// </summary>
	/// <param name="arg_layerIndex">�`�悷�郌�C���[�̔ԍ�</param>
	virtual void Rendering(const std::uint32_t arg_layerIndex) = 0;
	/// <summary>
	/// �`�惌�C���[�̉e�̃����_�����O
	/// </summary>
	/// <param name="arg_layerIndex">�`�悷�郌�C���[�̔ԍ�</param>
	virtual void ShadowRendering(const std::uint32_t arg_layerIndex) = 0;
	/// <summary>
	/// �S�̂̃����_�����O�I��
	/// </summary>
	virtual void RenderingEnd() = 0;
	/// <summary>
	/// �`��I�u�W�F�N�g�̉��
	/// </summary>
	virtual void ClearDrawObjects() = 0;
	/// <summary>
	/// �`�惌�C���[�ɑ΂���V���h�E�p�e�N�X�`���̐ݒ�
	/// </summary>
	/// <param name="arg_layerIndex">�ݒ肷�郌�C���[�̔ԍ�</param>
	/// <param name="arg_shadowTex">�ݒ肷��V���h�E�p�e�N�X�`��</param>
	virtual void SetShadowTexture(const std::uint32_t arg_layerIndex, Value_weak_ptr<IResource_Texture> arg_shadowTex) = 0;
	/// <summary>
	/// �`�惌�C���[���g�p����V���h�E�p�e�N�X�`���̎擾
	/// </summary>
	/// <param name="arg_layerIndex">���C���[�̔ԍ�</param>
	/// <returns>�g�p���Ă���V���h�E�p�e�N�X�`��</returns>
	virtual Value_ptr<IResource_Texture> GetShadowTexture(const std::uint32_t arg_layerIndex) = 0;
	/// <summary>
	/// �`��I�u�W�F�N�g�̓o�^
	/// </summary>
	/// <param name="arg_vlp_drawObject">�`��I�u�W�F�N�g</param>
	/// <param name="arg_afterDraw">�x�点�ĕ`�悷�邩(�������I�u�W�F�N�g��)</param>
	/// <param name="arg_layerIndex">�o�^����`�惌�C���[�̔ԍ�</param>
	/// <param name="isShadow">�e�𗎂Ƃ���</param>
	virtual void RegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
	/// <summary>
	/// �`��I�u�W�F�N�g�̓o�^����
	/// </summary>
	/// <param name="arg_vlp_drawObject">�`��I�u�W�F�N�g</param>
	/// <param name="arg_afterDraw">�x�点�ĕ`�悷�邩(�������I�u�W�F�N�g��)</param>
	/// <param name="arg_layerIndex">�o�^����`�惌�C���[�̔ԍ�</param>
	/// <param name="isShadow">�e�𗎂Ƃ���</param>
	virtual void UnRegistDrawObject(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_afterDraw, const std::uint32_t arg_layerIndex = 0, const bool isShadow = false) = 0;
	/// <summary>
	/// �������
	/// </summary>
	virtual void Release() = 0;
	/// <summary>
	/// Renderer����̒萔�o�b�t�@�X�V
	/// </summary>
	/// <param name="arg_param">�X�V����l</param>
	virtual void UpdateRendererCBuffer(const RenderingSceneInfo& arg_param) = 0;
	/// <summary>
	/// Renderer����̒萔�o�b�t�@���
	/// </summary>
	virtual void ReleaseRendererCBuffer() = 0;
	/// <summary>
	/// Renderer����̒萔�o�b�t�@�擾
	/// </summary>
	/// <returns>Renderer����̒萔�o�b�t�@</returns>
	virtual Value_ptr<CBuffer<RenderingSceneInfo>> GetRendererCBuffer() = 0;
	/// <summary>
	/// �V���h�E�p�J�����̐ݒ�
	/// </summary>
	/// <param name="arg_layer">�ݒ肷�郌�C���[�̔ԍ�</param>
	/// <param name="arg_vlp_camera">�ݒ肷��J����</param>
	virtual void SetShadowCamera(const std::uint32_t arg_layer, Value_ptr<ICamera> arg_vlp_camera) = 0;
	/// <summary>
	/// �V���h�E�p�J�����̎擾
	/// </summary>
	/// <param name="arg_layer">�擾���郌�C���[�̔ԍ�</param>
	/// <returns>�V���h�E�p�J����</returns>
	virtual Value_ptr<ICamera> GetShadowCamera(const std::uint32_t arg_layer) = 0;
	virtual Value_ptr<GraphicDevice> GetGraphicDevice() const= 0;
};

/// <summary>
/// �`��I�u�W�F�N�g���C���[�̃C���^�[�t�F�[�X
/// </summary>
class IDrawLayer :public IObject {
public:

	void Initialize()override {};
	void PreInitialize()override {}

	/// <summary>
	/// ���g�̍폜
	/// </summary>
	virtual void Clear() = 0;
	/// <summary>
	/// �����_�����O�����̑O�ɍs�������𑖂点��
	/// </summary>
	virtual void BefRendering() = 0;
	/// <summary>
	/// �`��I�u�W�F�N�g��o�^
	/// </summary>
	/// <param name="arg_vwp_drawObject">�`��I�u�W�F�N�g</param>
	/// <param name="arg_isAfterRendering">�x�点�ĕ`�悷�邩(�������I�u�W�F�N�g��)</param>
	/// <param name="arg_ret_pim">�`��I�u�W�F�N�g�̌`��</param>
	/// <param name="arg_isShadow">�e�𐶐�����I�u�W�F�N�g��</param>
	virtual void Regist(Value_ptr< IDrawObject> arg_vwp_drawObject, const bool arg_isAfterRendering, const bool arg_isShadow = false) = 0;
	/// <summary>
	/// �`��I�u�W�F�N�g�̓o�^����
	/// </summary>
	/// <param name="arg_vlp_drawObject">�`��I�u�W�F�N�g</param>
	/// <param name="arg_isAfterRendering">�`���x�点�邩</param>
	/// <param name="arg_isShadow">�e�𐶐�����I�u�W�F�N�g��</param>
	virtual void UnRegist(Value_ptr< IDrawObject> arg_vlp_drawObject, const bool arg_isAfterRendering, const bool arg_isShadow = false) = 0;
	/// <summary>
	/// �V���h�E�B�e�p�J�����̓o�^
	/// </summary>
	/// <param name="arg_shadowCamera"></param>
	virtual void SetShadowCamera(Value_ptr<ICamera> arg_shadowCamera) = 0;
	/// <summary>
	/// �V���h�E�ǂݍ��݃e�N�X�`���̓o�^
	/// </summary>
	/// <param name="arg_texture"></param>
	virtual void SetShadowTexture(Value_weak_ptr<IResource_Texture> arg_texture) = 0;
	/// <summary>
	/// �����_�����O�̎��s
	/// </summary>
	virtual void Rendering() = 0;
	/// <summary>
	/// �`��R�}���h�̍\�z
	/// </summary>
	virtual void CommandSet() = 0;
	/// <summary>
	/// �e�̃����_�����O
	/// </summary>
	virtual void ShadowRendering() = 0;
	/// <summary>
	/// �V���h�E�p�J�����̎擾
	/// </summary>
	/// <returns></returns>
	virtual Value_ptr<ICamera> GetShadowCamera() = 0;
	/// <summary>
	/// �V���h�E�p�e�N�X�`���̎擾
	/// </summary>
	/// <returns></returns>
	virtual Value_ptr<IResource_Texture> GetShadowTexture() = 0;
};
/// <summary>
/// �e�p�̕`�惌�C���[
/// </summary>
class IDrawLayer_Shadow {
public:
	/// <summary>
	/// �e���������񂾂��̎擾
	/// </summary>
	/// <returns></returns>
	virtual bool IsShadowDrawed() = 0;
};

BUTIRENDERING_API Value_ptr<IRenderer> CreateRenderer(Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
}
}