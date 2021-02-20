/*
* Copyright 2019 Axel Waggershauser
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "ReadBarcode.h"

#include "BinaryBitmap.h"
#include "BitArray.h"
#include "BitMatrix.h"
#include "DecodeHints.h"
#include "GenericLuminanceSource.h"
#include "GlobalHistogramBinarizer.h"
#include "HybridBinarizer.h"
#include "MultiFormatReader.h"
#include "ThresholdBinarizer.h"

#include <memory>

#include "../../thirdparty/stb/stb_image_write.h"

static int count = 0;
static int gh_count = 0;

namespace ZXing {

struct RGBA {
    unsigned char r, g, b, a; //赤, 緑, 青, 透過
    RGBA() = default;
    constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
};

static Result ReadBarcode(GenericLuminanceSource&& source, const DecodeHints& hints)
{
	MultiFormatReader reader(hints);
	auto srcPtr = std::shared_ptr<LuminanceSource>(&source, [](void*) {});
    
    {
        auto aa = HybridBinarizer(srcPtr);
        auto w = aa.width();
        auto h = aa.height();
        auto bitmap = ToMatrix<RGBA>(*aa.getBlackMatrix(), RGBA(0,0,0,255), RGBA(255,255,255,255));
        char filePath[256];
        sprintf(filePath, "/Users/kenta/Desktop/cpp_hb_%d.png", count++);
        stbi_write_png(filePath, bitmap.width(), bitmap.height(), sizeof(RGBA), bitmap.data(), 0);
    }
    {
        auto gh = GlobalHistogramBinarizer(srcPtr);
        auto w = gh.width();
        auto h = gh.height();
        auto bitmap = ToMatrix<RGBA>(*gh.getBlackMatrix(), RGBA(0,0,0,255), RGBA(255,255,255,255));
        char filePath[256];
        sprintf(filePath, "/Users/kenta/Desktop/cpp_gh_%d.png", gh_count++);
        stbi_write_png(filePath, bitmap.width(), bitmap.height(), sizeof(RGBA), bitmap.data(), 0);
    }

	if (hints.binarizer() == Binarizer::LocalAverage)
		return reader.read(HybridBinarizer(srcPtr));
	else
		return reader.read(GlobalHistogramBinarizer(srcPtr));
}

Result ReadBarcode(const ImageView& iv, const DecodeHints& hints)
{
	switch (hints.binarizer()) {
	case Binarizer::BoolCast: return MultiFormatReader(hints).read(ThresholdBinarizer(iv, 0));
	case Binarizer::FixedThreshold: return MultiFormatReader(hints).read(ThresholdBinarizer(iv, 127));
	default:
		return ReadBarcode(
			{
				0,
				0,
				iv._width,
				iv._height,
				iv._data,
				iv._rowStride,
				iv._pixStride,
				RedIndex(iv._format),
				GreenIndex(iv._format),
				BlueIndex(iv._format),
				nullptr
			},
			hints);
	}
}

Result ReadBarcode(int width, int height, const uint8_t* data, int rowStride, BarcodeFormats formats, bool tryRotate,
				   bool tryHarder)
{
	return ReadBarcode({0, 0, width, height, data, rowStride, 1, 0, 0, 0, nullptr},
					   DecodeHints().setTryHarder(tryHarder).setTryRotate(tryRotate).setFormats(formats));
}

Result ReadBarcode(int width, int height, const uint8_t* data, int rowStride, int pixelStride, int rIndex, int gIndex,
				   int bIndex, BarcodeFormats formats, bool tryRotate, bool tryHarder)
{
	return ReadBarcode({0, 0, width, height, data, rowStride, pixelStride, rIndex, gIndex, bIndex, nullptr},
					   DecodeHints().setTryHarder(tryHarder).setTryRotate(tryRotate).setFormats(formats));
}

} // ZXing
