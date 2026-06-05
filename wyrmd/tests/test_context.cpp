#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "NatNetTypes.h"
#include "context.hpp"
#include "net.hpp"

TEST_CASE("BuildDescriptionTable adds rigid body descriptions to the context") {
    sRigidBodyDescription rd{};
    rd.ID       = 1;
    rd.parentID = -1;
    rd.nMarkers = 5;
    std::strncpy(rd.szName, "Pelvis", sizeof(rd.szName) - 1);

    sDataDescription data_desc{};
    data_desc.type                       = Descriptor_RigidBody;
    data_desc.Data.RigidBodyDescription  = &rd;

    sDataDescriptions descriptions{};
    descriptions.nDataDescriptions       = 1;
    descriptions.arrDataDescriptions[0]  = data_desc;

    WyrmContext ctx{};
    BuildDescriptionTable(&descriptions, ctx);

    REQUIRE(ctx.descriptions.size()          == 1);
    REQUIRE(ctx.descriptions.count(1)        == 1);
    REQUIRE(ctx.descriptions[1].id           == 1);
    REQUIRE(ctx.descriptions[1].name         == "Pelvis");
    REQUIRE(ctx.descriptions[1].num_markers  == 5);
}

TEST_CASE("BuildDescriptionTable ignores non-rigid-body descriptions") {
    sRigidBodyDescription rd{};
    rd.ID       = 1;
    rd.parentID = -1;
    rd.nMarkers = 5;
    std::strncpy(rd.szName, "Pelvis", sizeof(rd.szName) - 1);

    sDataDescription rigid_desc{};
    rigid_desc.type                      = Descriptor_RigidBody;
    rigid_desc.Data.RigidBodyDescription = &rd;

    sDataDescription other_desc{};
    other_desc.type                      = Descriptor_MarkerSet;

    sDataDescriptions descriptions{};
    descriptions.nDataDescriptions       = 2;
    descriptions.arrDataDescriptions[0]  = rigid_desc;
    descriptions.arrDataDescriptions[1]  = other_desc;

    WyrmContext ctx{};
    BuildDescriptionTable(&descriptions, ctx);

    REQUIRE(ctx.descriptions.size() == 1);
}
