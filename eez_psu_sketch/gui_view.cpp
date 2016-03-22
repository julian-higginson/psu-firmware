/*
 * EEZ PSU Firmware
 * Copyright (C) 2015 Envox d.o.o.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "psu.h"

namespace eez {
namespace psu {
namespace gui {

uint8_t document[2556] = {
    0x1C, 0x06, 0x00, 0x05, 0x72, 0x01, 0x02, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x03, 0x14, 0x00,
    0xBA, 0x55, 0x00, 0xF8, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14, 0x00, 0xBA, 0x55, 0xE0,
    0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14, 0x00, 0xBA, 0x55, 0x00, 0xF8, 0x9A, 0xD6,
    0x00, 0x00, 0x00, 0x00, 0x03, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10,
    0x00, 0xBA, 0x55, 0x10, 0x84, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0xBA, 0x55,
    0xF9, 0xCF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0xBA, 0x55, 0x00, 0xF8, 0x9A,
    0xD6, 0x00, 0x00, 0x00, 0x00, 0x01, 0x14, 0x00, 0xBA, 0x55, 0xFF, 0xFF, 0x9A, 0xD6, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x14, 0x00, 0xDD, 0xAE, 0xEF, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x04, 0x14, 0x00, 0xBF,
    0xEF, 0x65, 0x29, 0x9A, 0xD6, 0x04, 0x00, 0x00, 0x00, 0x02, 0x15, 0x00, 0xE0, 0xFF, 0xBA, 0x55,
    0xBA, 0x55, 0x00, 0x00, 0x00, 0x00, 0x04, 0x15, 0x00, 0xE0, 0xFF, 0xBA, 0x55, 0xBA, 0x55, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x14, 0x00, 0xBA, 0x55, 0x3E, 0xCF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00,
    0x03, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14, 0x00,
    0xBA, 0x55, 0xFF, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14, 0x00, 0xBA, 0x55, 0x00,
    0xF8, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x14, 0x00, 0xBA, 0x55, 0xFF, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x14, 0x00, 0xBA, 0x55, 0x00, 0xF8, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x01, 0x14,
    0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x01, 0x14, 0x00, 0xBF, 0xEF,
    0x65, 0x29, 0x9A, 0xD6, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A,
    0xD6, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14, 0x00, 0xBA, 0x55, 0xE0, 0xFF, 0x9A, 0xD6, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x78, 0xA0, 0x06, 0x00, 0xA9, 0x01, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x78, 0xA0, 0x06, 0x00, 0xAC, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x78, 0xA0, 0x06,
    0x00, 0xAF, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x78, 0xA0, 0x06, 0x00, 0xB2, 0x01, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x78, 0xA0, 0x06, 0x00, 0xB5, 0x01, 0x05, 0xB8, 0x01, 0x08, 0xEF, 0x01, 0x08,
    0x47, 0x02, 0x08, 0x9F, 0x02, 0x04, 0xF7, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x78, 0xA0, 0x06,
    0x00, 0x23, 0x03, 0x06, 0x00, 0x00, 0x00, 0x8A, 0x19, 0x16, 0x95, 0x00, 0x25, 0x03, 0x06, 0x00,
    0x00, 0x19, 0x8A, 0x19, 0x16, 0x95, 0x00, 0x27, 0x03, 0x06, 0x00, 0x00, 0x32, 0x8A, 0x23, 0x16,
    0x95, 0x00, 0x29, 0x03, 0x06, 0x00, 0x07, 0x55, 0x8A, 0x23, 0x16, 0x95, 0x00, 0x2B, 0x03, 0x05,
    0x0F, 0x00, 0x14, 0x00, 0x64, 0x10, 0xE3, 0x00, 0x00, 0x00, 0x0A, 0x0D, 0x00, 0x00, 0x00, 0x14,
    0x94, 0x58, 0x01, 0x2D, 0x03, 0x04, 0x10, 0x00, 0x14, 0x10, 0x64, 0x56, 0x06, 0x00, 0x30, 0x03,
    0x06, 0x00, 0x02, 0x00, 0x8E, 0x18, 0x12, 0xAF, 0x00, 0x33, 0x03, 0x06, 0x00, 0x03, 0x18, 0x8E,
    0x18, 0x12, 0xA2, 0x00, 0x35, 0x03, 0x06, 0x00, 0x04, 0x30, 0x8E, 0x18, 0x12, 0xA2, 0x00, 0x37,
    0x03, 0x0B, 0x10, 0x1B, 0x48, 0x8E, 0x18, 0x12, 0xBC, 0x00, 0x39, 0x03, 0x06, 0x00, 0x05, 0x60,
    0x8E, 0x18, 0x12, 0xBC, 0x00, 0x3D, 0x03, 0x05, 0x0F, 0x00, 0x14, 0x00, 0x64, 0x10, 0xE3, 0x00,
    0x00, 0x00, 0x0C, 0x11, 0x00, 0x00, 0x00, 0x14, 0x94, 0x65, 0x01, 0x00, 0x00, 0x04, 0x10, 0x00,
    0x14, 0x10, 0x64, 0x56, 0x06, 0x00, 0x3F, 0x03, 0x06, 0x00, 0x02, 0x00, 0x8E, 0x18, 0x12, 0xA2,
    0x00, 0x42, 0x03, 0x06, 0x00, 0x03, 0x18, 0x8E, 0x18, 0x12, 0xAF, 0x00, 0x44, 0x03, 0x06, 0x00,
    0x04, 0x30, 0x8E, 0x18, 0x12, 0xA2, 0x00, 0x46, 0x03, 0x0B, 0x10, 0x1B, 0x48, 0x8E, 0x18, 0x12,
    0xBC, 0x00, 0x48, 0x03, 0x06, 0x00, 0x05, 0x60, 0x8E, 0x18, 0x12, 0xBC, 0x00, 0x4C, 0x03, 0x05,
    0x0F, 0x00, 0x00, 0x00, 0x78, 0x10, 0xE3, 0x00, 0x00, 0x00, 0x03, 0x01, 0x00, 0x00, 0x10, 0x78,
    0x36, 0x06, 0x00, 0x4E, 0x03, 0x01, 0x00, 0x00, 0x00, 0x46, 0x78, 0x48, 0x06, 0x00, 0x50, 0x03,
    0x06, 0x00, 0x02, 0x00, 0x8E, 0x18, 0x12, 0xA2, 0x00, 0x53, 0x03, 0x06, 0x00, 0x03, 0x18, 0x8E,
    0x18, 0x12, 0xA2, 0x00, 0x55, 0x03, 0x06, 0x00, 0x04, 0x30, 0x8E, 0x18, 0x12, 0xAF, 0x00, 0x57,
    0x03, 0x06, 0x00, 0x00, 0x48, 0x8E, 0x18, 0x12, 0xBC, 0x00, 0x59, 0x03, 0x06, 0x00, 0x05, 0x60,
    0x8E, 0x18, 0x12, 0xBC, 0x00, 0x5B, 0x03, 0x05, 0x0C, 0x00, 0x00, 0x00, 0x78, 0x50, 0x95, 0x00,
    0x00, 0x00, 0x06, 0x00, 0x08, 0x0A, 0x6E, 0x18, 0x14, 0x95, 0x00, 0x5D, 0x03, 0x06, 0x00, 0x09,
    0x28, 0x6E, 0x19, 0x14, 0x95, 0x00, 0x5F, 0x03, 0x06, 0x00, 0x0A, 0x43, 0x6E, 0x2C, 0x14, 0x95,
    0x00, 0x61, 0x03, 0x63, 0x03, 0x6E, 0x03, 0x73, 0x03, 0x77, 0x03, 0x7E, 0x03, 0xFF, 0xFF, 0x0B,
    0x02, 0x83, 0x03, 0x99, 0x03, 0x9B, 0x03, 0x9D, 0x03, 0x9F, 0x03, 0xA1, 0x03, 0xA3, 0x03, 0x02,
    0xA5, 0x03, 0xBB, 0x03, 0xBD, 0x03, 0xBF, 0x03, 0xC1, 0x03, 0xC3, 0x03, 0xC5, 0x03, 0xC7, 0x03,
    0x10, 0xD2, 0x03, 0x82, 0x04, 0x84, 0x04, 0x86, 0x04, 0x88, 0x04, 0x89, 0x04, 0x8B, 0x04, 0x8F,
    0x04, 0x92, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00, 0x78, 0x45, 0x06, 0x00, 0x99, 0x04, 0x4D, 0x61,
    0x69, 0x6E, 0x00, 0x4F, 0x66, 0x66, 0x00, 0x52, 0x65, 0x6D, 0x6F, 0x74, 0x65, 0x00, 0x43, 0x61,
    0x6C, 0x2E, 0x00, 0x01, 0x00, 0x00, 0x14, 0x10, 0x64, 0x56, 0x06, 0x00, 0x9C, 0x04, 0x01, 0x00,
    0x00, 0x14, 0x10, 0x64, 0x56, 0x06, 0x00, 0x9F, 0x04, 0x53, 0x00, 0x4D, 0x00, 0x4B, 0x00, 0x49,
    0x00, 0x4A, 0x00, 0x58, 0x00, 0x01, 0x00, 0x00, 0x14, 0x10, 0x64, 0x56, 0x06, 0x00, 0xA2, 0x04,
    0x01, 0x00, 0x00, 0x14, 0x10, 0x64, 0x56, 0x06, 0x00, 0xA5, 0x04, 0x53, 0x00, 0x4D, 0x00, 0x4B,
    0x00, 0x49, 0x00, 0x4A, 0x00, 0x58, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x36, 0x06, 0x00,
    0xA8, 0x04, 0x06, 0x00, 0x12, 0x00, 0x00, 0x1E, 0x12, 0xC9, 0x00, 0xAB, 0x04, 0x06, 0x00, 0x13,
    0x1E, 0x00, 0x1E, 0x12, 0xC9, 0x00, 0xAD, 0x04, 0x06, 0x00, 0x14, 0x3C, 0x00, 0x1E, 0x12, 0xC9,
    0x00, 0xAF, 0x04, 0x06, 0x00, 0x17, 0x5A, 0x00, 0x1E, 0x12, 0xD6, 0x00, 0xB1, 0x04, 0x06, 0x00,
    0x0F, 0x00, 0x12, 0x1E, 0x12, 0xC9, 0x00, 0xB3, 0x04, 0x06, 0x00, 0x10, 0x1E, 0x12, 0x1E, 0x12,
    0xC9, 0x00, 0xB5, 0x04, 0x06, 0x00, 0x11, 0x3C, 0x12, 0x1E, 0x12, 0xC9, 0x00, 0xB7, 0x04, 0x06,
    0x00, 0x18, 0x5A, 0x12, 0x1E, 0x12, 0xD6, 0x00, 0xB9, 0x04, 0x06, 0x00, 0x0C, 0x00, 0x24, 0x1E,
    0x12, 0xC9, 0x00, 0xBB, 0x04, 0x06, 0x00, 0x0D, 0x1E, 0x24, 0x1E, 0x12, 0xC9, 0x00, 0xBD, 0x04,
    0x06, 0x00, 0x0E, 0x3C, 0x24, 0x1E, 0x12, 0xC9, 0x00, 0xBF, 0x04, 0x06, 0x00, 0x19, 0x5A, 0x24,
    0x1E, 0x12, 0xD6, 0x00, 0xC1, 0x04, 0x06, 0x00, 0x0B, 0x00, 0x36, 0x1E, 0x12, 0xC9, 0x00, 0xC3,
    0x04, 0x06, 0x00, 0x15, 0x1E, 0x36, 0x1E, 0x12, 0xC9, 0x00, 0xC5, 0x04, 0x06, 0x00, 0x16, 0x3C,
    0x36, 0x1E, 0x12, 0xD6, 0x00, 0xC7, 0x04, 0x05, 0x0E, 0x1A, 0x5A, 0x36, 0x1E, 0x12, 0xC9, 0x00,
    0x00, 0x00, 0x53, 0x00, 0x4D, 0x00, 0x4B, 0x00, 0x00, 0x58, 0x00, 0x59, 0x65, 0x73, 0x00, 0x4E,
    0x6F, 0x00, 0x43, 0x61, 0x6E, 0x63, 0x65, 0x6C, 0x00, 0x08, 0xC9, 0x04, 0x03, 0x21, 0x05, 0x06,
    0x42, 0x05, 0x03, 0x84, 0x05, 0x06, 0xA5, 0x05, 0x03, 0xE7, 0x05, 0x08, 0x06, 0x0A, 0x06, 0x0C,
    0x06, 0x0E, 0x06, 0x10, 0x06, 0x12, 0x06, 0x14, 0x06, 0x16, 0x06, 0x18, 0x06, 0x1A, 0x06, 0x1C,
    0x06, 0x1E, 0x06, 0x20, 0x06, 0x22, 0x06, 0x24, 0x06, 0x09, 0x07, 0x00, 0x00, 0x00, 0x16, 0x0D,
    0x6E, 0x00, 0x26, 0x06, 0x09, 0x08, 0x00, 0x16, 0x00, 0x16, 0x0D, 0x6E, 0x00, 0x2C, 0x06, 0x09,
    0x09, 0x00, 0x2C, 0x00, 0x16, 0x0D, 0x6E, 0x00, 0x32, 0x06, 0x09, 0x0A, 0x00, 0x42, 0x00, 0x16,
    0x0D, 0x6E, 0x00, 0x38, 0x06, 0x07, 0x0B, 0x00, 0x58, 0x00, 0x20, 0x0D, 0x06, 0x00, 0x3E, 0x06,
    0x04, 0x02, 0x06, 0x00, 0x0D, 0x78, 0x24, 0x06, 0x00, 0x46, 0x06, 0x05, 0x05, 0x01, 0x00, 0x31,
    0x3C, 0x14, 0x06, 0x00, 0x00, 0x00, 0x05, 0x06, 0x01, 0x3C, 0x31, 0x3C, 0x14, 0x06, 0x00, 0x00,
    0x00, 0x05, 0x0D, 0x00, 0x00, 0x00, 0x64, 0x24, 0xF0, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x24, 0x46, 0x5A, 0xE3, 0x00, 0x49, 0x06, 0x02, 0x01, 0x00, 0x46, 0x24, 0x1E, 0x56, 0x06, 0x00,
    0x4B, 0x06, 0x05, 0x0D, 0x00, 0x00, 0x00, 0x64, 0x1E, 0xF0, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x1E, 0x46, 0x4E, 0xE3, 0x00, 0x4D, 0x06, 0x02, 0x01, 0x00, 0x46, 0x1E, 0x1E, 0x56, 0x06,
    0x00, 0x4F, 0x06, 0x06, 0x00, 0x00, 0x00, 0x6C, 0x04, 0x12, 0x06, 0x00, 0x51, 0x06, 0x06, 0x00,
    0x1C, 0x04, 0x6C, 0x30, 0x12, 0x4B, 0x01, 0x53, 0x06, 0x06, 0x00, 0x1D, 0x34, 0x6C, 0x30, 0x12,
    0x4B, 0x01, 0x55, 0x06, 0x05, 0x0D, 0x00, 0x00, 0x00, 0x64, 0x24, 0xF0, 0x00, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x00, 0x24, 0x46, 0x5A, 0xE3, 0x00, 0x57, 0x06, 0x02, 0x01, 0x00, 0x46, 0x24, 0x1E,
    0x56, 0x06, 0x00, 0x59, 0x06, 0x05, 0x0D, 0x00, 0x00, 0x00, 0x64, 0x1E, 0xF0, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x1E, 0x46, 0x4E, 0xE3, 0x00, 0x5B, 0x06, 0x02, 0x01, 0x00, 0x46, 0x1E,
    0x1E, 0x56, 0x06, 0x00, 0x5D, 0x06, 0x06, 0x00, 0x00, 0x00, 0x6C, 0x04, 0x12, 0x06, 0x00, 0x5F,
    0x06, 0x06, 0x00, 0x1C, 0x04, 0x6C, 0x30, 0x12, 0x4B, 0x01, 0x61, 0x06, 0x06, 0x00, 0x1D, 0x34,
    0x6C, 0x30, 0x12, 0x4B, 0x01, 0x63, 0x06, 0x04, 0x02, 0x00, 0x00, 0x00, 0x3C, 0x12, 0x06, 0x00,
    0x65, 0x06, 0x05, 0x05, 0x00, 0x00, 0x12, 0x3C, 0x12, 0xFD, 0x00, 0x00, 0x00, 0x05, 0x06, 0x00,
    0x00, 0x24, 0x3C, 0x12, 0xFD, 0x00, 0x00, 0x00, 0x37, 0x00, 0x38, 0x00, 0x39, 0x00, 0x3C, 0x00,
    0x34, 0x00, 0x35, 0x00, 0x36, 0x00, 0x43, 0x00, 0x31, 0x00, 0x32, 0x00, 0x33, 0x00, 0x4F, 0x00,
    0x30, 0x00, 0x2E, 0x00, 0x2D, 0x00, 0x7B, 0x00, 0x88, 0x00, 0x68, 0x06, 0x7B, 0x00, 0x88, 0x00,
    0x6C, 0x06, 0x7B, 0x00, 0x88, 0x00, 0x70, 0x06, 0x7B, 0x00, 0x88, 0x00, 0x74, 0x06, 0x7B, 0x00,
    0x78, 0x06, 0x6E, 0x00, 0x7E, 0x06, 0x02, 0x85, 0x06, 0x9B, 0x06, 0x0F, 0x07, 0x1A, 0x07, 0x67,
    0x07, 0x72, 0x07, 0x73, 0x07, 0x7B, 0x07, 0x84, 0x07, 0xC3, 0x07, 0xCE, 0x07, 0x1B, 0x08, 0x26,
    0x08, 0x27, 0x08, 0x2F, 0x08, 0x02, 0x38, 0x08, 0x4F, 0x56, 0x50, 0x00, 0x4F, 0x43, 0x50, 0x00,
    0x4F, 0x50, 0x50, 0x00, 0x4F, 0x54, 0x50, 0x00, 0x44, 0x50, 0x20, 0x6F, 0x6E, 0x00, 0x44, 0x50,
    0x20, 0x6F, 0x66, 0x66, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0D, 0x78, 0x24, 0x54, 0x00, 0x4E, 0x08,
    0x04, 0x03, 0x06, 0x00, 0x0D, 0x78, 0x24, 0x06, 0x00, 0x50, 0x08, 0x49, 0x6E, 0x74, 0x65, 0x72,
    0x61, 0x63, 0x74, 0x69, 0x76, 0x65, 0x20, 0x6D, 0x6F, 0x64, 0x65, 0x3A, 0x20, 0x4D, 0x6F, 0x76,
    0x65, 0x20, 0x75, 0x70, 0x2F, 0x64, 0x6F, 0x77, 0x6E, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x68, 0x61,
    0x6E, 0x67, 0x65, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65, 0x2E, 0x0A, 0x55, 0x73, 0x65, 0x20, 0x6C,
    0x65, 0x66, 0x74, 0x6D, 0x6F, 0x73, 0x74, 0x20, 0x70, 0x6F, 0x73, 0x2E, 0x20, 0x66, 0x6F, 0x72,
    0x20, 0x63, 0x6F, 0x61, 0x72, 0x73, 0x65, 0x20, 0x61, 0x6E, 0x64, 0x20, 0x72, 0x69, 0x67, 0x68,
    0x6D, 0x6F, 0x73, 0x74, 0x20, 0x70, 0x6F, 0x73, 0x2E, 0x20, 0x66, 0x6F, 0x72, 0x20, 0x66, 0x69,
    0x6E, 0x65, 0x20, 0x61, 0x64, 0x6A, 0x75, 0x73, 0x74, 0x6D, 0x65, 0x6E, 0x74, 0x2E, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x1E, 0x2D, 0x06, 0x00, 0x53, 0x08, 0x4E, 0x6F, 0x6E, 0x2D, 0x69, 0x6E,
    0x74, 0x65, 0x72, 0x61, 0x63, 0x74, 0x69, 0x76, 0x65, 0x20, 0x6D, 0x6F, 0x64, 0x65, 0x3A, 0x20,
    0x4D, 0x6F, 0x76, 0x65, 0x20, 0x75, 0x70, 0x2F, 0x64, 0x6F, 0x77, 0x6E, 0x20, 0x74, 0x6F, 0x20,
    0x63, 0x68, 0x61, 0x6E, 0x67, 0x65, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65, 0x2E, 0x0A, 0x53, 0x65,
    0x6C, 0x65, 0x63, 0x74, 0x20, 0x45, 0x6E, 0x74, 0x65, 0x72, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x6F,
    0x6E, 0x66, 0x69, 0x72, 0x6D, 0x2E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x27, 0x06, 0x00,
    0x56, 0x08, 0x00, 0x7E, 0x20, 0x45, 0x6E, 0x74, 0x65, 0x72, 0x00, 0x7F, 0x20, 0x43, 0x61, 0x6E,
    0x63, 0x65, 0x6C, 0x00, 0x49, 0x6E, 0x74, 0x65, 0x72, 0x61, 0x63, 0x74, 0x69, 0x76, 0x65, 0x20,
    0x6D, 0x6F, 0x64, 0x65, 0x3A, 0x20, 0x4D, 0x6F, 0x76, 0x65, 0x20, 0x75, 0x70, 0x2F, 0x64, 0x6F,
    0x77, 0x6E, 0x20, 0x74, 0x6F, 0x20, 0x63, 0x68, 0x61, 0x6E, 0x67, 0x65, 0x20, 0x76, 0x61, 0x6C,
    0x75, 0x65, 0x20, 0x73, 0x74, 0x65, 0x70, 0x20, 0x69, 0x6E, 0x20, 0x61, 0x20, 0x74, 0x69, 0x6D,
    0x65, 0x2E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x2D, 0x06, 0x00, 0x59, 0x08, 0x4E, 0x6F,
    0x6E, 0x2D, 0x69, 0x6E, 0x74, 0x65, 0x72, 0x61, 0x63, 0x74, 0x69, 0x76, 0x65, 0x20, 0x6D, 0x6F,
    0x64, 0x65, 0x3A, 0x20, 0x4D, 0x6F, 0x76, 0x65, 0x20, 0x75, 0x70, 0x2F, 0x64, 0x6F, 0x77, 0x6E,
    0x20, 0x74, 0x6F, 0x20, 0x63, 0x68, 0x61, 0x6E, 0x67, 0x65, 0x20, 0x76, 0x61, 0x6C, 0x75, 0x65,
    0x2E, 0x0A, 0x53, 0x65, 0x6C, 0x65, 0x63, 0x74, 0x20, 0x45, 0x6E, 0x74, 0x65, 0x72, 0x20, 0x74,
    0x6F, 0x20, 0x63, 0x6F, 0x6E, 0x66, 0x69, 0x72, 0x6D, 0x2E, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x27, 0x06, 0x00, 0x5C, 0x08, 0x00, 0x7E, 0x20, 0x45, 0x6E, 0x74, 0x65, 0x72, 0x00, 0x7F,
    0x20, 0x43, 0x61, 0x6E, 0x63, 0x65, 0x6C, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x12, 0xFD,
    0x00, 0x5F, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x3C, 0x12, 0x06, 0x00, 0x61, 0x08, 0x64, 0x08,
    0x02, 0x68, 0x08, 0x03, 0x7E, 0x08, 0x03, 0x9F, 0x08, 0x03, 0xC0, 0x08, 0x03, 0xE1, 0x08, 0x02,
    0x09, 0x02, 0x06, 0x09, 0x4F, 0x46, 0x46, 0x00, 0x05, 0x04, 0x06, 0x00, 0x0D, 0x78, 0x24, 0x20,
    0x00, 0x00, 0x00, 0x05, 0x04, 0x06, 0x00, 0x0D, 0x78, 0x24, 0x2D, 0x00, 0x00, 0x00, 0x04, 0x02,
    0x00, 0x00, 0x00, 0x1E, 0x0F, 0x06, 0x00, 0x1C, 0x09, 0x05, 0x05, 0x00, 0x00, 0x0F, 0x1E, 0x0F,
    0x24, 0x01, 0x00, 0x00, 0x05, 0x06, 0x00, 0x00, 0x1E, 0x1E, 0x0F, 0x24, 0x01, 0x00, 0x00, 0x04,
    0x02, 0x00, 0x00, 0x00, 0x1E, 0x0D, 0x06, 0x00, 0x1F, 0x09, 0x05, 0x05, 0x00, 0x00, 0x0D, 0x1E,
    0x0D, 0x24, 0x01, 0x00, 0x00, 0x05, 0x06, 0x00, 0x00, 0x1A, 0x1E, 0x0D, 0x24, 0x01, 0x00, 0x00,
    0x04, 0x02, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x06, 0x00, 0x22, 0x09, 0x05, 0x05, 0x00, 0x00, 0x0F,
    0x1E, 0x0F, 0x24, 0x01, 0x00, 0x00, 0x05, 0x06, 0x00, 0x00, 0x1E, 0x1E, 0x0F, 0x24, 0x01, 0x00,
    0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x1E, 0x0D, 0x06, 0x00, 0x25, 0x09, 0x05, 0x05, 0x00, 0x00,
    0x0D, 0x1E, 0x0D, 0x24, 0x01, 0x00, 0x00, 0x05, 0x06, 0x00, 0x00, 0x1A, 0x1E, 0x0D, 0x24, 0x01,
    0x00, 0x00, 0x4F, 0x46, 0x46, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x3C, 0x12, 0xFD, 0x00, 0x00,
    0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x3C, 0x12, 0x0A, 0x01, 0x00, 0x00, 0x02, 0x28, 0x09, 0x02,
    0x3E, 0x09, 0x02, 0x54, 0x09, 0x02, 0x6A, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x24,
    0x01, 0x80, 0x09, 0x04, 0x03, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x06, 0x00, 0x82, 0x09, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x1E, 0x0D, 0x24, 0x01, 0x85, 0x09, 0x04, 0x03, 0x00, 0x00, 0x00, 0x1E, 0x0D,
    0x06, 0x00, 0x87, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x24, 0x01, 0x8A, 0x09, 0x04,
    0x03, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x06, 0x00, 0x8C, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00, 0x1E,
    0x0D, 0x24, 0x01, 0x8F, 0x09, 0x04, 0x03, 0x00, 0x00, 0x00, 0x1E, 0x0D, 0x06, 0x00, 0x91, 0x09,
    0x94, 0x09, 0x02, 0x98, 0x09, 0xAE, 0x09, 0x02, 0xB2, 0x09, 0xC8, 0x09, 0x02, 0xCC, 0x09, 0xE2,
    0x09, 0x02, 0xE6, 0x09, 0x4F, 0x46, 0x46, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x24,
    0x01, 0x00, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x31, 0x01, 0x00, 0x00, 0x4F, 0x46,
    0x46, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x1E, 0x0D, 0x24, 0x01, 0x00, 0x00, 0x05, 0x04, 0x00,
    0x00, 0x00, 0x1E, 0x0D, 0x31, 0x01, 0x00, 0x00, 0x4F, 0x46, 0x46, 0x00, 0x05, 0x04, 0x00, 0x00,
    0x00, 0x1E, 0x0F, 0x24, 0x01, 0x00, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x1E, 0x0F, 0x31, 0x01,
    0x00, 0x00, 0x4F, 0x46, 0x46, 0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x1E, 0x0D, 0x24, 0x01, 0x00,
    0x00, 0x05, 0x04, 0x00, 0x00, 0x00, 0x1E, 0x0D, 0x31, 0x01, 0x00, 0x00
};

}
}
} // namespace eez::psu::gui
