{
    "targets": [{
        "target_name": "ella",
        "sources": ["include/java/jvm_handler.cpp", "include/java/jvm_object.cpp", "include/java/jvm_argument.cpp", "include/java/java_value.cpp", "include/javascript/adapter.cpp", "main.cpp"],
        "cflags_cc": ["-fexceptions"],
        "include_dirs": [

            "<!(node -e \"require('nan')\")",
            "include/java/",
            "include/javascript/"
        ],

        "conditions": [
            ['OS=="mac"', {
                    "make_global_settings": [
                        ["CC", 'usr/bin/clang'],
                        ["CXX", "/usr/bin/clang++"],
                        ["LINK", "/usr/bin/clang++"]
                    ],

                    "cflags": ["-std=c++11x", "-Wall", "-pedantic", "-fexceptions"],

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
                },

                "OS=='linux'", {
        
                    "cflags": ['-std=gnu++0x', "-Wall", "-pedantic", "-fexceptions"],

                    'include_dirs': [
                        'linux/java/',
                        'linux/java/linux'
                    ],

                }
            ]
        ]
    }]
}
