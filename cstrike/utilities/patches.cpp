#include "patches.h"
#include <Windows.h>

bool PATCH::mempatch_t::DoPatchNOP()
{
	if (!addrToPatch)
		return false;
	if (!MEM::SetMemAccess(addrToPatch, bytesToPatch, PAGE_EXECUTE_READWRITE))
		return false;

	for (size_t i = 0; i < bytesToPatch; i++)
	{
		uint8_t* byteToPatch = (uint8_t*)(addrToPatch + i);
		*byteToPatch = 0x90;
	}

	if (!MEM::SetMemAccess(addrToPatch, bytesToPatch, PAGE_EXECUTE_READ))
		return false;

	return true;
}

bool PATCH::mempatch_t::DoPatchCustom(uint8_t byte, size_t offset)
{
	uint8_t* byteToPatch = (uint8_t*)(addrToPatch + offset);
	if (!addrToPatch || !byteToPatch)
		return false;

	if (!MEM::SetMemAccess(byteToPatch, 1, PAGE_EXECUTE_READWRITE))
		return false;

	*byteToPatch = byte;

	if (!MEM::SetMemAccess(byteToPatch, 1, PAGE_EXECUTE_READ))
		return false;

	return true;
}

bool PATCH::SetupPatches()
{
	bool bSuccess = true;
	bSuccess &= RotateFuncPatch1(CLIENT_DLL, "48 8D 54 24 20 48 8B C8 E8 FC 00 7E FF", 17);
	bSuccess &= RotateFuncPatch2(CLIENT_DLL, "84 C0 0F 85 A2 01 00 00", 8);
	bSuccess &= RotateFuncPatch3(CLIENT_DLL, "44 38 BD A1 07 00 00 74 05", 1);

	bSuccess &= DoPatches();

	return bSuccess;
}

bool PATCH::DoPatches()
{
	auto bSuccess = true;
	bSuccess &= RotateFuncPatch1.DoPatchNOP();
	bSuccess &= RotateFuncPatch2.DoPatchNOP();
	bSuccess &= RotateFuncPatch3.DoPatchCustom(0x75, 7);
	//bSuccess &= RotateFuncPatch3.DoPatchNOP();

	return bSuccess;
}
