from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class DevopsAppConan(ConanFile):
    name = "devops-console-app"
    package_type = "application"
    settings = "os", "compiler", "build_type", "arch"

    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # Qt n'est volontairement PAS géré par Conan ici : on s'appuie sur
        # l'installation Qt système/aqtinstall, comme c'est l'usage courant
        # en entreprise pour éviter de rebuild Qt à chaque poste dev.
        self.requires("devops-core/0.1.0")
        self.requires("devops-protocol/0.1.0")
        self.requires("devops-network/0.1.0")
        self.requires("devops-ui-components/0.1.0")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
