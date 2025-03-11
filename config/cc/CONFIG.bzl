load("@obazl_tools_cc//config:BASE.bzl",
     _BASE_COPTS    = "BASE_COPTS",
     _define_module_version = "define_module_version")

define_module_version = _define_module_version

BASE_COPTS = _BASE_COPTS

BASE_DEPS = ["@liblogc//lib:logc"]

BASE_DEFINES = [] # "PROFILE_$(COMPILATION_MODE)"]

BASE_LOCAL_DEFINES = []

BASE_LINKOPTS      = []

# def pkg_include(pkg):
#     if native.repository_name() == "@":
#         return pkg # native.package_name()
#     else:
#         return "external/{}~{}/{}".format(
#             native.module_name(), native.module_version(), pkg)
