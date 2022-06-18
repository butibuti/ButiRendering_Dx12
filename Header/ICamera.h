#ifndef ICAMERA_H
#define ICAMERA_H
#include"ButiMath/ButiMath.h"
#include"ButiMath/Transform.h"
#include <vector>
#include"ResourceInterface.h"
#include"RenderingStatus.h"
namespace ButiEngine {
namespace Geometry {
class Box_AABB;
}
namespace ButiRendering {
class IRenderer;
class GraphicDevice;
struct CameraProperty {
	CameraProperty() {};
	BUTIRENDERING_API CameraProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell = false, std::uint32_t arg_layer = 0);

	std::int32_t currentWidth = 0, currentHeight = 0, width=0, height=0, left = 0, top = 0;
	float front = 0.0f, angle = 60.0f, farClip = 50.0f, nearClip = 0.1f;
	bool isPararell = false;
	std::uint32_t layer = 0;
	std::int32_t renderingInfo = 0;
	std::string cameraName;
	Vector4 clearColor;
	bool isInitActive = true, isEditActive = false, isShadow = false;

	List<Value_weak_ptr<IRenderTarget>> m_list_vlp_renderTarget;
	Value_weak_ptr<IDepthStencil> m_depthStencilTexture;
	List<Value_weak_ptr<IResource_Texture>> m_list_vlp_shadowTexture;
	List<Value_weak_ptr<IResource_Texture>> m_list_vlp_staticShadowTexture;
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
		archive(cameraName);
		archive(clearColor);
		archive(isInitActive);
		archive(isEditActive);
		archive(isShadow);
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
	/// �J�����ݒ�̎擾
	/// </summary>
	/// <returns></returns>
	virtual CameraProperty& GetCameraProperty() = 0;
	/// <summary>
	/// �J�����ݒ�̎擾
	/// </summary>
	/// <returns></returns>
	virtual const CameraProperty& GetCameraProperty() const= 0;
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
	/// <param name="arg_renderTarget">�`�挋�ʂ��o�͂��郌���_�[�^�[�Q�b�g�e�N�X�`��</param>
	virtual void SetRenderTarget(Value_ptr<IRenderTarget> arg_renderTarget) = 0;
	/// <summary>
	/// �Q�ƁA�o�͂���[�x�e�N�X�`���̐ݒ�
	/// </summary>
	/// <param name="arg_depthStencil">�Q�ƁA�o�͂���[�x�e�N�X�`��</param>
	virtual void SetDepthStencil(Value_ptr<IDepthStencil> arg_depthStencil) = 0;
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
	virtual Vector3 WorldToScreen(const Vector3& arg_pos)const = 0;
	virtual Value_weak_ptr<IRenderer> GetRenderer()const = 0;
	virtual Value_ptr<GraphicDevice> GetGraphicDevice()const = 0;
protected:
};
BUTIRENDERING_API Value_ptr<ICamera> CreateCamera(const CameraProperty& arg_cameraProp, const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<ICamera> CreateCamera(const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
}
}

#endif // !ICAMERA_H
