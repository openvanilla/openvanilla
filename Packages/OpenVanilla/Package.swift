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
        .package(path: "../SystemCharacterInfo"),
    ],
    targets: [
        // Targets are the basic building blocks of a package, defining a module or a test suite.
        // Targets can depend on other targets in this package and products from dependencies.
        .target(
            name: "OpenVanilla",
            cSettings: [
                .headerSearchPath("include"),
            ]
        ),
        .target(
            name: "OpenVanillaImpl",
            dependencies: ["OpenVanilla", "CandidateUI", "SystemCharacterInfo",],
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
            cSettings: [
                // We still have a number of umbrella headers that also define classes. For
                // example, OVIMTableBased.h defines OVIMTableBased, and OVIMTableBasedContext.h
                // depends on OVIMTableBased.h. It is not possible to also include
                // OVIMTableBasedContext.h in OVIMTableBased.h, but this causes clang to complain
                // about incomplete umbrella headers. Until we no longer define classes in umbrella
                // headers, we'll rely on these to get rid of the warnings. Fortunately for these
                // affected modules, only the principle class defined in the umbrella header matters
                // when it comes to using those modules.
                .unsafeFlags(["-Wno-incomplete-umbrella"], .when(configuration: .release)),
                .unsafeFlags(["-Wno-incomplete-umbrella"], .when(configuration: .debug))
            ],
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
    ],
    cxxLanguageStandard: CXXLanguageStandard.cxx11
)
