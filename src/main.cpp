#include "SteamWorkaround.h"
#include "MenuHandler.h"

bool InitModCore(const F4SE::QueryInterface* a_f4se)
{
	if (a_f4se->IsEditor())
	{
		REX::ERROR("Loaded in editor.");
		return false;
	}

	const auto ver = a_f4se->RuntimeVersion();
	if (ver < F4SE::RUNTIME_1_11_159)
	{
		auto verString = ver.string();
		REX::ERROR(std::format("unsupported runtime v{}", verString));
		return false;
	}

	return true;
}

void OnMessage(F4SE::MessagingInterface::Message* message)
{
	if (message->type == F4SE::MessagingInterface::kGameDataReady)
	{
		SteamWorkaround::Hook();

		MenuHandler::Register();

		REX::INFO("Finished initialization.");
	}
}

F4SE_PLUGIN_LOAD(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);

	InitModCore(a_f4se);

	F4SE::GetMessagingInterface()->RegisterListener(OnMessage);

	return true;
}
