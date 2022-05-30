set_symbols("debug")
set_strip("none")
set_optimize("none")

target("rvvm")
    set_kind("static")
    set_warnings("all", "error")
    set_targetdir("dist")
    set_languages("c++17")
    add_files("src/*.cc")
    add_includedirs("include", {public=true})

includes("rv32_loader")
