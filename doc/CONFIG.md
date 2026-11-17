# The Ignis configuration format

Ignis requires that a configuration file is present on the boot volume. The configuration
file describes menu entries, protocols and various other variables. Each menu entry is to
be declared with its name preceding a pair of curly braces ('{}'), within those curly braces
exist information regarding the boot protocol, the kernel path and other information that is
described in the "Menu Entry Metadata" section.

An example of the configuration format can be found in ``test/ignis.cfg``

## Limitations

We currently only support a maximum number of 8 boot entries.

## Menu Entry Metadata

Valid keys are described below:

- kernel : Provides the path of the binary to be loaded
- protocol : The protocol to be used (as described in "Valid Protocols")

## Valid Protocols

- flat : Flat binary, no structure, expected to be loaded at 2 MiB
