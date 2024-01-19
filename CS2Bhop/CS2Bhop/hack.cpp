#include "memory/memory.h"
#include <iostream>
#include <thread>

namespace offsets {
	constexpr std::ptrdiff_t dwEntityList = 24910160;
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 23891800;
	constexpr std::ptrdiff_t m_iIDEntIndex = 5444;
	constexpr std::ptrdiff_t m_iTeamNum = 959;
	constexpr std::ptrdiff_t m_iHealth = 812;
	constexpr std::ptrdiff_t spotted = 8;
}

int main() {
	Memory mem{ "cs2.exe" };

	const auto client = mem.GetModuleAddress("client.dll");

	std::cout << "Client.dll -> " << std::hex << client << std::dec << std::endl;
	std::cout << "[-] Triggerbot started" << std::endl;
	std::cout << "[-] Triggerbot key SHIFT" << std::endl;


	while (true) {
		if (GetAsyncKeyState(VK_SHIFT)) {

			const auto localPlayer = mem.Read<LONGLONG>(client + offsets::dwLocalPlayerPawn);
			const auto playerTeam = mem.Read<int>(localPlayer + offsets::m_iTeamNum);
			const auto entityId = mem.Read<int>(localPlayer + offsets::m_iIDEntIndex);

			if (localPlayer) {
				auto health = mem.Read<LONGLONG>(localPlayer + offsets::m_iHealth);

				if (health && entityId > 0) {
					auto entList = mem.Read<LONGLONG>(client + offsets::dwEntityList);

					auto entEntry = mem.Read<LONGLONG>(entList + 0x8 * (entityId >> 9) + 0x10);
					auto entity = mem.Read<LONGLONG>(entEntry + 120 * (entityId & 0x1FF));

					auto entityTeam = mem.Read<int>(entity + offsets::m_iTeamNum);

					if (entityTeam != playerTeam) {
						auto entityHp = mem.Read<int>(entity + offsets::m_iHealth);
						if (entityHp > 0) {
							std::this_thread::sleep_for(std::chrono::milliseconds(50));
							mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
							std::this_thread::sleep_for(std::chrono::milliseconds(30));
							mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
						}
					}
				}
			}

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return 0;
}