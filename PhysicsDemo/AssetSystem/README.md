# Asset System

The asset system is a custom file system for the Editor. Its goal is to create and manage loadable assets in-editor or in-game. While the system encompasses the entire system, from data structures to the graphical user interface, smaller components and system make this all possible.

An asset is serialized to a JSON formatted file. As we store it in plain text, manipulating and reading the file is easy for humans. The an asset file (`*.asset`) is structured as follows:

```json
// A JSON representation of an asset.
{
    "parent": 
}
```