# DPCM audio compression encoder & decoder v0.1

### Description
Imperfect simple implementation of DPCM audio compression, achivieving 1.88:1 compression. This is an extremely basic, purely academic implementation with no practical use case, as the codec yields an extremely noisey audio stream.

### Environment
This program was developed and tested on `Ubuntu 14.04 LTS`.

### Installation & Compilation
Clone the repository and then compile with gcc using the provided makefile.
```
$ git clone https://github.com/sanjurosaves/eoy_low_level_audio.git
$ cd eoy_low_level_audio
$ make
gcc -Wall -Werror -Wextra -pedantic mains/main_compress_pcm_to_dpcm.c dpcm.c deltas.c samples.c util.c -o encode
gcc -Wall -Werror -Wextra -pedantic mains/main_decompress_dpcm.c dpcm.c deltas.c samples.c util.c -o decode
```

### Constraints
The original audio source file that gets fed to the encoder must be a stereo (2-channel) 16-bit little-endian PCM file encodeded at a rate of 44,100 samples per second. This must be a "raw" PCM file. The program is not setup to handle containerized formats such as WAVE or AIFF.

### Usage
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