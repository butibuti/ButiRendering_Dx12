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
	BUTIRENDERING_API CameraProperty(const std::uint32_t arg_widthScale, const std::uint32_t arg_heightScale, const std::uint32_t arg_x, const std::uint32_t arg_y, const bool arg_isPararell = false);

	std::int32_t currentWidth = 0, currentHeight = 0, width=0, height=0, left = 0, top = 0;
	float front = 0.0f, angle = 60.0f, farClip = 50.0f, nearClip = 0.1f;
	bool isPararell = false;
	std::int32_t renderingInfo = 0;
	std::string cameraName;
	Vector4 clearColor;

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
		archive(front);
		archive(renderingInfo);
		archive(cameraName);
		archive(clearColor);
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

	virtual void UpdateMatrix() = 0;
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

	virtual Value_ptr<Transform> GetTransform()const = 0;
protected:
};
BUTIRENDERING_API Value_ptr<ICamera> CreateCamera(const CameraProperty& arg_cameraProp, const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
BUTIRENDERING_API Value_ptr<ICamera> CreateCamera(const std::string& cameraName, const bool initActive, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice> arg_vwp_graphicDevice);
}
}

#endif // !ICAMERA_H
