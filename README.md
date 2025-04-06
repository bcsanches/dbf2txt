dbf2txt
=======

A small C++ library for reading dbf files. It was designed for converting dbf files to txt and can be easily used for generating files in any format desired.
Free for commercial use, open source under the ZLib License.

http://github.com/bcsanches/dbf2txt

You can use CMAKE for building it and use it as a submodule on your projects.

The code includes a simple sample, that you can build with cmake and use simple as:

```
sample input.dbf output.txt
```

This will output all data from input.dbf to output.txt.

In case you need only certain fields, use as:


```
sample input.dbf output.txt CODE NAME AGE
```

This will output only the fields CODE, NAME and AGE, ignoring any other fields in the file.

This is a simple dump tool to output DBF files to any format you need. In my case, I wrote it when I needed to migrate from a old system to a new format.
