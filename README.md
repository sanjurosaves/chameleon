# CHAMELEON audio transcoder v0.5

### Description
Linux app to easily convert audio files between formats and compression schemes.

### Environment
This program was developed and tested on `Ubuntu 16.04 LTS`.

### Dependencies
[libsndfile](https://github.com/erikd/libsndfile)

### Installation & Compilation
Install the above listed dependency. Try `sudo apt-get install libsndfile1-dev`
Clone the repository and then compile with gcc using the provided makefile.
```
$ git clone https://github.com/sanjurosaves/chameleon.git
$ cd chameleon
$ make
gcc chameleon.c -lsndfile -Wall -Werror -Wextra -pedantic -o chameleon
```

### Usage
                chameleon <inputfile> output_type
                Valid output types:
                                        ogg
                                        flac
                                        wave
                                        aiff
                                        ima_adpcm_wav
```
$ ./encode orignal_pcm_file dpcm_file
$ ./decode dpcm_file decoded_pcm_file
```

###### Example command line calls
```
$ ls -lt | awk '{print $9, $5}' | column -t
original_zebra.pcm  11818800
decode              18059
encode              18064
$ ./encode original_zebra.pcm z.dpcm
$ ls -lt | awk '{print $9, $5}' | column -t
original_zebra.pcm  11818800
z.dpcm              6278736
decode              18059
encode              18064
$ ./decode z.dpcm new_zebra.pcm
$ ls -lt | awk '{print $9, $5}' | column -t
new_zebra.pcm       11818800
z.dpcm              6278736
original_zebra.pcm  11818800
decode              18059
encode              18064
```

# Author
* [**Alex Allen**](https://github.com/sanjurosaves)