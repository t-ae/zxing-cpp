/*
* Copyright 2016 Nu-book Inc.
* Copyright 2016 ZXing authors
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

#include "QRReader.h"

#include "BinaryBitmap.h"
#include "BitMatrix.h"
#include "DecodeHints.h"
#include "DecoderResult.h"
#include "DetectorResult.h"
#include "QRDecoder.h"
#include "QRDecoderMetadata.h"
#include "QRDetector.h"
#include "Result.h"
#include "ResultPoint.h"

#include <utility>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../thirdparty/stb/stb_image_write.h"

namespace ZXing::QRCode {

struct RGBA {
    unsigned char r, g, b, a; //赤, 緑, 青, 透過
    RGBA() = default;
    constexpr RGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
};

Reader::Reader(const DecodeHints& hints)
	: _tryHarder(hints.tryHarder()), _isPure(hints.isPure()), _charset(hints.characterSet())
{
}

Result
Reader::decode(const BinaryBitmap& image) const
{
	auto binImg = image.getBlackMatrix();
	if (binImg == nullptr) {
		return Result(DecodeStatus::NotFound);
	}

	auto detectorResult = Detect(*binImg, _tryHarder, _isPure);
	if (!detectorResult.isValid())
		return Result(DecodeStatus::NotFound);
    
    {
        auto bitmap = ToMatrix<RGBA>(detectorResult.bits(), RGBA(0,0,0,255), RGBA(255,255,255,255));
        char filePath[256];
        sprintf(filePath, "/Users/iceman/Desktop/detectorResult.png");
        stbi_write_png(filePath, bitmap.width(), bitmap.height(), sizeof(RGBA), bitmap.data(), 0);
    }

	auto decoderResult = Decode(detectorResult.bits(), _charset);
	auto position = detectorResult.position();

	// TODO: report the information that the symbol was mirrored back to the caller
	//bool isMirrored = decoderResult.extra() && static_cast<DecoderMetadata*>(decoderResult.extra().get())->isMirrored();

	return Result(std::move(decoderResult), std::move(position), BarcodeFormat::QRCode);
}

} // namespace ZXing::QRCode
