/*
 * image.c
 *
 *  Created on: 28 mai 2014
 *      Author: OrpheoXlite
 */


#include "image.h"

const char image_orpheo_png[IMAGE_SIZE_ORPHEO_PNG] =
{
		0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52,
		0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x12, 0x08, 0x06, 0x00, 0x00, 0x00, 0x07, 0x5A, 0xBD,
		0x34, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xB1, 0x8F, 0x0B, 0xFC, 0x61,
		0x05, 0x00, 0x00, 0x00, 0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xAE, 0xCE, 0x1C, 0xE9, 0x00, 0x00,
		0x00, 0x20, 0x63, 0x48, 0x52, 0x4D, 0x00, 0x00, 0x7A, 0x26, 0x00, 0x00, 0x80, 0x84, 0x00, 0x00,
		0xFA, 0x00, 0x00, 0x00, 0x80, 0xE8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xEA, 0x60, 0x00, 0x00,
		0x3A, 0x98, 0x00, 0x00, 0x17, 0x70, 0x9C, 0xBA, 0x51, 0x3C, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4B,
		0x47, 0x44, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xA0, 0xBD, 0xA7, 0x93, 0x00, 0x00, 0x00, 0x09,
		0x70, 0x48, 0x59, 0x73, 0x00, 0x00, 0x0B, 0x13, 0x00, 0x00, 0x0B, 0x13, 0x01, 0x00, 0x9A, 0x9C,
		0x18, 0x00, 0x00, 0x07, 0x46, 0x49, 0x44, 0x41, 0x54, 0x58, 0xC3, 0xDD, 0x97, 0x6B, 0x4C, 0x54,
		0xE9, 0x1D, 0xC6, 0x7F, 0xE7, 0xE2, 0xDC, 0x2F, 0x20, 0x50, 0x60, 0x58, 0x0B, 0x3A, 0x0C, 0xAB,
		0x54, 0x51, 0x09, 0xDD, 0x7E, 0xF0, 0x86, 0x59, 0x93, 0x9A, 0x18, 0x1A, 0x13, 0x3F, 0xD9, 0xCA,
		0xA7, 0xA5, 0xDE, 0x02, 0x69, 0xA8, 0xA9, 0xF7, 0xA8, 0x18, 0x1B, 0xBA, 0x89, 0x31, 0xD1, 0xD8,
		0x20, 0xA6, 0xC6, 0xC8, 0x46, 0xA2, 0xBB, 0xD5, 0x44, 0x83, 0xAD, 0xB1, 0xDA, 0x05, 0xEC, 0x9A,
		0x54, 0x5B, 0x2F, 0xD1, 0x2A, 0x46, 0x41, 0x01, 0xCF, 0x14, 0x04, 0x05, 0x86, 0x71, 0x06, 0xCE,
		0xCC, 0x39, 0x6F, 0x3F, 0x0C, 0xCC, 0xE2, 0x6A, 0xAD, 0x36, 0x6E, 0x36, 0xED, 0x93, 0x9C, 0xBC,
		0x6F, 0x4E, 0xDE, 0xDB, 0xF3, 0x7F, 0xFE, 0x97, 0xF7, 0x95, 0x4C, 0xD3, 0x10, 0x08, 0x41, 0x02,
		0x12, 0x6F, 0x87, 0xC4, 0x14, 0x49, 0x02, 0x64, 0x19, 0xE9, 0xAD, 0xE7, 0x7D, 0xF7, 0x50, 0x8D,
		0x68, 0x94, 0xF6, 0xDF, 0x1D, 0x61, 0xA4, 0xB7, 0x37, 0x41, 0xE0, 0x25, 0x5A, 0x63, 0x66, 0x10,
		0x5F, 0xF7, 0x01, 0x84, 0x10, 0x20, 0x04, 0x4E, 0xBF, 0x9F, 0x69, 0xAB, 0x56, 0xA1, 0xD8, 0x6C,
		0xDF, 0x35, 0x8F, 0xB7, 0x27, 0x6C, 0xC6, 0xE2, 0xF4, 0xFD, 0xE5, 0x2B, 0x86, 0xDB, 0x1F, 0x26,
		0x75, 0x12, 0x8C, 0xA9, 0x27, 0x18, 0x13, 0x5D, 0x42, 0xE8, 0x3A, 0x86, 0xAE, 0xA3, 0x7A, 0xDC,
		0x38, 0x72, 0x72, 0x70, 0xFB, 0xF3, 0xB1, 0xA4, 0x78, 0x91, 0xE4, 0xFF, 0x1D, 0x75, 0x01, 0x54,
		0x49, 0x02, 0x49, 0x51, 0x90, 0x15, 0x05, 0x24, 0x29, 0xE1, 0x9E, 0x42, 0x20, 0x84, 0x40, 0xB2,
		0xA8, 0x58, 0xD2, 0xD2, 0xF0, 0xE4, 0xE7, 0xE3, 0x2D, 0x2A, 0x22, 0x75, 0x76, 0x11, 0xEE, 0x40,
		0x01, 0x8A, 0xCD, 0x86, 0xA4, 0xAA, 0x63, 0xE3, 0xBF, 0x1D, 0xE8, 0xBA, 0x4E, 0x2C, 0x16, 0xC3,
		0xE1, 0x70, 0x20, 0x49, 0xEF, 0x6F, 0x17, 0xF5, 0xEB, 0xAE, 0x40, 0x18, 0x26, 0xB2, 0xC3, 0x8E,
		0xDD, 0xE7, 0x23, 0xED, 0x87, 0x1F, 0xE1, 0xF6, 0xE7, 0xA3, 0xA6, 0x78, 0x51, 0x9D, 0x2E, 0x14,
		0xAB, 0x05, 0xE7, 0x94, 0x0F, 0x90, 0x2D, 0x56, 0x62, 0xE1, 0x30, 0x12, 0xA0, 0xBA, 0xDD, 0x09,
		0xE2, 0xDF, 0x02, 0x9A, 0x9A, 0x9A, 0x68, 0x69, 0x69, 0xA1, 0xA6, 0xA6, 0x86, 0x94, 0x94, 0x94,
		0xF7, 0x4B, 0x58, 0x18, 0x06, 0x8A, 0xCD, 0x4E, 0xD6, 0xD2, 0xA5, 0x4C, 0x2E, 0x9E, 0x0B, 0x48,
		0x0C, 0xDC, 0xBE, 0xCD, 0xD0, 0x83, 0x07, 0x78, 0xA6, 0xE6, 0xA1, 0x58, 0x2D, 0xE8, 0xE1, 0x61,
		0x9E, 0xFD, 0xED, 0xEF, 0x18, 0x2F, 0xC2, 0x38, 0xBE, 0x9F, 0x8B, 0x77, 0xFA, 0x74, 0x1C, 0x4E,
		0x07, 0x32, 0x6F, 0x4F, 0x58, 0x08, 0xF1, 0xD6, 0x6A, 0x69, 0x9A, 0xC6, 0xAD, 0x5B, 0xB7, 0x88,
		0xC5, 0x62, 0xEF, 0xD5, 0x90, 0xAA, 0x34, 0x69, 0x12, 0xFE, 0x9F, 0x7F, 0x82, 0x30, 0x4D, 0xFA,
		0x5A, 0x5B, 0xE9, 0xFA, 0xFC, 0x0B, 0x5C, 0x85, 0x85, 0xA4, 0x15, 0xCF, 0x25, 0xFA, 0xE4, 0x09,
		0x7D, 0x57, 0xAE, 0x10, 0x1F, 0xD5, 0x71, 0x07, 0xF2, 0x49, 0x99, 0x35, 0x13, 0x4C, 0xC1, 0x8B,
		0x47, 0x8F, 0x78, 0xD8, 0xDA, 0x8C, 0xE2, 0x74, 0xF2, 0x83, 0x4D, 0x9B, 0x51, 0x9D, 0xCE, 0xE4,
		0x82, 0xB1, 0x58, 0x8C, 0x70, 0x38, 0x8C, 0xAA, 0xAA, 0x38, 0x1C, 0x0E, 0x14, 0x45, 0x01, 0x20,
		0x1E, 0x8F, 0x27, 0x09, 0xBF, 0x78, 0xF1, 0x02, 0xA7, 0xD3, 0x89, 0x69, 0x9A, 0x28, 0x8A, 0x82,
		0x61, 0x18, 0x44, 0x22, 0x11, 0xAC, 0x56, 0x2B, 0x36, 0x9B, 0xED, 0x15, 0xA3, 0x44, 0xA3, 0x51,
		0x74, 0x5D, 0xC7, 0xE5, 0x72, 0xA1, 0x28, 0x4A, 0x22, 0x69, 0x92, 0x70, 0xFB, 0x48, 0x24, 0x82,
		0xDD, 0x6E, 0xC7, 0x36, 0x21, 0x71, 0x9A, 0xA6, 0x49, 0x24, 0x12, 0x21, 0x1E, 0x8F, 0x63, 0xB3,
		0xD9, 0xB0, 0x5A, 0xAD, 0xC9, 0x35, 0x55, 0x84, 0xA0, 0xEF, 0xAB, 0x2B, 0x84, 0xEE, 0xDE, 0x23,
		0xEB, 0xC7, 0x4B, 0x48, 0x9D, 0x5B, 0xCC, 0xF3, 0xAB, 0xD7, 0x78, 0x70, 0xF0, 0xB7, 0xB8, 0xFD,
		0xF9, 0xA4, 0xFD, 0xE8, 0x23, 0x84, 0x29, 0x18, 0xB8, 0x75, 0x93, 0x07, 0xCD, 0x5F, 0xA2, 0x0F,
		0x0C, 0x61, 0x8C, 0x8E, 0x82, 0x69, 0x92, 0x3A, 0xBB, 0x88, 0xF1, 0x92, 0x26, 0x84, 0xE0, 0xFA,
		0xF5, 0xEB, 0x1C, 0x39, 0x72, 0x84, 0x60, 0x30, 0x88, 0xAA, 0xAA, 0x94, 0x94, 0x94, 0x50, 0x59,
		0x59, 0x89, 0xDD, 0x6E, 0xA7, 0xAE, 0xAE, 0x2E, 0x49, 0xFA, 0xE2, 0xC5, 0x8B, 0x6C, 0xDD, 0xBA,
		0x95, 0x53, 0xA7, 0x4E, 0x31, 0x6F, 0xDE, 0x3C, 0x5A, 0x5A, 0x5A, 0x68, 0x6B, 0x6B, 0xC3, 0xE3,
		0xF1, 0xB0, 0x7B, 0xF7, 0x6E, 0x66, 0xCC, 0x98, 0x91, 0x5C, 0xB3, 0xB9, 0xB9, 0x99, 0x33, 0x67,
		0xCE, 0xA0, 0x69, 0x1A, 0x2B, 0x56, 0xAC, 0xA0, 0xA2, 0xA2, 0x02, 0x8B, 0xC5, 0xC2, 0x95, 0x2B,
		0x57, 0x38, 0x7A, 0xF4, 0x28, 0xFD, 0xFD, 0xFD, 0xF8, 0x7C, 0x3E, 0x2A, 0x2B, 0x2B, 0x29, 0x2C,
		0x2C, 0x64, 0x78, 0x78, 0x98, 0x93, 0x27, 0x4F, 0x72, 0xFE, 0xFC, 0x79, 0x0C, 0xC3, 0xC0, 0xEB,
		0xF5, 0x52, 0x59, 0x59, 0x49, 0x71, 0x71, 0x31, 0x8A, 0xA2, 0x20, 0x0B, 0x21, 0x48, 0x99, 0x33,
		0x9B, 0x0F, 0xAB, 0x7F, 0xC1, 0xD0, 0xAD, 0xDB, 0xB4, 0xED, 0xDB, 0x87, 0x69, 0xC4, 0x28, 0xA8,
		0xAC, 0xC4, 0xE9, 0x9F, 0x46, 0xD7, 0x17, 0xBF, 0xE7, 0xDE, 0xA7, 0x9F, 0xF2, 0xF4, 0xE2, 0x25,
		0xA2, 0x3D, 0xBD, 0x18, 0x23, 0x51, 0x10, 0x06, 0x92, 0x22, 0x25, 0xCB, 0xB6, 0x10, 0x02, 0x4D,
		0xD3, 0xD8, 0xB0, 0x61, 0x03, 0xD1, 0x68, 0x94, 0x5D, 0xBB, 0x76, 0xB1, 0x6A, 0xD5, 0x2A, 0x8E,
		0x1F, 0x3F, 0xCE, 0xE1, 0xC3, 0x87, 0x19, 0x1D, 0x1D, 0xE5, 0xC6, 0x8D, 0x1B, 0xD4, 0xD7, 0xD7,
		0xD3, 0xD5, 0xD5, 0xC5, 0xEA, 0xD5, 0xAB, 0xF1, 0x78, 0x3C, 0x5C, 0xBA, 0x74, 0x89, 0x03, 0x07,
		0x0E, 0x50, 0x50, 0x50, 0xC0, 0xDA, 0xB5, 0x6B, 0x89, 0x44, 0x22, 0x6C, 0xDC, 0xB8, 0x91, 0x67,
		0xCF, 0x9E, 0x01, 0xF0, 0xF8, 0xF1, 0x63, 0x4E, 0x9C, 0x38, 0xC1, 0xD2, 0xA5, 0x4B, 0x59, 0xB0,
		0x60, 0x01, 0x75, 0x75, 0x75, 0xB4, 0xB5, 0xB5, 0xD1, 0xD1, 0xD1, 0xC1, 0xF6, 0xED, 0xDB, 0x09,
		0x04, 0x02, 0xD4, 0xD4, 0xD4, 0x00, 0xB0, 0x6D, 0xDB, 0x36, 0x74, 0x5D, 0xA7, 0xB1, 0xB1, 0x91,
		0xDA, 0xDA, 0x5A, 0xCA, 0xCB, 0xCB, 0xD9, 0xB1, 0x63, 0x07, 0xD9, 0xD9, 0xD9, 0xAC, 0x59, 0xB3,
		0x86, 0xF6, 0xF6, 0xF6, 0x84, 0xC2, 0x8A, 0xD5, 0x86, 0x88, 0x1B, 0xDC, 0xFC, 0xE5, 0xAF, 0x70,
		0x4D, 0xFF, 0x90, 0x39, 0xB5, 0xBF, 0x21, 0xFC, 0xB8, 0x93, 0x7F, 0xFC, 0x7A, 0x0F, 0x7A, 0x28,
		0x84, 0xAC, 0xA8, 0x48, 0x56, 0xEB, 0x38, 0xB3, 0x31, 0xA7, 0x91, 0x92, 0x9F, 0x18, 0xAB, 0xD0,
		0x17, 0x2E, 0x5C, 0x60, 0x70, 0x70, 0x90, 0xED, 0xDB, 0xB7, 0xE3, 0xF7, 0xFB, 0x99, 0x35, 0x6B,
		0x16, 0x4F, 0x9F, 0x3E, 0xE5, 0xD0, 0xA1, 0x43, 0x2C, 0x5F, 0xBE, 0x1C, 0x49, 0x92, 0x28, 0x2C,
		0x2C, 0x64, 0xE7, 0xCE, 0x9D, 0xA4, 0xA6, 0xA6, 0x12, 0x0C, 0x06, 0x51, 0x14, 0x85, 0x95, 0x2B,
		0x57, 0xB2, 0x6E, 0xDD, 0x3A, 0x84, 0x10, 0xF8, 0x7C, 0x3E, 0xCA, 0xCA, 0xCA, 0xB8, 0x7F, 0xFF,
		0x3E, 0x00, 0x0E, 0x87, 0x83, 0x3D, 0x7B, 0xF6, 0x30, 0x63, 0xC6, 0x0C, 0xEE, 0xDE, 0xBD, 0x4B,
		0x53, 0x53, 0x13, 0xFD, 0xFD, 0xFD, 0xB4, 0xB4, 0xB4, 0x30, 0x32, 0x32, 0x42, 0x69, 0x69, 0x29,
		0x56, 0xAB, 0x95, 0xC5, 0x8B, 0x17, 0x53, 0x5D, 0x5D, 0xCD, 0xC3, 0x87, 0x0F, 0x69, 0x6C, 0x6C,
		0xA4, 0xBC, 0xBC, 0x9C, 0xE5, 0xCB, 0x97, 0x03, 0x10, 0x08, 0x04, 0x38, 0x7D, 0xFA, 0x34, 0xE7,
		0xCE, 0x9D, 0x23, 0x10, 0x08, 0xA0, 0x1A, 0x91, 0x17, 0x04, 0xCF, 0x9E, 0x25, 0xE3, 0xE3, 0x45,
		0xE4, 0xFD, 0xB4, 0x9C, 0x47, 0x9F, 0x7D, 0xC6, 0xD3, 0x2F, 0xFF, 0x0C, 0xA6, 0x89, 0xAC, 0xA8,
		0x49, 0x7A, 0x6F, 0x82, 0x24, 0x49, 0x74, 0x77, 0x77, 0xE3, 0x72, 0xB9, 0xC8, 0xC8, 0xC8, 0x40,
		0x08, 0x81, 0xA2, 0x28, 0x04, 0x02, 0x01, 0xFA, 0xFA, 0xFA, 0x18, 0x18, 0x18, 0x40, 0x08, 0x41,
		0x66, 0x66, 0x66, 0x32, 0xD6, 0x84, 0x10, 0xC8, 0xB2, 0x8C, 0xC7, 0xE3, 0x49, 0xC6, 0xF6, 0x94,
		0x29, 0x53, 0x92, 0x39, 0x00, 0x20, 0x33, 0x33, 0x93, 0xF4, 0xF4, 0xF4, 0xE4, 0x1E, 0xE3, 0x71,
		0xD8, 0xD1, 0xD1, 0x41, 0x57, 0x57, 0x17, 0xB5, 0xB5, 0xB5, 0xC8, 0xB2, 0x8C, 0xAE, 0xEB, 0xF8,
		0x7C, 0x3E, 0x7A, 0x7A, 0x7A, 0x18, 0x18, 0x18, 0x20, 0x37, 0x37, 0x37, 0x79, 0x36, 0x87, 0xC3,
		0x41, 0x4E, 0x4E, 0x0E, 0xDD, 0xDD, 0xDD, 0x08, 0x21, 0x50, 0x85, 0x29, 0xF8, 0xDE, 0xC7, 0x4B,
		0xF8, 0xE0, 0x27, 0x65, 0xDC, 0xDB, 0xBB, 0x97, 0xA7, 0x17, 0xFF, 0x04, 0xB2, 0x0C, 0xEF, 0x50,
		0xFB, 0x84, 0x10, 0xB8, 0xDD, 0x6E, 0x74, 0x5D, 0x27, 0x1A, 0x8D, 0x26, 0x49, 0x0C, 0x0E, 0x0E,
		0x62, 0xB7, 0xDB, 0xB1, 0xDB, 0xED, 0x2F, 0x19, 0x67, 0x62, 0x3B, 0xDE, 0x1F, 0x1F, 0x6F, 0xB1,
		0x58, 0x70, 0xBB, 0xDD, 0x6F, 0x1C, 0x9F, 0x96, 0x96, 0x86, 0xDF, 0xEF, 0x67, 0xFF, 0xFE, 0xFD,
		0x78, 0x3C, 0x1E, 0x24, 0x49, 0xC2, 0x34, 0x4D, 0xA2, 0xD1, 0x28, 0x56, 0xAB, 0x35, 0x69, 0xB0,
		0xF1, 0x64, 0x19, 0x0E, 0x87, 0x71, 0xBB, 0xDD, 0x48, 0x92, 0x84, 0xAC, 0x7A, 0x3C, 0x4C, 0x2D,
		0xFF, 0x19, 0xCF, 0xAF, 0x5D, 0xE5, 0x9F, 0x7F, 0xFC, 0x03, 0xC8, 0xCA, 0x3B, 0xA5, 0x79, 0x89,
		0x84, 0xE5, 0x4B, 0x4B, 0x4B, 0x19, 0x1D, 0x1D, 0xE5, 0xD4, 0xA9, 0x53, 0xC4, 0xE3, 0x71, 0x34,
		0x4D, 0xA3, 0xA1, 0xA1, 0x81, 0x85, 0x0B, 0x17, 0xE2, 0xF3, 0xF9, 0x5E, 0x5B, 0x8E, 0x74, 0x5D,
		0xE7, 0xDA, 0xB5, 0x6B, 0x84, 0x42, 0x21, 0x86, 0x87, 0x87, 0x39, 0x76, 0xEC, 0x18, 0xC5, 0xC5,
		0xC5, 0x14, 0x14, 0x14, 0xBC, 0x64, 0xCC, 0x89, 0x2D, 0xC0, 0xB2, 0x65, 0xCB, 0x08, 0x87, 0xC3,
		0x9C, 0x3D, 0x7B, 0x16, 0xBB, 0xDD, 0x8E, 0xCB, 0xE5, 0x22, 0x12, 0x89, 0xE0, 0xF5, 0x7A, 0x59,
		0xB4, 0x68, 0x11, 0x27, 0x4E, 0x9C, 0x40, 0xD3, 0x34, 0x74, 0x5D, 0xE7, 0xC2, 0x85, 0x0B, 0xF4,
		0xF4, 0xF4, 0xB0, 0x64, 0xC9, 0x92, 0x44, 0x0C, 0x4B, 0x12, 0x08, 0x53, 0xF0, 0xFC, 0xC6, 0x4D,
		0x24, 0x45, 0xE5, 0xE5, 0x5B, 0xF3, 0x1B, 0x75, 0x7D, 0xE9, 0x20, 0x45, 0x45, 0x45, 0x54, 0x55,
		0x55, 0xD1, 0xD0, 0xD0, 0xC0, 0xE5, 0xCB, 0x97, 0x09, 0x85, 0x42, 0x98, 0xA6, 0xC9, 0x96, 0x2D,
		0x5B, 0x70, 0x3A, 0x9D, 0xA4, 0xA4, 0xA4, 0x60, 0xB1, 0x58, 0x92, 0xC4, 0xC7, 0xDD, 0xBE, 0xBB,
		0xBB, 0x9B, 0x8A, 0x8A, 0x0A, 0x0C, 0xC3, 0xA0, 0xB7, 0xB7, 0x97, 0xBD, 0x7B, 0xF7, 0x32, 0x79,
		0xF2, 0x64, 0x9C, 0x4E, 0x27, 0xE9, 0xE9, 0xE9, 0xC8, 0xB2, 0x9C, 0x38, 0xA8, 0xAA, 0x92, 0x91,
		0x91, 0x81, 0xC5, 0x62, 0x61, 0xF6, 0xEC, 0xD9, 0x6C, 0xDA, 0xB4, 0x89, 0xFA, 0xFA, 0x7A, 0x5A,
		0x5B, 0x5B, 0xB1, 0x5A, 0xAD, 0x8C, 0x8C, 0x8C, 0x70, 0xF0, 0xE0, 0x41, 0xD6, 0xAF, 0x5F, 0x4F,
		0x30, 0x18, 0xA4, 0xA2, 0xA2, 0x02, 0xAF, 0xD7, 0x8B, 0xA6, 0x69, 0x6C, 0xDE, 0xBC, 0x99, 0x92,
		0x92, 0x92, 0x44, 0x58, 0x98, 0xA6, 0x29, 0x00, 0x9E, 0x5D, 0xFD, 0x2B, 0x37, 0xAA, 0xAB, 0x31,
		0x46, 0x75, 0x64, 0x55, 0x7D, 0x99, 0xF6, 0x6B, 0x1E, 0x15, 0x12, 0x32, 0xA9, 0x45, 0x33, 0x99,
		0xBB, 0x6F, 0x1F, 0x93, 0x5C, 0x09, 0x17, 0x8C, 0xC5, 0x62, 0x74, 0x75, 0x75, 0xD1, 0xD9, 0xD9,
		0x89, 0xDD, 0x6E, 0x27, 0x10, 0x08, 0x90, 0x9E, 0x9E, 0x8E, 0x10, 0x82, 0x27, 0x4F, 0x9E, 0x00,
		0x90, 0x93, 0x93, 0x83, 0x2C, 0xCB, 0x04, 0x83, 0x41, 0xCA, 0xCA, 0xCA, 0xA8, 0xAA, 0xAA, 0x62,
		0xE6, 0xCC, 0x99, 0x84, 0xC3, 0x61, 0x72, 0x73, 0x73, 0xC9, 0xCB, 0xCB, 0x03, 0xA0, 0xBF, 0xBF,
		0x9F, 0xA1, 0xA1, 0x21, 0xF2, 0xF2, 0xF2, 0x50, 0x55, 0x95, 0x91, 0x91, 0x11, 0x3A, 0x3B, 0x3B,
		0xC9, 0xCE, 0xCE, 0xC6, 0xE3, 0xF1, 0x60, 0x18, 0x06, 0x9A, 0xA6, 0xD1, 0xDE, 0xDE, 0x8E, 0xA2,
		0x28, 0x64, 0x65, 0x65, 0x91, 0x9F, 0x9F, 0x8F, 0x24, 0x49, 0x84, 0xC3, 0x61, 0xEE, 0xDC, 0xB9,
		0x43, 0x34, 0x1A, 0x25, 0x2B, 0x2B, 0x8B, 0x82, 0x82, 0x02, 0xD4, 0xB1, 0x1B, 0xA1, 0x64, 0x98,
		0xA6, 0x90, 0xC6, 0xDE, 0x7A, 0xA3, 0x7D, 0x7D, 0xF4, 0x36, 0xB7, 0x10, 0x6A, 0xBB, 0x47, 0x6C,
		0x60, 0x30, 0x99, 0x81, 0x5F, 0x75, 0xE3, 0x44, 0x88, 0x3B, 0xA6, 0x4E, 0x23, 0xBF, 0xE2, 0x13,
		0x14, 0xBB, 0x23, 0xA9, 0xDA, 0x44, 0x05, 0x27, 0xC6, 0xDE, 0x44, 0x97, 0x94, 0x24, 0x09, 0x4D,
		0xD3, 0x28, 0x2B, 0x2B, 0xA3, 0xBA, 0xBA, 0x9A, 0xF2, 0xF2, 0xF2, 0x57, 0x5C, 0xF8, 0x9B, 0x21,
		0x30, 0xF1, 0xFF, 0xF8, 0x3E, 0xAF, 0x6B, 0xFF, 0xDD, 0x98, 0xE4, 0xDE, 0x62, 0xE2, 0x49, 0xC6,
		0xDE, 0xB9, 0x98, 0x06, 0x66, 0x2C, 0xFE, 0x9F, 0xE3, 0x57, 0x96, 0x91, 0x27, 0x4D, 0x7A, 0xA7,
		0x04, 0x37, 0x8E, 0x50, 0x28, 0x44, 0x43, 0x43, 0x03, 0xF3, 0xE7, 0xCF, 0x67, 0xCE, 0x9C, 0x39,
		0xEF, 0x3C, 0xFF, 0xBF, 0xC5, 0x37, 0x08, 0xFF, 0xFF, 0xE3, 0x5F, 0xAA, 0xA8, 0x3D, 0xA7, 0x8E,
		0x2B, 0xEE, 0x6D, 0x00, 0x00, 0x00, 0x62, 0x74, 0x45, 0x58, 0x74, 0x63, 0x6F, 0x6D, 0x6D, 0x65,
		0x6E, 0x74, 0x00, 0x62, 0x6F, 0x72, 0x64, 0x65, 0x72, 0x20, 0x62, 0x73, 0x3A, 0x30, 0x20, 0x62,
		0x63, 0x3A, 0x23, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x70, 0x73, 0x3A, 0x30, 0x20, 0x70,
		0x63, 0x3A, 0x23, 0x65, 0x65, 0x65, 0x65, 0x65, 0x65, 0x20, 0x65, 0x73, 0x3A, 0x30, 0x20, 0x65,
		0x63, 0x3A, 0x23, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x20, 0x63, 0x6B, 0x3A, 0x35, 0x30, 0x30,
		0x64, 0x30, 0x32, 0x61, 0x34, 0x66, 0x31, 0x66, 0x31, 0x64, 0x37, 0x34, 0x39, 0x37, 0x33, 0x34,
		0x30, 0x63, 0x63, 0x35, 0x38, 0x36, 0x38, 0x39, 0x36, 0x62, 0x66, 0x31, 0x31, 0x84, 0x9F, 0xD0,
		0x00, 0x00, 0x00, 0x00, 0x21, 0x74, 0x45, 0x58, 0x74, 0x43, 0x72, 0x65, 0x61, 0x74, 0x69, 0x6F,
		0x6E, 0x20, 0x54, 0x69, 0x6D, 0x65, 0x00, 0x32, 0x30, 0x30, 0x34, 0x3A, 0x30, 0x39, 0x3A, 0x32,
		0x33, 0x20, 0x31, 0x31, 0x3A, 0x31, 0x39, 0x3A, 0x34, 0x39, 0x66, 0xD9, 0xEE, 0x5F, 0x00, 0x00,
		0x00, 0x25, 0x74, 0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3A, 0x63, 0x72, 0x65, 0x61, 0x74,
		0x65, 0x00, 0x32, 0x30, 0x31, 0x34, 0x2D, 0x30, 0x35, 0x2D, 0x32, 0x37, 0x54, 0x31, 0x33, 0x3A,
		0x35, 0x38, 0x3A, 0x31, 0x34, 0x2B, 0x30, 0x30, 0x3A, 0x30, 0x30, 0x4B, 0x37, 0x6C, 0x44, 0x00,
		0x00, 0x00, 0x25, 0x74, 0x45, 0x58, 0x74, 0x64, 0x61, 0x74, 0x65, 0x3A, 0x6D, 0x6F, 0x64, 0x69,
		0x66, 0x79, 0x00, 0x32, 0x30, 0x31, 0x34, 0x2D, 0x30, 0x35, 0x2D, 0x32, 0x37, 0x54, 0x31, 0x33,
		0x3A, 0x35, 0x38, 0x3A, 0x31, 0x34, 0x2B, 0x30, 0x30, 0x3A, 0x30, 0x30, 0x3A, 0x6A, 0xD4, 0xF8,
		0x00, 0x00, 0x00, 0x26, 0x74, 0x45, 0x58, 0x74, 0x53, 0x6F, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65,
		0x00, 0x41, 0x64, 0x6F, 0x62, 0x65, 0x20, 0x50, 0x68, 0x6F, 0x74, 0x6F, 0x73, 0x68, 0x6F, 0x70,
		0x20, 0x43, 0x53, 0x36, 0x20, 0x28, 0x57, 0x69, 0x6E, 0x64, 0x6F, 0x77, 0x73, 0x29, 0x80, 0x15,
		0xF9, 0xEB, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

const char image_favicon_ico[IMAGE_SIZE_FAVICON_ICO] =
{
		0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x68, 0x04,
		0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00,
		0x00, 0x00, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF9, 0xFD,
		0xFB, 0x33, 0xE4, 0xE3, 0xF5, 0x33, 0x80, 0x7E, 0xD2, 0x33, 0x21, 0x20, 0xB3, 0x33, 0x1B, 0x16,
		0xAB, 0x33, 0x2F, 0x2F, 0xB5, 0x33, 0x30, 0x33, 0xB3, 0x33, 0x30, 0x32, 0xB5, 0x33, 0x30, 0x32,
		0xB6, 0x33, 0x30, 0x32, 0xB6, 0x33, 0x30, 0x32, 0xB6, 0x33, 0x30, 0x32, 0xB6, 0x33, 0x30, 0x32,
		0xB6, 0x33, 0x2F, 0x30, 0xB8, 0x33, 0x30, 0x30, 0xB9, 0x33, 0x3E, 0x3E, 0xBF, 0x33, 0xE7, 0xE9,
		0xF5, 0xFF, 0x93, 0x91, 0xD7, 0xFF, 0x63, 0x61, 0xC8, 0xFF, 0x2A, 0x29, 0xB4, 0xFF, 0x26, 0x22,
		0xB0, 0xFF, 0x31, 0x31, 0xB6, 0xFF, 0x31, 0x34, 0xB4, 0xFF, 0x31, 0x33, 0xB6, 0xFF, 0x31, 0x33,
		0xB6, 0xFF, 0x31, 0x33, 0xB6, 0xFF, 0x31, 0x33, 0xB6, 0xFF, 0x31, 0x33, 0xB6, 0xFF, 0x31, 0x33,
		0xB6, 0xFF, 0x32, 0x32, 0xB8, 0xFF, 0x32, 0x32, 0xB9, 0xFF, 0x40, 0x41, 0xBE, 0xFF, 0xD0, 0xCF,
		0xEE, 0xFF, 0x24, 0x22, 0xB0, 0xFF, 0x38, 0x35, 0xB8, 0xFF, 0x35, 0x35, 0xB8, 0xFF, 0x35, 0x35,
		0xB8, 0xFF, 0x35, 0x36, 0xB9, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x35, 0x35, 0xB8, 0xFF, 0x35, 0x36,
		0xB8, 0xFF, 0x35, 0x36, 0xB8, 0xFF, 0x35, 0x36, 0xB8, 0xFF, 0x35, 0x36, 0xB8, 0xFF, 0x35, 0x36,
		0xB8, 0xFF, 0x37, 0x36, 0xB6, 0xFF, 0x35, 0x35, 0xB7, 0xFF, 0x42, 0x42, 0xBC, 0xFF, 0xD1, 0xD4,
		0xF0, 0xFF, 0x38, 0x39, 0xB8, 0xFF, 0x39, 0x38, 0xB8, 0xFF, 0x36, 0x35, 0xB8, 0xFF, 0x34, 0x34,
		0xB9, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x35, 0xB8, 0xFF, 0x36, 0x36,
		0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36,
		0xB8, 0xFF, 0x37, 0x36, 0xB7, 0xFF, 0x36, 0x36, 0xB7, 0xFF, 0x42, 0x41, 0xBC, 0xFF, 0xF3, 0xF4,
		0xFB, 0xFF, 0x85, 0x85, 0xD7, 0xFF, 0x30, 0x30, 0xB6, 0xFF, 0x35, 0x35, 0xB8, 0xFF, 0x3A, 0x35,
		0xBA, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36,
		0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36,
		0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB7, 0xFF, 0x42, 0x42, 0xBB, 0xFF, 0xA5, 0xA5,
		0xE0, 0xFF, 0x89, 0x88, 0xD4, 0xFF, 0x77, 0x78, 0xD0, 0xFF, 0x84, 0x85, 0xD3, 0xFF, 0x4D, 0x4A,
		0xBF, 0xFF, 0x37, 0x38, 0xB7, 0xFF, 0x35, 0x36, 0xB5, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x35, 0x35,
		0xB7, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x35, 0x35, 0xB7, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36,
		0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB7, 0xFF, 0x42, 0x42, 0xBB, 0xFF, 0x62, 0x62,
		0xC6, 0xFF, 0x46, 0x43, 0xBD, 0xFF, 0x7A, 0x79, 0xCE, 0xFF, 0xE4, 0xE4, 0xF5, 0xFF, 0xB6, 0xB3,
		0xE6, 0xFF, 0x4E, 0x51, 0xBE, 0xFF, 0x33, 0x34, 0xB4, 0xFF, 0x36, 0x37, 0xB6, 0xFF, 0x35, 0x35,
		0xB8, 0xFF, 0x35, 0x35, 0xB7, 0xFF, 0x35, 0x35, 0xB7, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36,
		0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB7, 0xFF, 0x42, 0x42, 0xBB, 0xFF, 0xC1, 0xC1,
		0xE8, 0xFF, 0x1D, 0x1F, 0xB1, 0xFF, 0x25, 0x26, 0xB4, 0xFF, 0x6C, 0x6F, 0xC9, 0xFF, 0xD7, 0xDA,
		0xF1, 0xFF, 0x96, 0x95, 0xD9, 0xFF, 0x2B, 0x2A, 0xB7, 0xFF, 0x35, 0x35, 0xBC, 0xFF, 0x35, 0x37,
		0xB5, 0xFF, 0x34, 0x35, 0xB6, 0xFF, 0x36, 0x35, 0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36,
		0xB8, 0xFF, 0x36, 0x36, 0xB8, 0xFF, 0x36, 0x36, 0xB7, 0xFF, 0x42, 0x42, 0xBB, 0xFF, 0x3A, 0x3D,
		0xB9, 0xFF, 0xEC, 0xEF, 0xF7, 0xFF, 0xEB, 0xE9, 0xF6, 0xFF, 0x5F, 0x5D, 0xC8, 0xFF, 0x1A, 0x1B,
		0xAD, 0xFF, 0x4F, 0x50, 0xC3, 0xFF, 0xE0, 0xE2, 0xF2, 0xFF, 0xF0, 0xF1, 0xFA, 0xFF, 0x65, 0x67,
		0xC6, 0xFF, 0x34, 0x32, 0xB8, 0xFF, 0x2F, 0x2E, 0xB6, 0xFF, 0x36, 0x35, 0xB9, 0xFF, 0x35, 0x36,
		0xB7, 0xFF, 0x34, 0x36, 0xB6, 0xFF, 0x34, 0x35, 0xB7, 0xFF, 0x41, 0x42, 0xBB, 0xFF, 0x61, 0x5E,
		0xC8, 0xFF, 0x6F, 0x6E, 0xC9, 0xFF, 0xA0, 0xA2, 0xDA, 0xFF, 0xC9, 0xC7, 0xEC, 0xFF, 0xA3, 0x9F,
		0xE1, 0xFF, 0x5A, 0x58, 0xC6, 0xFF, 0x6F, 0x70, 0xCD, 0xFF, 0x9D, 0x9C, 0xDB, 0xFF, 0xC8, 0xCA,
		0xEC, 0xFF, 0xA8, 0xA9, 0xDE, 0xFF, 0x6F, 0x72, 0xCD, 0xFF, 0x49, 0x49, 0xBD, 0xFF, 0x2D, 0x2E,
		0xB3, 0xFF, 0x2E, 0x2D, 0xB5, 0xFF, 0x30, 0x31, 0xB6, 0xFF, 0x41, 0x42, 0xBA, 0xFF, 0x66, 0x65,
		0xC9, 0xFF, 0x13, 0x12, 0xAC, 0xFF, 0x2C, 0x2A, 0xB1, 0xFF, 0xB7, 0xB8, 0xE3, 0xFF, 0xFD, 0xFC,
		0xFE, 0xFF, 0xCF, 0xD0, 0xEE, 0xFF, 0x3C, 0x3D, 0xBB, 0xFF, 0x10, 0x0D, 0xA9, 0xFF, 0x8E, 0x8E,
		0xD6, 0xFF, 0xDC, 0xDB, 0xF1, 0xFF, 0xF4, 0xF5, 0xFB, 0xFF, 0xA7, 0xA7, 0xDD, 0xFF, 0x65, 0x67,
		0xC6, 0xFF, 0x46, 0x45, 0xBB, 0xFF, 0x61, 0x62, 0xC8, 0xFF, 0xA6, 0xA8, 0xDE, 0xFF, 0x33, 0x33,
		0xB8, 0xFF, 0x2E, 0x2B, 0xB3, 0xFF, 0x1E, 0x1F, 0xB0, 0xFF, 0x41, 0x41, 0xBB, 0xFF, 0x95, 0x95,
		0xD7, 0xFF, 0xF7, 0xF8, 0xFC, 0xFF, 0xCC, 0xCB, 0xEC, 0xFF, 0x90, 0x90, 0xDA, 0xFF, 0x59, 0x56,
		0xC4, 0xFF, 0x81, 0x7F, 0xD1, 0xFF, 0xE2, 0xE1, 0xF5, 0xFF, 0xF6, 0xF6, 0xFB, 0xFF, 0xE7, 0xE6,
		0xF6, 0xFF, 0xD0, 0xD1, 0xEE, 0xFF, 0xCA, 0xCB, 0xED, 0xFF, 0xF0, 0xEF, 0xFB, 0xFF, 0x30, 0x32,
		0xB7, 0xFF, 0x35, 0x36, 0xB8, 0xFF, 0x38, 0x38, 0xB9, 0xFF, 0x2A, 0x28, 0xB5, 0xFF, 0x2C, 0x2A,
		0xB5, 0xFF, 0x8B, 0x88, 0xD6, 0xFF, 0xD5, 0xD5, 0xF0, 0xFF, 0xD8, 0xD7, 0xF0, 0xFF, 0xC5, 0xC4,
		0xEB, 0xFF, 0xA0, 0xA0, 0xDB, 0xFF, 0xB0, 0xAF, 0xE3, 0xFF, 0xF5, 0xF5, 0xFC, 0xFF, 0xFF, 0xFC,
		0xFD, 0xFF, 0xFF, 0xFE, 0xFA, 0xFF, 0xFF, 0xFE, 0xFE, 0xFF, 0xFE, 0xFE, 0xFE, 0xFF, 0x32, 0x33,
		0xB8, 0xFF, 0x35, 0x35, 0xB8, 0xFF, 0x36, 0x35, 0xBA, 0xFF, 0x36, 0x35, 0xB8, 0xFF, 0x36, 0x34,
		0xB6, 0xFF, 0x37, 0x36, 0xB9, 0xFF, 0x38, 0x37, 0xBA, 0xFF, 0x3A, 0x38, 0xBA, 0xFF, 0x29, 0x28,
		0xB3, 0xFF, 0x22, 0x20, 0xB2, 0xFF, 0x4B, 0x4A, 0xC0, 0xFF, 0xAA, 0xA9, 0xE1, 0xFF, 0xD1, 0xD1,
		0xEE, 0xFF, 0xEA, 0xEB, 0xF7, 0xFF, 0xF9, 0xFA, 0xFB, 0xFF, 0xFE, 0xFD, 0xFE, 0xFF, 0x25, 0x24,
		0xB2, 0xFF, 0x27, 0x26, 0xB1, 0xFF, 0x26, 0x26, 0xB2, 0xFF, 0x26, 0x26, 0xB1, 0xFF, 0x26, 0x26,
		0xB1, 0xFF, 0x27, 0x27, 0xB1, 0xFF, 0x27, 0x26, 0xB3, 0xFF, 0x24, 0x25, 0xB2, 0xFF, 0x29, 0x28,
		0xB1, 0xFF, 0x24, 0x23, 0xB1, 0xFF, 0x1A, 0x19, 0xAE, 0xFF, 0x2E, 0x2E, 0xB5, 0xFF, 0x48, 0x4A,
		0xBF, 0xFF, 0x62, 0x63, 0xC7, 0xFF, 0x71, 0x71, 0xCC, 0xFF, 0x83, 0x82, 0xD2, 0xFF, 0x1C, 0x1A,
		0xAF, 0x33, 0x1D, 0x1C, 0xAE, 0x33, 0x1D, 0x1C, 0xAE, 0x33, 0x1D, 0x1C, 0xAE, 0x33, 0x1D, 0x1C,
		0xAE, 0x33, 0x1D, 0x1C, 0xAE, 0x33, 0x1D, 0x1C, 0xAE, 0x33, 0x1D, 0x1C, 0xAE, 0x33, 0x1D, 0x1C,
		0xAE, 0x33, 0x1C, 0x1B, 0xAD, 0x33, 0x1C, 0x1B, 0xAE, 0x33, 0x1C, 0x1C, 0xB0, 0x33, 0x1B, 0x1D,
		0xB0, 0x33, 0x1C, 0x1C, 0xAF, 0x33, 0x1C, 0x1B, 0xAE, 0x33, 0x31, 0x2F, 0xB5, 0x33, 0x00, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
		0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF
};
