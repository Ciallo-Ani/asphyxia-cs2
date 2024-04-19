#pragma once

#include <cstdint>
#include "./memory.h"

namespace PATCH
{
	struct mempatch_t
	{
		uint8_t* addrToPatch = nullptr;
		size_t bytesToPatch;

		bool operator()(uint8_t* addrToPatch, size_t bytesToPatch)
		{
			this->addrToPatch = addrToPatch;
			this->bytesToPatch = bytesToPatch;
		};

		bool operator()(const wchar_t* moduleName, const char* pattern, size_t bytesToPatch = 0)
		{
			this->addrToPatch = MEM::FindPattern(moduleName, CS_XOR(pattern));
			this->bytesToPatch = bytesToPatch;

			return this->addrToPatch != nullptr;
		};

		bool DoPatchNOP();
		bool DoPatchCustom(uint8_t byte, size_t offset);
	};

	bool SetupPatches();
	bool DoPatches();

	inline mempatch_t RotateFuncPatch1;
	inline mempatch_t RotateFuncPatch2;
	inline mempatch_t RotateFuncPatch3;
};
