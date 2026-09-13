from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class DevopsProtocolConan(ConanFile):
    name = "devops-protocol"
    package_type = "library"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = "CMakeLists.txt", "include/*", "src/*"

    def requirements(self):
        self.requires("nlohmann_json/3.11.3")
        self.requires(f"devops-core/{self._core_version()}")

    def _core_version(self):
        # En pratique : lu depuis un fichier de version partagé ou fixé
        # explicitement à chaque bump. Volontairement isolé ici pour que
        # la stratégie de versioning soit visible et discutable en revue.
        return "0.1.0"

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
        self.cpp_info.libs = ["devops-protocol"]
        self.cpp_info.set_property("cmake_target_name", "devops::protocol")
