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
//        .testTarget(
//            name: "CZXingCppTest",
//            dependencies: ["CppZXingCpp"],
//            path: "test/swiftpm/CZXingCppTest"
//        ),
//        .testTarget(
//            name: "ZXingCppTest",
//            dependencies: ["CZXingCppTest"],
//            path: "test/swiftpm/ZXingCppTest"
//        )
    ],
    cxxLanguageStandard: .cxx1z
)
