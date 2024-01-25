// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "OpenVanillaLib",
    products: [
        // Products define the executables and libraries a package produces, making them visible to other packages.
        .library(
            name: "OpenVanilla",
            targets: ["OpenVanilla",
                      "OpenVanillaImpl",
                      "LoaderService",
                      "ModuleManager",
                      "LegacyOpenVanilla",
                      "OVIMBig5Code",
                      "OVIMTableBased",
                      "OVIMArray",
                      "OVAFAssociatedPhrases"]),
    ],
    dependencies: [
        .package(path: "../CandidateUI"),
        .package(path: "../TooltipUI"),
        .package(path: "../VXHanConvert"),
    ],
    targets: [
        // Targets are the basic building blocks of a package, defining a module or a test suite.
        // Targets can depend on other targets in this package and products from dependencies.
        .target(
            name: "OpenVanilla"),
        .target(
            name: "OpenVanillaImpl",
            dependencies: ["OpenVanilla", "CandidateUI"],
            cxxSettings: [.unsafeFlags(["-fcxx-modules", "-fmodules"])]
        ),
        .target(
            name: "LoaderService",
            dependencies: ["OpenVanilla", "OpenVanillaImpl"],
            cxxSettings: [.unsafeFlags(["-fcxx-modules", "-fmodules"])]
        ),
        .target(
            name: "ModuleManager",
            dependencies: ["OpenVanilla",
                           "OpenVanillaImpl",
                           "LoaderService",
                           "CandidateUI", "TooltipUI", "VXHanConvert",
                           "OVIMBig5Code", "OVIMTableBased", "OVIMArray", "OVAFAssociatedPhrases"],
            cxxSettings: [.unsafeFlags(["-fcxx-modules", "-fmodules"])]
        ),
        .target(
            name: "LegacyOpenVanilla",
            dependencies: ["OpenVanilla"]),
        .target(
            name: "OVIMBig5Code",
            dependencies: ["OpenVanilla"]),
        .target(
            name: "OVIMTableBased",
            dependencies: ["OpenVanilla"]),
        .target(
            name: "OVIMArray",
            dependencies: ["OpenVanilla", "LegacyOpenVanilla"]),
        .target(
            name: "OVAFAssociatedPhrases",
            dependencies: ["OpenVanilla"]),
//        .testTarget(
//            name: "OpenVanillaTests",
//            dependencies: ["OpenVanilla"]),
    ],
    cxxLanguageStandard: CXXLanguageStandard.cxx11
)
