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
            path: "core/src",
            sources: [
              "."
            ],
            publicHeadersPath: ".",
            cSettings: [
                .headerSearchPath("."),
            ]
        ),
    ],
    cxxLanguageStandard: .cxx1z
)

