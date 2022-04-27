#ifndef ICAMERA_H
#define ICAMERA_H
#include"ButiMath/ButiMath.h"
#include"ButiMath/Transform.h"
#include <vector>
#include"ButiUtil/ButiUtil/ID.h"
#include"ResourceInterface.h"
#include"RenderingStatus.h"
namespace ButiEngine {
namespace Geometry {
class Box_AABB;
}
namespace ButiRendering {


struct CameraProjProperty {
	CameraProjProperty() {};
	CameraProjProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell = false, std::uint32_t arg_layer = 0);

	std::int32_t currentWidth = 0, currentHeight = 0, width, height, left = 0, top = 0;
	float front = 0.0f, angle = 60.0f, farClip = 50.0f, nearClip = 0.1f;
	bool isPararell = false;
	std::uint32_t layer = 0;
	std::int32_t renderingInfo = 0;
	std::vector< TextureTag >projectionTexture;
	TextureTag depthStencilTexture;
	std::string cameraName;
	Vector4 clearColor;
	bool isInitActive = true, isEditActive = false, isShadow = false;
	std::vector< TextureTag >vec_shadowTextures;
	std::vector< TextureTag >vec_staticShadowTextures;


	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(width);
		archive(height);
		archive(left);
		archive(top);
		archive(front);
		archive(angle);
		archive(farClip);
		archive(nearClip);
		archive(isPararell);
		archive(layer);
		archive(front);
		archive(renderingInfo);
		archive(projectionTexture);
		archive(depthStencilTexture);
		archive(cameraName);
		archive(clearColor);
		archive(isInitActive);
		archive(isEditActive);
		archive(isShadow);
		archive(vec_shadowTextures);
		archive(vec_staticShadowTextures);
	}

};


/// <summary>
/// �J�����̃C���^�[�t�F�[�X
/// </summary>
class ICamera :public IObject {
public:

	Value_ptr<Transform> vlp_transform = ObjectFactory::Create<Transform>(Vector3(0, 0, 0));

	/// <summary>
	/// �`��J�n
	/// </summary>
	virtual void Start() = 0;
	/// <summary>
	/// �`���~
	/// </summary>
	virtual void Stop()const = 0;

	virtual void Initialize()override {}
	virtual void PreInitialize()override {}
	/// <summary>
	/// ���O�̎擾
	/// </summary>
	/// <returns>���O</returns>
	virtual const std::string& GetName() const = 0;
	/// <summary>
	/// ���O�̐ݒ�
	/// </summary>
	/// <param name="arg_name">���O</param>
	virtual void SetName(const std::string& arg_name) = 0;
	/// <summary>
	/// ���s���邩�̐ݒ�
	/// </summary>
	/// <param name="arg_active"></param>
	virtual void SetActive(const bool arg_active) = 0;
	/// <summary>
	/// ���s���邩�̎擾
	/// </summary>
	/// <returns></returns>
	virtual bool GetActive()const = 0;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// GUI�Ăяo��
	/// </summary>
	virtual void ShowUI() = 0;
	/// <summary>
	/// �J�����ݒ�̎擾
	/// </summary>
	/// <returns></returns>
	virtual CameraProjProperty& GetCameraProperty() = 0;
	/// <summary>
	/// �J�����Ɍ`�󂪎ʂ��Ă��邩�̎擾
	/// </summary>
	/// <param name="arg_AABB">�`��</param>
	/// <returns></returns>
	virtual std::int32_t IsContaineVisibility(Value_ptr<Geometry::Box_AABB>arg_AABB) = 0;
	/// <summary>
	/// �I������
	/// </summary>
	virtual void End() = 0;
	/// <summary>
	/// �`�挋�ʂ��o�͂��郌���_�[�^�[�Q�b�g�e�N�X�`���̐ݒ�
	/// </summary>
	/// <param name="arg_tag">�`�挋�ʂ��o�͂��郌���_�[�^�[�Q�b�g�e�N�X�`��</param>
	virtual void SetProjectionTexture(const TextureTag& arg_tag) = 0;
	/// <summary>
	/// �Q�ƁA�o�͂���[�x�e�N�X�`���̐ݒ�
	/// </summary>
	/// <param name="arg_tag">�Q�ƁA�o�͂���[�x�e�N�X�`��</param>
	virtual void SetDepthStencilView(const TextureTag& arg_tag) = 0;
	/// <summary>
	/// �`��O����
	/// </summary>
	virtual void BefDraw() = 0;
	/// <summary>
	/// VP�s��̎擾
	/// </summary>
	/// <returns>VP�s��</returns>
	virtual Matrix4x4 GetViewProjectionMatrix() = 0;
	/// <summary>
	/// V�s��̎擾
	/// </summary>
	/// <returns>V�s��</returns>
	virtual Matrix4x4 GetViewMatrix() = 0;
	/// <summary>
	/// P�s��̎擾
	/// </summary>
	/// <returns>P�s��</returns>
	virtual Matrix4x4 GetProjectionMatrix() = 0;
	/// <summary>
	/// �J�����ʒu�̎擾
	/// </summary>
	/// <returns>�J�����ʒu</returns>
	virtual Vector3 GetPosition() = 0;
protected:
};
}
}

#endif // !ICAMERA_H
