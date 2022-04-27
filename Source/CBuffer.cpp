#include "stdafx.h"

void ButiEngine::ButiRendering::ICBuffer::ShowUI()
{
	if (GUI::TreeNode((exName+"##"+std::to_string(slot)).c_str())) {
		static char cbufferName[128];
		GUI::InputTextWithHint("Name", exName.c_str(), cbufferName, 64, 64);
		GUI::SameLine();

		if (GUI::Button("Change")) {
			exName = (cbufferName);
		}
		if (OnShowUI()) {
			Update();
		}

		GUI::TreePop();
	}
}