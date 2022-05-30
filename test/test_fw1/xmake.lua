target("test_fw")
    set_kind("binary")
    set_targetdir("dist")
    set_policy("check.auto_ignore_flags", false)
    add_files("src/*.c")
    add_files("src/*.s")
    add_cflags("-march=rv32i")
    add_cflags("-mabi=ilp32")

    add_asflags("-march=rv32i")
    add_asflags("-mabi=ilp32")

    add_ldflags("-march=rv32i")
    add_ldflags("-mabi=ilp32")
    add_ldflags("-T$(scriptdir)/link.lds")
    add_ldflags("-nostartfiles")