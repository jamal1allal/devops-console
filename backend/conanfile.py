from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class DevopsBackendConan(ConanFile):
    name = "devops-backend"
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"
    
    def requirements(self):
        self.requires("drogon/1.9.4")
        self.requires("libpqxx/7.9.2")
        self.requires("jwt-cpp/0.7.0")
        self.requires("nlohmann_json/3.11.3")
        # Dépendances internes, résolues depuis le dépôt privé
        self.requires("devops-core/0.1.0")
        self.requires("devops-protocol/0.1.0")
        self.requires("libsodium/1.0.20")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def build_requirements(self):
        self.test_requires("gtest/1.14.0")

    default_options = {
        "drogon/*:with_orm": True,
        "drogon/*:with_postgres": True,
        "drogon/*:with_postgres_batch": True,
    }