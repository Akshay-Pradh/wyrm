#pragma once

#include <vector>
#include <wyrm/types.hpp> 

uint32_t PackBodyFlags(const WyrmRigidBody& b);
uint32_t PackFrameFlags(const WyrmFrame& f);
void UnpackBodyFlags(uint8_t flags, WyrmRigidBody& b);
void UnpackFrameFlags(uint8_t flags, WyrmFrame& f);

std::vector<uint8_t> SerializeFrame(const WyrmFrame& frame);
std::vector<uint8_t> SerializeDescriptions(const std::unordered_map<int32_t, WyrmDescription>& descriptions);