#include"stdafx.h"

#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicDevice_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/Camera_Dx12.h"
#include"ButiRendering_Dx12/Header/Rendering_Dx12/GraphicResourceUtil_Dx12.h"
#include"../Header/Renderer.h"
ButiEngine::ButiRendering::Camera_Dx12::Camera_Dx12(const std::string& arg_cameraName, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
{
	vwp_graphicDevice = arg_vwp_graphicDevice;
	cameraViewProp.top = 0;
	cameraViewProp.left = 0;
	cameraViewProp.currentWidth = vwp_graphicDevice.lock()->GetApplication().lock()->GetWindow()->GetSize().x;
	cameraViewProp.currentHeight = vwp_graphicDevice.lock()->GetApplication().lock()->GetWindow()->GetSize().y;
	SetName(arg_cameraName);
	cameraViewProp = (CameraProjProperty());
	vlp_renderer = arg_vlp_renderer;
}

ButiEngine::ButiRendering::Camera_Dx12::Camera_Dx12(const CameraProjProperty& arg_cameraViewProp, const std::string& arg_cameraName, Value_ptr<IRenderer> arg_vlp_renderer, Value_weak_ptr<GraphicDevice_Dx12> arg_vwp_graphicDevice)
{
	cameraViewProp = arg_cameraViewProp;
	SetName(arg_cameraName);
	vlp_renderer = arg_vlp_renderer;
	vwp_graphicDevice = arg_vwp_graphicDevice;
}

ButiEngine::ButiRendering::Camera_Dx12::~Camera_Dx12()
{
}

void ButiEngine::ButiRendering::Camera_Dx12::Initialize()
{
	vec_vlp_renderTarget.clear();
	vlp_depthStencil = nullptr;
	if (cameraViewProp.angle <= 5) {
		cameraViewProp.angle = 60;
	}
#ifdef _EDITORBUILD
	if (!cameraViewProp.projectionTexture.size()) {
		cameraViewProp.currentWidth = cameraViewProp.width / 2;
		cameraViewProp.currentHeight = cameraViewProp.height / 2;
	}else
#endif // _EDITORBUILD

	{
		cameraViewProp.currentWidth = cameraViewProp.width;
		cameraViewProp.currentHeight = cameraViewProp.height;
	}

	if (cameraViewProp.isPararell) {
		
		projectionMatrix =Matrix4x4::OrthographicOffCenterLH(
			-(float)cameraViewProp.currentWidth / 2, (float)cameraViewProp.currentWidth / 2,
			-(float)cameraViewProp.currentHeight / 2, (float)cameraViewProp.currentHeight / 2,
			cameraViewProp.nearClip,
			cameraViewProp.farClip
		);
	}
	else {
		projectionMatrix =
			Matrix4x4::PersepectiveFovLH(
#ifdef _EDITORBUILD
				MathHelper::ToRadian(cameraViewProp.angle*0.5f),
#else
				MathHelper::ToRadian(cameraViewProp.angle),
#endif
				(float)cameraViewProp.currentWidth / (float)cameraViewProp.currentHeight,
				cameraViewProp.nearClip,
				cameraViewProp.farClip
			);
	}

	projectionMatrix.Transpose();

	viewport.TopLeftX = (float)cameraViewProp.left;
	viewport.TopLeftY = (float)cameraViewProp.top;
	viewport.Width = static_cast<FLOAT>(cameraViewProp.currentWidth);
	viewport.Height = static_cast<FLOAT>(cameraViewProp.currentHeight);
	viewport.MinDepth = cameraViewProp.front;
	viewport.MaxDepth = 1.0f;

	scissorRect.left = 0;
	scissorRect.right = cameraViewProp.currentWidth;
	scissorRect.top = 0;
	scissorRect.bottom = cameraViewProp.currentHeight;

	auto renderTargetTextures = cameraViewProp.projectionTexture;
	if (!renderTargetTextures.size() && cameraViewProp.cameraName != "editorMain") {
		renderTargetTextures.push_back( vwp_graphicDevice.lock()->GetDefaultRenderTarget());
	}
	for (auto itr = renderTargetTextures.begin(); itr != renderTargetTextures.end(); itr++) {
		if (!itr->IsEmpty()) {
			
			auto tex = vwp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetTexture(*itr);
			if (tex.lock()->IsThis<IRenderTarget>()) {
				vec_vlp_renderTarget.push_back( tex.lock()->GetThis<IRenderTarget>());
			}
		}
	}

	auto dsvTexture = cameraViewProp.depthStencilTexture;
	if ((!dsvTexture.IsEmpty())) {
		auto tex = vwp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetTexture(dsvTexture);
		if (tex.lock()->IsThis<IDepthStencil>()) {
			auto IDS = tex.lock()->GetThis<IDepthStencil>();
			vlp_depthStencil = IDS;
		}
	}



}

void ButiEngine::ButiRendering::Camera_Dx12::Start()
{
	if (!drawCommandList) {

		drawCommandList = CommandListHelper::CreateDefault(nullptr, vwp_graphicDevice.lock()->GetDevice(), vwp_graphicDevice.lock()->GetCommandAllocator());
		CommandListHelper::Close(drawCommandList);
	}
	CommandListHelper::Reset(nullptr, drawCommandList, vwp_graphicDevice.lock()->GetCommandAllocator());
	vwp_graphicDevice.lock()->SetCommandList(drawCommandList.Get());
	vwp_graphicDevice.lock()->DrawStart();

	if (vlp_depthStencil) {

		vwp_graphicDevice.lock()->GetCommandList().RSSetScissorRects(1, &scissorRect);
		vlp_depthStencil->SetDepthStencilView();
	}
	else {
		vwp_graphicDevice.lock()->CommandList_SetScissorRect();
		vwp_graphicDevice.lock()->ClearDepthStancil(1.0f);
		vwp_graphicDevice.lock()->SetDefaultDepthStencil();
	}

	if (vec_vlp_renderTarget.size()) {
		vlp_renderer->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / scissorRect.right, 1.0 / scissorRect.bottom);
		auto rtEnd = vec_vlp_renderTarget.end();
		for (auto itr = vec_vlp_renderTarget.begin(); itr != rtEnd; itr++) {
			(*itr)->SetRenderTarget(cameraViewProp.clearColor);
		}
	}
	else {
		auto size = vwp_graphicDevice.lock()->GetApplication().lock()->GetWindow()->GetSize();
		vlp_renderer->GetRendererCBuffer()->Get().pixelScale = Vector2(1.0 / size.x, 1.0 / size.y);
		vwp_graphicDevice.lock()->SetDefaultRenderTarget();
	}
	vwp_graphicDevice.lock()->CommandList_SetRenderTargetView();
	vwp_graphicDevice.lock()->SetCameraPos(cameraPos);
	vwp_graphicDevice.lock()->SetProjectionMatrix(projectionMatrix);
	vwp_graphicDevice.lock()->SetRawViewMatrix(viewMatrix);

	auto transposed = viewMatrix.Transpose();

	vwp_graphicDevice.lock()->SetViewMatrix(transposed);

	auto billboard= transposed;
	billboard._14 = 0.0f;
	billboard._24 = 0.0f;
	billboard._34 = 0.0f;

	billboard.Inverse();

	vwp_graphicDevice.lock()->SetViewMatrix_billBoard(billboard);
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardX(billboard.GetInValidYZ());
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardY(billboard.GetInValidXZ());
	vwp_graphicDevice.lock()->SetViewMatrix_billBoardZ(billboard.GetInValidXY());

	vwp_graphicDevice.lock()->GetCommandList().RSSetViewports(1, &viewport);

}

void ButiEngine::ButiRendering::Camera_Dx12::Stop() const
{
	auto rtEnd = vec_vlp_renderTarget.end();
	for (auto itr = vec_vlp_renderTarget.begin(); itr != rtEnd; itr++) {
		(*itr)->DisSetRenderTarget();
	}
	if (vlp_depthStencil) {
		vlp_depthStencil->DisSetDepthStencil();
	}
	CommandListHelper::Close(drawCommandList);
	vwp_graphicDevice.lock()->DrawEnd();

}

void ButiEngine::ButiRendering::Camera_Dx12::SetProjectionTexture(const TextureTag& arg_tag)
{

	if ((!arg_tag.IsEmpty())) {
		auto tex = vwp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetTexture(arg_tag);
		if (tex.lock()->IsThis<IRenderTarget>()) {
			vec_vlp_renderTarget.push_back(tex.lock()->GetThis<IRenderTarget>());
			cameraViewProp.projectionTexture.push_back(arg_tag);
		}
	}
	else {

	}
}

void ButiEngine::ButiRendering::Camera_Dx12::SetDepthStencilView(const TextureTag& arg_tag)
{
	cameraViewProp.depthStencilTexture = arg_tag;
	auto dsvTexture = cameraViewProp.depthStencilTexture;
	if ((!dsvTexture.IsEmpty())) {
		auto tex = vwp_graphicDevice.lock()->GetApplication().lock()->GetResourceContainer()->GetTexture(dsvTexture);
		if (tex.lock()->IsThis<IDepthStencil>()) {
			auto IDS = tex.lock()->GetThis<IDepthStencil>();
			vlp_depthStencil = IDS;
			std::int32_t i = 0;
		}
	}
}


void ButiEngine::ButiRendering::Camera_Dx12::ShowUI()
{
		//Transform
		if(GUI::TreeNode("Transform"))
		{
			GUI::Edit(*vlp_transform);
			std::string target = "BaseTransform:";
			if (vlp_transform->GetBaseTransform()) {
				target += "Existence";
			}
			else {
				target += "nullptr";
			}


			GUI::BeginChild("##BaseTransform", Vector2((GUI::GetFontSize()) * (target.size() + 2), GUI::GetFontSize() * 2), true);
			GUI::BulletText((target).c_str());
			if (vlp_transform->GetBaseTransform()) {
				GUI::SameLine();
				if (GUI::Button("Detach")) {
					vlp_transform->SetBaseTransform(nullptr);
				}
			}
			GUI::SameLine();
			if (GUI::Button("Attach New")) {
				if (!vlp_transform->GetBaseTransform())
					vlp_transform->SetBaseTransform(ObjectFactory::Create<Transform>(), true);
				else {
					vlp_transform->SetBaseTransform(vlp_transform->GetBaseTransform()->Clone(), true);
				}
			}
			if (GUI::IsWindowHovered())
			{
				auto obj =GUI::GetDraggingObject();

				if (obj && obj->IsThis<GameObject>()) {
					auto trans = obj->GetThis<GameObject>()->transform;
					vlp_transform->SetBaseTransform(trans, true);
				}

			}
			GUI::EndChild();
			if (vlp_transform->GetBaseTransform()) {
				GUI::BeginChild("BaseTransform");

				GUI::Edit(*vlp_transform->GetBaseTransform());
				GUI::EndChild();
			}
			GUI::TreePop();
		}

		GUI::Checkbox("isActive", &isActive);
		GUI::Checkbox("isInitActive", &cameraViewProp.isInitActive);
		GUI::Checkbox("isEditActive", &cameraViewProp.isEditActive);

		bool isEdit = false;

		if (GUI::Checkbox("isShadow", cameraViewProp.isShadow)) {
			isEdit = true;
		}

		if (GUI::ArrowButton("##minLayer", GUI::GuiDir_Left)) {
			if(cameraViewProp.layer!=0)
			cameraViewProp.layer--;
		}
		GUI::SameLine();
		GUI::Text(std::to_string(cameraViewProp.layer));
		GUI::SameLine();
		if (GUI::ArrowButton("##plusLayer", GUI::GuiDir_Right)) {
			cameraViewProp.layer++;
			cameraViewProp.layer = min(vlp_renderer->GetLayerCount()-1, cameraViewProp.layer);
		}

		if (GUI::DragInt2("##projectionSize", &cameraViewProp.width, 1, 0, 100000)) {
			if (!cameraViewProp.projectionTexture.size()) {
				cameraViewProp.currentWidth = cameraViewProp.width / 2;
				cameraViewProp.currentHeight = cameraViewProp.height / 2;
			}
			isEdit = true;
		}
		if (
			GUI::DragInt2("##projctionPos", &cameraViewProp.left, 1, 0, 100000)) {
			isEdit = true;
		}
		if (GUI::DragFloat2("##FarClipAndNearClip", &cameraViewProp.farClip, 0.01f, 0.01f, 100000000)) {
			isEdit = true;
		}
		if (GUI::DragFloat("##cameraAngle", cameraViewProp.angle, 0.01f, 5.0f, 360)) {
			isEdit = true;
		}
		if (GUI::Checkbox("isPararell", &cameraViewProp.isPararell)) {
			isEdit = true;
		}


		GUI::BulletText("ProjectionTextures");
		std::int32_t i = 0;
		for (auto itr= cameraViewProp.projectionTexture.begin(); itr!= cameraViewProp.projectionTexture.end(); i++) {

			auto tagName = itr->GetID();
			(GUI::BeginChild("ProjectionTexture:" + std::to_string(i) , Vector2(GUI::GetFontSize() * (tagName.size() + 2), GUI::GetFontSize() * 2), true));
			GUI::Text(Util::ToUTF8(tagName).c_str());

			GUI::SameLine();
			if (GUI::Button("Detach")) {
				itr=cameraViewProp.projectionTexture.erase(itr);
				isEdit = true;
			}
			else {
				itr++;
			}

			GUI::EndChild();
		}
		GUI::BeginChild("NewRenderTarget" , Vector2(GUI::GetFontSize()* (16), GUI::GetFontSize() * 2), true);
		GUI::Text("NewRenderTarget");
		if (GUI::IsWindowHovered()) {
			auto tag =GUI::GetTextureTag();
			if (!tag.IsEmpty()) {
				cameraViewProp.projectionTexture .push_back( tag);
				isEdit = true;
			}
		}
		GUI::EndChild();



		GUI::BulletText("DepthStencilTexture");
		auto tagName = cameraViewProp.depthStencilTexture.GetID();
		(GUI::BeginChild("DSVTagWin", Vector2(GUI::GetFontSize()* (tagName.size() + 2), GUI::GetFontSize() * 2), true));
		GUI::Text(Util::ToUTF8(tagName).c_str());

		if (GUI::IsWindowHovered()) {
			auto tag = GUI::GetTextureTag();
			if (!tag.IsEmpty()) {
				cameraViewProp.depthStencilTexture = tag;
				isEdit = true;
			}
		}
		GUI::SameLine();
		if (GUI::Button("Detach")) {
			cameraViewProp.depthStencilTexture = TextureTag();
			isEdit = true;
		}

		GUI::EndChild();

		GUI::ColorEdit4("ClearColor", cameraViewProp.clearColor);

		if (isEdit) {
			Initialize();
		}


}

void ButiEngine::ButiRendering::Camera_Dx12::End()
{
	auto rtEnd = vec_vlp_renderTarget.end();
	for (auto itr = vec_vlp_renderTarget.begin(); itr != rtEnd; itr++) {
		(*itr)->SetIsCleared(false);
	}
	if (vlp_depthStencil) {
		vlp_depthStencil->SetIsCleared(false);
	}
}
