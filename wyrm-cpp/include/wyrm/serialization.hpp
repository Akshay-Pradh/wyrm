/**
 * Copyright [2026] [Nicholas Sutton]
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#pragma once

#include <vector>
#include <unordered_map>
#include <wyrm/types.hpp>

/**
 * Serializes a WyrmFrame struct using FastCDR
 * @param frame The frame to serialize
 * @return A vector of bytes
 */
std::vector<uint8_t> SerializeFrame(const WyrmFrame& frame);

/**
 * Serializes a WyrmDescriptions Map using FastCDR
 * @param frame The descriptions to serialize
 * @return A vector of bytes
 */
std::vector<uint8_t> SerializeDescriptions(const std::unordered_map<int32_t, WyrmDescription>& descriptions);

/**
 * Deserializes a WyrmFrame struct using FastCDR
 * @param data a vector of bytes to deserialize
 * @return A WyrmFrame Struct
 */
WyrmFrame DeserializeFrame(const std::vector<uint8_t>& data);

/**
 * Deserializes a WyrmDescriptions Map using FastCDR
 * @param data a vector of bytes to deserialize
 * @return A WyrmDescriptions Map
 */
std::unordered_map<int32_t, WyrmDescription> DeserializeDescriptions(const std::vector<uint8_t>& data);