#include "stdafx.h"
#include <dinput.h>
#include "Actor.h"
#include "Torch.h"
#include "trade.h"
#include "../CameraBase.h"
#include "Car.h"
#include "HudManager.h"
#include "inventory.h"
#include "level.h"
#include "game_cl_base.h"
#include "xr_level_controller.h"
#include "UsableScriptObject.h"
#include "clsid_game.h"
#include "actorcondition.h"

void CActor::IR_OnKeyboardPress(int cmd)
{
	if (Remote())		return;
	if (conditions().IsSleeping())	return;
	if (IsTalking())	return;
	if (IsControlled())	return;


	if (!g_Alive()) return;

	if(m_holder && kUSE != cmd)
	{
		m_holder->OnKeyboardPress(cmd);
		if (m_holder->allowWeapon() && inventory().Action(cmd, CMD_START))		return;
		return;
	}
	else
	{
		if (inventory().Action(cmd, CMD_START))					return;
	}

	switch(cmd){
	case kACCEL:	mstate_wishful |= mcAccel;					break;
	case kL_STRAFE:	mstate_wishful |= mcLStrafe;				break;
	case kR_STRAFE:	mstate_wishful |= mcRStrafe;				break;
	case kL_LOOKOUT:
					mstate_wishful |= mcLLookout;				
//					mstate_wishful &= ~mcAnyMove; 
					break;
	case kR_LOOKOUT:
					mstate_wishful |= mcRLookout;				
//					mstate_wishful &= ~mcAnyMove; 
					break;
	case kFWD:		mstate_wishful |= mcFwd;					break;
	case kBACK:		mstate_wishful |= mcBack;					break;
	case kJUMP:		mstate_wishful |= mcJump;					break;
	case kCROUCH:	mstate_wishful |= mcCrouch;					break;
	case kCROUCH_TOGGLE:	
		{
			if (mstate_wishful & mcCrouch)
				mstate_wishful &=~mcCrouch;
			else
				mstate_wishful |= mcCrouch;					
		}break;
	case kSPRINT_TOGGLE:	
		{
			if (mstate_wishful & mcSprint)
				mstate_wishful &=~mcSprint;
			else
				mstate_wishful |= mcSprint;					
		}break;
	case kCAM_1:	cam_Set			(eacFirstEye);				break;
	case kCAM_2:	cam_Set			(eacLookAt);				break;
	case kCAM_3:	cam_Set			(eacFreeLook);				break;

	case kWPN_1:	
	case kWPN_2:	
	case kWPN_3:	
	case kWPN_4:	
		break;
	case kWPN_RELOAD:
		//Weapons->Reload			();
		break;
	case kUSE:
		ActorUse();
		break;
	case kDROP:
		b_DropActivated			= TRUE;
		f_DropPower				= 0;
		break;
	}
}

void CActor::IR_OnKeyboardRelease(int cmd)
{
	if (Remote())		return;
	if (conditions().IsSleeping())	return;
	if (IsControlled())	return;

	if (g_Alive())	
	{
		if (cmd == kUSE) 
			PickupModeOff();

		if(m_holder)
		{
			m_holder->OnKeyboardRelease(cmd);
			
			if(m_holder->allowWeapon() && inventory().Action(cmd, CMD_STOP))		return;
			return;
		}
		else
		{
			if (inventory().Action(cmd, CMD_STOP))		return;
		}
		
		switch(cmd)
		{
		case kACCEL:	mstate_wishful &=~mcAccel;		break;
		case kL_STRAFE:	mstate_wishful &=~mcLStrafe;	break;
		case kR_STRAFE:	mstate_wishful &=~mcRStrafe;	break;
		case kL_LOOKOUT:mstate_wishful &=~mcLLookout;	break;
		case kR_LOOKOUT:mstate_wishful &=~mcRLookout;	break;
		case kFWD:		mstate_wishful &=~mcFwd;		break;
		case kBACK:		mstate_wishful &=~mcBack;		break;
		case kJUMP:		mstate_wishful &=~mcJump;		break;
		case kCROUCH:	mstate_wishful &=~mcCrouch;		break;

		case kDROP:		g_PerformDrop();				break;
		}
	}
}

void CActor::IR_OnKeyboardHold(int cmd)
{
	if (Remote() || !g_Alive())		return;
	if (conditions().IsSleeping())				return;
	if (IsControlled())				return;
	if (IsTalking())				return;

	if(m_holder)
	{
		m_holder->OnKeyboardHold(cmd);
		return;
	}

	switch(cmd)
	{
	case kUP:
	case kDOWN: 
	case kCAM_ZOOM_IN: 
	case kCAM_ZOOM_OUT: 
		cam_Active()->Move(cmd); break;
	case kLEFT:
	case kRIGHT:
		if (eacFreeLook!=cam_active) cam_Active()->Move(cmd); break;
	}
}

void CActor::IR_OnMouseMove(int dx, int dy)
{
	if (Remote())		return;
	if (conditions().IsSleeping())	return;

	if(m_holder) 
	{
		m_holder->OnMouseMove(dx,dy);
		return;
	}

	if (!IsControlled()) m_controlled_mouse_scale_factor = 1.0f;
	VERIFY(!fis_zero(m_controlled_mouse_scale_factor));

	CCameraBase* C	= cameras	[cam_active];
	float scale		= (C->f_fov/ g_fov)*psMouseSens * psMouseSensScale/50.f  / m_controlled_mouse_scale_factor;
	if (dx){
		float d = float(dx)*scale;
		cam_Active()->Move((d<0)?kLEFT:kRIGHT, _abs(d));
	}
	if (dy){
		float d = ((psMouseInvert.test(1))?-1:1)*float(dy)*scale*3.f/4.f;
		cam_Active()->Move((d>0)?kUP:kDOWN, _abs(d));
	}
}

void CActor::ActorUse()
{
	mstate_real = 0;
	PickupModeOn();
				
	if(m_PhysicMovementControl->PHCapture())
		m_PhysicMovementControl->PHReleaseObject();

	

	if(m_pUsableObject)m_pUsableObject->use(this);
	

	if(!m_pUsableObject||m_pUsableObject->nonscript_usable())
	{
		collide::rq_result& RQ = HUD().GetCurrentRayQuery();
		CPhysicsShellHolder* object = smart_cast<CPhysicsShellHolder*>(RQ.O);
		u16 element = BI_NONE;
		if(object) 
			element = (u16)RQ.element;

		if(Level().IR_GetKeyState(DIK_LSHIFT))
		{
	
			if(!m_PhysicMovementControl->PHCapture())
			{
				m_PhysicMovementControl->PHCaptureObject(object,element);
			
			}

		}
	}


}
//void CActor::IR_OnMousePress(int btn)

BOOL CActor::HUDview()const
{
	return IsFocused() && (cam_active == eacFirstEye) &&
		((!m_holder) || (m_holder && m_holder->allowWeapon() && m_holder->HUDView()));
}