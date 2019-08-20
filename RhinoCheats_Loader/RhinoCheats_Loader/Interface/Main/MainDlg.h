#pragma once

#include <time.h>
#include <thread>
#include <mutex>

#include "../../Networking/Network.h"

#include "../../Resources/Resource.h"
#include "../WinAPI/Button.hpp"
#include "../WinAPI/Dialog.hpp"
#include "../WinAPI/ListView.hpp"
#include "../WinAPI/Message.hpp"
#include "../WinAPI/ProgressBar.hpp"
#include "../WinAPI/Static.hpp"
#include "../WinAPI/StatusBar.hpp"

#include "../../Security/Encryption/Encryption.h"

#include <src/BlackBone/Process/Process.h>
#include <src/BlackBone/Misc/Utils.h>

#include <VMProtectSDK.h>

class cMainDlg : public cDialog
{
public:

	cMainDlg(HINSTANCE instance, std::vector<std::string>& logindata, std::string& info_from_server);
	~cMainDlg();
	
	bool RefreshClientInfo(std::string username, std::string password, std::string hwid);

private:

	typedef enum
	{
		Game,
		Subscription,
		Expiration
	} eListViewColumn;

	typedef enum
	{
		_1Day,
		_1Week,
		_2Weeks,
		_1Month,
		_3Months,
		_6Months,
		_1Year,
		_Lifetime,
		_SpecialAccess,
	} eSubscriptionType;

	typedef struct
	{
		std::string name;
		std::string process;
		std::string link;
		std::string subscription;
		std::string date_start;
		std::string date_end;
	} Rhinocheats;

	std::string GetClientInfo(std::string sData);
	void AddCheatToList(Rhinocheats* cheat);
	void Inject();

	DLG_HANDLER(OnInit);
	DLG_HANDLER(OnClose);
	DLG_HANDLER(OnRefresh); // trigged by clicking the 'Logout' menu item
	DLG_HANDLER(OnLogout); // trigged by clicking the 'Logout' menu item
	DLG_HANDLER(OnInject); // trigged by clicking the 'Inject' button

protected:

	NTSTATUS _lastStatus; // for the injection procedure

	ctrl::cStatic _clientInfo; // string with name and number of cheats acquired
	ctrl::cListView _cheatList; // list of cheats with subscription type and expriation date
	ctrl::cButton _inject;
	ctrl::cProgressBar _injectionState;
	ctrl::cStatusBar _info;

	blackbone::Process _process;
	std::mutex _lock;

	HINSTANCE _instance; // passed in through WinMain and this class's default constructor
	std::vector<std::string>& _logindata;
	std::string& _info_from_server;
	std::vector<Rhinocheats> cheatList;

	std::string _clientInfoFormat;
};