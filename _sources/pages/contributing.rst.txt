Contributing
============

This page contains some information that might be helpful for developing Thallium.


Editor configuration
--------------------

An ``.editorconfig`` file is provided to ensure consistent formatting where supported.


VS Code guidance
^^^^^^^^^^^^^^^^

For convenience, VS Code build and launch scripts (to be put in the ``.vscode/`` folder) are provided in the dropdown below.

.. dropdown:: VS Code configuration scripts

    .. tab-set::

        .. tab-item:: tasks.json

            .. code-block:: javascript

                {
                    "version": "2.0.0",
                    "tasks": [
                        {
                            "type": "shell",
                            "label": "__config",

                            "command": "cmake",
                            "args": [
                                ".",
                                "-B",
                                "${workspaceFolder}/build",

                                "-DCMAKE_BUILD_TYPE=Debug",
                                "-DCMAKE_EXPORT_COMPILE_COMMANDS=1",

                                "-DTHALLIUM_BUILD_MODULE_VULKAN=ON",

                                "-DTHALLIUM_BUILD_TESTS=ON",
                                "-DTHALLIUM_BUILD_DOCS=ON",
                            ]
                        },
                        {
                            "type": "shell",
                            "label": "__build",

                            "command": "cmake",
                            "args": [
                                "--build",
                                "${workspaceFolder}/build"
                            ],

                            "problemMatcher": [
                                "$gcc"
                            ],
                        },
                        {
                            "type": "shell",
                            "label": "__clean",

                            "command": "rm",
                            "args": [
                                "-rf",
                                "${workspaceFolder}/build"
                            ],
                        },
                        {
                            "type": "shell",
                            "label": "__clear_out",

                            "command": "clear"
                        },

                        {
                            "type": "shell",
                            "label": "build project",

                            "dependsOrder": "sequence",
                            "dependsOn": [
                                "__clear_out",
                                "__config",
                                "__build"
                            ],

                            "group": {
                                "kind": "build",
                                "isDefault": true
                            },
                        },
                        {
                            "type": "shell",
                            "label": "clean and rebuild project",

                            "dependsOrder": "sequence",
                            "dependsOn": [
                                "__clear_out",
                                "__clean",
                                "build project"
                            ]
                        },
                    ]
                }


        .. tab-item:: launch.json

            .. code-block:: javascript

                {
                    "inputs": [
                        {
                            "id": "testExec",
                            "type": "pickString",
                            "description": "Thallium test executable",
                            "options": [
                                "hellotriangle",
                                "standalone",
                            ],
                            "default": "hellotriangle"
                        }
                    ],
                    "configurations": [
                        {
                            "type": "lldb",
                            "request": "launch",
                            "name": "(lldb) Launch",
                            "program": "${workspaceFolder}/build/tests/${input:testExec}",
                            "args": [],
                            "cwd": "${workspaceFolder}",
                            "preLaunchTask": "build project"
                        },
                        {
                            "name": "(gdb) Launch",
                            "type": "cppdbg",
                            "request": "launch",
                            "program": "${workspaceFolder}/build/tests/${input:testExec}",
                            "args": [],
                            "stopAtEntry": false,
                            "cwd": "${workspaceFolder}",
                            "externalConsole": false,
                            "MIMode": "gdb",
                            "preLaunchTask": "build project"
                        }
                    ],
                    "version": "2.0.0"
                }
