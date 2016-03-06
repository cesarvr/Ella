{
    "make_global_settings": [
        ["CXX", "/usr/bin/clang++"],
        ["LINK", "/usr/bin/clang++"]
    ],

    "targets": [{
        "target_name": "ella",
        "sources": [ "include/java/jvm_handler.cpp", "include/java/jvm_object.cpp", "include/java/jvm_argument.cpp", "include/java/java_value.cpp","include/javascript/adapter.cpp", "main.cpp"],
        "cflags": ["-std=c++11", "-stdlib=libc++", "-fexceptions"],
        "cflags_cc": ["-fexceptions"],
        "include_dirs": [

            "<!(node -e \"require('nan')\")",
            "include/java/",
            "include/javascript/"
        ],

        "conditions": [
            ["OS=='mac'", {
                "defines": [
                    "__MACOSX_CORE__"
                ],
                "libraries": ["-framework", "JavaVM"],
                "xcode_settings": {
                    "OTHER_CPLUSPLUSFLAGS": ["-std=c++11", "-stdlib=libc++"],
                    "OTHER_LDFLAGS": ["-stdlib=libc++"],
                    "MACOSX_DEPLOYMENT_TARGET": "10.7",
                    "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
                }
            }]
        ]
    }]
}
