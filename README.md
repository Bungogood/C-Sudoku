# C Sudoku

## Running:

```
gcc [filename].c -o out
time ./out [input file] [output file]
```

## Challenge:

Originally from [here](https://codegolf.stackexchange.com/q/190727).

The file will be on the format:
```
<length>
<sudoku#1>,<solution#1>
<sudoku#2>,<solution#2>
...
<sudoku#n>,<solution#n>
```

To check solutions:
```
md5sum [filename]
```

MD5:
easy.txt ```3cb465ef6077c4fcab5bd6ae3bc50d62```
hard.txt ```41704fd7d8fd0723a45ffbb2dbbfa488```