#include "JWControlManager.h"

auto JWControlManager::Create(LPDIRECT3DDEVICE9 pDevice, LPD3DXFONT pFont)->JWERROR
{
	return JWControl::Create(pDevice, pFont);
}

void JWControlManager::Destroy()
{
	if (m_Controls.size())
	{
		for (JWControl* iterator : m_Controls)
		{
			delete iterator;
			iterator = nullptr;
		}
	}
}

void JWControlManager::DrawControls()
{
	// Draw if there is any control added
	if (m_Controls.size())
	{
		// @warning: The draw order is the added(newed) order
		for (JWControl* iterator : m_Controls)
		{
			if (iterator->GetControlType() == JWControl::CONTROL_TYPE::Label)
			{
				JWLabel* temp = dynamic_cast<JWLabel*>(iterator);
				temp->Draw();
				temp->DrawBorder();
			}
			else if (iterator->GetControlType() == JWControl::CONTROL_TYPE::Button)
			{
				JWButton* temp = dynamic_cast<JWButton*>(iterator);
				temp->Draw();
				temp->DrawBorder();
			}
		}
	}
}

void JWControlManager::UpdateControlStates(JWWinBase* pBase)
{
	JWControl* LastControlMouseOver = nullptr;
	for (JWControl* iterator : m_Controls)
	{
		iterator->UpdateControlState(pBase);

		if (iterator->GetControlState() != JWControl::CONTROL_STATE::Normal)
			LastControlMouseOver = iterator;

		// @warning
		// Do not skip the loop although it found a control which has the mouse pointer over
		// because if there are many controls at the same place, the last one will be selected!
	}

	if (LastControlMouseOver)
	{
		// If there is a control which has the mouse pointer over
		
	}
}