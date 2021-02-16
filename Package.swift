// swift-tools-version:5.3

import PackageDescription

let package = Package(
    name: "CppZXingCpp",
    products: [
        .library(name: "CppZXingCpp", targets: ["CppZXingCpp"])
    ],
    targets: [
        .target(
            name: "CppZXingCpp",
            path: "core",
            exclude: [
                "ZXVersion.h.in",
                "Android.mk",
                "ZXingConfig.cmake.in",
                "CMakeLists.txt",
            ],
            sources: [
                "src",
            ],
            publicHeadersPath: "swiftpm",
            cSettings: [
                .headerSearchPath("src"),
            ]
        ),
    ],
    cxxLanguageStandard: .cxx1z
)
