{
    "targets": [{
        "target_name": "ella",
        "sources": ["ella.cpp"],
        "cflags": ["-std=c++11", '-stdlib=libc++', "-fexceptions"],
        'cflags_cc': ['-fexceptions'],
        "include_dirs": [

            "<!(node -e \"require('nan')\")",
            "include/java/",
            "include/javascript/",
        ],
        'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS': ['-std=c++11'],
        },
    }]
}
