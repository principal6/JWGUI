#pragma once

#include "Core/JWWinBase.h"
#include "Control/JWLabel.h"
#include "Control/JWButton.h"
#include "Control/JWImage.h"

class JWControlManager final : public JWControl
{
private:

public:
	VECTOR<JWControl*> m_Controls;

public:
	JWControlManager() {};
	~JWControlManager() {};

	// Creation & destruction
	auto JWControlManager::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->JWERROR override;
	void JWControlManager::Destroy();

	// Update
	void JWControlManager::UpdateControlStates(JWWinBase* pBase);

	// Draw
	void JWControlManager::DrawControls();
};