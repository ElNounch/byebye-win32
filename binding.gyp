{
  "targets": [
    {
      "target_name": "<(module_name)",
      "sources": [ "src/byebye.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
	  "win_delay_load_hook": "true"
	},
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
