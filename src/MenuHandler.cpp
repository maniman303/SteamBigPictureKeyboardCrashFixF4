#include "MenuHandler.h"
#include "SteamWorkaround.h"

class MenuOpenCloseSink : public RE::BSTEventSink<RE::MenuOpenCloseEvent>
{
public:
    static MenuOpenCloseSink* GetSingleton()
    {
        static MenuOpenCloseSink singleton;
        return std::addressof(singleton);
    }

private:
    RE::BSEventNotifyControl ProcessEvent(const RE::MenuOpenCloseEvent& aEvent, RE::BSTEventSource<RE::MenuOpenCloseEvent>*) override
    {
        SteamWorkaround::SetMenu(aEvent.menuName.c_str(), aEvent.opening);

        return RE::BSEventNotifyControl::kContinue;
    }
};

void MenuHandler::Register()
{
    auto mgr = RE::UI::GetSingleton();
    if (mgr == NULL)
    {
        REX::WARN("UI is null.");

        return;
    }

    auto sink = MenuOpenCloseSink::GetSingleton();
    if (sink == NULL)
    {
        REX::WARN("MenuOpenCloseSink is null.");

        return;
    }

    mgr->UnregisterSink(sink);
    mgr->RegisterSink(sink);

    REX::INFO("MenuOpenClose event registered.");
}