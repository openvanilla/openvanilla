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
                      "OVModuleManager",
                      "OpenVanillaImp",
                      "OVIMBig5Code",
                      "OVIMTableBased",
                      "OVIMArray",
                      "OVAFAssociatedPhrases"]),
    ],
    dependencies: [.package(path: "../Voltaire")],
    targets: [
        // Targets are the basic building blocks of a package, defining a module or a test suite.
        // Targets can depend on other targets in this package and products from dependencies.
        .target(
            name: "OpenVanilla"),
        .target(
            name:"OVModuleManager",
            dependencies: ["OpenVanilla",
                           "OpenVanillaImp",
                           "OVIMBig5Code",
                           "OVIMTableBased",
                           "OVIMArray",
                           "OVAFAssociatedPhrases"]
            ),
        .target(
            name: "OpenVanillaImp",
            dependencies: ["OpenVanilla", "Voltaire"]),
        .target(
            name: "OVIMBig5Code",
            dependencies: ["OpenVanilla"]),
        .target(
            name: "OVIMTableBased",
            dependencies: ["OpenVanilla"]),
        .target(
            name: "OVIMArray",
            dependencies: ["OpenVanilla"]),
        .target(
            name: "OVAFAssociatedPhrases",
            dependencies: ["OpenVanilla"]),
//        .testTarget(
//            name: "OpenVanillaTests",
//            dependencies: ["OpenVanilla"]),
    ],
    cxxLanguageStandard: CXXLanguageStandard.cxx11
)
