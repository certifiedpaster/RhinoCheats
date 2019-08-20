#pragma once

#include "../../Networking/Network.h"

#include "../../Resources/Resource.h"
#include "../WinAPI/Dialog.hpp"
#include "../WinAPI/EditBox.hpp"
#include "../WinAPI/Message.hpp"
#include "../WinAPI/Static.hpp"

#include "../../Security/Encryption/Encryption.h"

#include <src/BlackBone/Process/Process.h>
#include <src/BlackBone/Misc/Utils.h>

#include <VMProtectSDK.h>

class cLoginDlg : public cDialog
{
public:

	cLoginDlg(HINSTANCE instance);
	~cLoginDlg();

	bool ValidateClientInfo(std::string username, std::string password);

	std::vector<std::string> _loginData;
	std::string _info_from_server;

private:

	DLG_HANDLER(OnInit);
	DLG_HANDLER(OnClose);
	DLG_HANDLER(OnOK);
	DLG_HANDLER(OnCancel);	

protected:

	ctrl::cEditBox _editUserName;
	ctrl::cEditBox _editPassWord;

	HINSTANCE _instance; // passed in through WinMain and this class's default constructor
};