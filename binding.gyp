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

        'conditions': [
            ['OS=="mac"', {
                'xcode_settings': {
                    'OTHER_CPLUSPLUSFLAGS': ['-std=gnu++11'],
                    'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                }
            }]
        ],

    }]
}
