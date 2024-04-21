## How to use this .path file
1. Clone Open Rails from https://github.com/openrails/openrails.
2. Put the `ORC_endpoint_for_OpenRails.patch` in the main directory.
3. Run the command:
```bash
git apply ORC_endpoint_for_OpenRails.patch
```


## How to create this file
1. Modify Open Rails source code to suit your needs.
2. Run the command:
```bash
git diff > ORC_endpoint_for_OpenRails.patch
```
