{
    'targets': [
        {
            'target_name': 'lockfile_bindings',
            'include_dirs': [
                "<!(node -e \"require('nan')\")",
                './src'
            ],
            'sources': [
                'src/lockfile.cc'
            ],
            'conditions': [
                [
                    'OS=="win"', {
                        'defines': [
                            'OS_WINDOWS'
                        ],
                        'sources': [
                            'src/lockfile_win.cc'
                        ]
                    }
                ],
                [
                    'OS=="linux"', {
                        'defines': [
                            'OS_LINUX'
                        ],
                        'sources': [
                            'src/lockfile_unix.cc'
                        ]
                    }
                ]
            ]
        }
    ]
}
