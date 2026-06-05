#include <catch2/catch_test_macros.hpp>
#include <cstring>
#include "NatNetTypes.h"
#include "net.hpp"
#include <wyrm/types.hpp>

TEST_CASE("A NatNet RigidBody can be converted to a wyrm rigid body") {
    sRigidBodyData b{};
    b.ID        = 1;
    b.x         = 1.0f;
    b.y         = 2.0f;
    b.z         = 3.0f;
    b.qx        = 0.0f;
    b.qy        = 0.0f;
    b.qz        = 0.0f;
    b.qw        = 1.0f;
    b.MeanError = 0.5f;
    b.params    = 3; // bit0 = tracking_lost, bit1 = model_filled

    WyrmDescription wd{};
    wd.id          = 1;
    wd.parent_id   = -1;
    wd.name        = "Body1";
    wd.num_markers = 5;

    std::unordered_map<int32_t, WyrmDescription> d = {{b.ID, wd}};

    WyrmRigidBody wb = ToWyrmBody(b, d);

    REQUIRE(wb.id            == b.ID);
    REQUIRE(wb.x             == b.x);
    REQUIRE(wb.y             == b.y);
    REQUIRE(wb.z             == b.z);
    REQUIRE(wb.qx            == b.qx);
    REQUIRE(wb.qy            == b.qy);
    REQUIRE(wb.qz            == b.qz);
    REQUIRE(wb.qw            == b.qw);
    REQUIRE(wb.mean_error    == b.MeanError);
    REQUIRE(wb.tracking_lost == true);
    REQUIRE(wb.model_filled  == true);
    REQUIRE(wb.name          == wd.name);
}

TEST_CASE("A NatNet Frame can be converted to a wyrm frame") {
    sRigidBodyData b{};
    b.ID        = 1;
    b.x         = 1.0f;
    b.y         = 2.0f;
    b.z         = 3.0f;
    b.qx        = 0.0f;
    b.qy        = 0.0f;
    b.qz        = 0.0f;
    b.qw        = 1.0f;
    b.MeanError = 0.5f;
    b.params    = 0;

    WyrmDescription wd{};
    wd.id          = 1;
    wd.parent_id   = -1;
    wd.name        = "Body1";
    wd.num_markers = 5;

    std::unordered_map<int32_t, WyrmDescription> d = {{b.ID, wd}};

    sFrameOfMocapData f{};
    f.iFrame         = 42;
    f.fTimestamp     = 1.5;
    f.params         = 1; // is_recording
    f.nRigidBodies   = 1;
    f.RigidBodies[0] = b;

    WyrmFrame wf = ToWyrmFrame(f, d);

    REQUIRE(wf.frame_id             == f.iFrame);
    REQUIRE(wf.timestamp            == f.fTimestamp);
    REQUIRE(wf.body_count           == 1);
    REQUIRE(wf.is_recording         == true);
    REQUIRE(wf.model_list_changed   == false);
    REQUIRE(wf.bodies.size()        == 1);
    REQUIRE(wf.bodies[0].id         == b.ID);
    REQUIRE(wf.bodies[0].name       == wd.name);
}

TEST_CASE("A NatNet RigidBodyDescription can be converted to a wyrm description") {
    sRigidBodyDescription rd{};
    rd.ID       = 3;
    rd.parentID = -1;
    rd.nMarkers = 7;
    std::strncpy(rd.szName, "body1", sizeof(rd.szName) - 1);

    WyrmDescription wd = ToWyrmDescription(rd);

    REQUIRE(wd.id          == rd.ID);
    REQUIRE(wd.parent_id   == rd.parentID);
    REQUIRE(wd.num_markers == rd.nMarkers);
    REQUIRE(wd.name        == "body1");
}
