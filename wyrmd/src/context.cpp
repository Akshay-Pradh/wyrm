#include "context.hpp"
#include "net.hpp"

void BuildDescriptionTable(sDataDescriptions* desc, WyrmContext& ctx) {
    for (int i = 0; i < desc->nDataDescriptions; i++) {
        if (desc->arrDataDescriptions[i].type == Descriptor_RigidBody) {
            WyrmDescription d = ToWyrmDescription(
                *desc->arrDataDescriptions[i].Data.RigidBodyDescription
            );
            ctx.descriptions[d.id] = d;
        }
    }
}