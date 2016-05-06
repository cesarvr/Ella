{
    "targets": [{
        "target_name": "ella",
        "sources": ["src/java/jvm_handler.cpp", "src/java/jvm_object.cpp", "src/java/args.cpp",
                    "src/java/classpath.cpp", "src/javascript/ella_objects.cpp", "src/ella.cpp"],

        "make_global_settings": [
            ["CC", 'usr/bin/clang'],
            ["CXX", "/usr/bin/clang++"],
            ["LINK", "/usr/bin/clang++"]
        ],

        "cflags_cc": ["-fexceptions"],

        "include_dirs": [

            "<!(node -e \"require('nan')\")",
            "src/java/",
            "src/javascript/"
        ],

        "conditions": [
            ['OS=="mac"', {
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

                    "cflags": ['-std=c++11', "-Wall", "-fexceptions"],

                    'include_dirs': [
                        'linux/jdk8/java/',
                        'linux/jdk8/java/linux'
                    ],
                }
            ]
        ]
    }]
}
