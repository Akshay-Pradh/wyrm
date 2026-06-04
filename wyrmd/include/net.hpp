#pragma once

#include <wyrm/types.hpp> 
#include "NatNetTypes.h"
#include "NatNetClient.h"
#include "context.hpp"

WyrmRigidBody ToWyrmBody(const sRigidBodyData& src,
                                 const DescriptionTable& descriptions);
WyrmFrame ToWyrmFrame(const sFrameOfMocapData& src,
                              const DescriptionTable& descriptions);
WyrmDescription ToWyrmDescription(const sRigidBodyDescription& src);

void ConnectToServer(NatNetClient& client, const WyrmConfig& cfg);
void NATNET_CALLCONV frame_callback(sFrameOfMocapData* data, void* user_data);