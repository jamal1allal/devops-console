from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class DevopsNetworkConan(ConanFile):
    name = "devops-network"
    package_type = "library"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = "CMakeLists.txt", "include/*", "src/*"

    def requirements(self):
        self.requires("cpp-httplib/0.15.3")
        self.requires("nlohmann_json/3.11.3", transitive_headers=True, transitive_libs=True)
        self.requires("devops-core/0.1.0", transitive_headers=True, transitive_libs=True)
        self.requires("devops-protocol/0.1.0", transitive_headers=True, transitive_libs=True)

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        CMake(self).install()

    def package_info(self):
        self.cpp_info.libs = ["devops-network"]
        self.cpp_info.set_property("cmake_target_name", "devops::network")
