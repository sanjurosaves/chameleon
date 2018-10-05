# CHAMELEON audio transcoder v0.5

### Description
Linux app to easily convert audio files between formats and compression schemes.

### Environment
This program was developed and tested on `Ubuntu 16.04 LTS`.

### Dependencies
[libsndfile](https://github.com/erikd/libsndfile)

### Installation & Compilation
Install the above listed dependency. Try `sudo apt-get install libsndfile1-dev`. Then clone the repository and compile with gcc using the provided makefile.
```
$ git clone https://github.com/sanjurosaves/chameleon.git
$ cd chameleon
$ make
gcc chameleon.c -lsndfile -Wall -Werror -Wextra -pedantic -o chameleon
```

### Usage
                chameleon <input_file> output_type
                Valid output types:
                                        ogg
                                        flac
                                        wave
                                        aiff
                                        ima_adpcm_wav
```
$ ./chameleon input_file output_type
```

###### Example command line calls
```
$ tree -s
├── [      13720]  chameleon
├── [  119306039]  Goo-Dizzy-Iris.flac
├── [   24285964]  Meshell-FeelingGood.flac
├── [   31447501]  M.I.A.-BadGirls.flac
├── [    8418661]  Nina-NobodyKnowsYouWhenYou'reDownandOut.flac
└── [  123080056]  Tom-American Girl.aif
$
$ ./chameleon Goo-Dizzy-Iris.flac ogg
    Goo-Dizzy-Iris.flac -> Goo-Dizzy-Iris.ogg ... ok
$ ./chameleon Meshell-FeelingGood.flac aiff
    Meshell-FeelingGood.flac -> Meshell-FeelingGood.aiff ... ok
$ ./chameleon M.I.A.-BadGirls.flac ima_adpcm_wav
    M.I.A.-BadGirls.flac -> M.I.A.-BadGirls_ima_adpcm.wav ... ok
$ ./chameleon Nina-NobodyKnowsYouWhenYou\'reDownandOut.flac wave
    Nina-NobodyKnowsYouWhenYou'reDownandOut.flac -> Nina-NobodyKnowsYouWhenYou'reDownandOut.wav ... ok
$ ./chameleon Tom-American\ Girl.aif ogg
    Tom-American Girl.aif -> Tom-American Girl.ogg ... ok
$
$ tree -s
├── [      13720]  chameleon
├── [  119306039]  Goo-Dizzy-Iris.flac
├── [    5807431]  Goo-Dizzy-Iris.ogg
├── [   44114166]  Meshell-FeelingGood.aiff
├── [   24285964]  Meshell-FeelingGood.flac
├── [   31447501]  M.I.A.-BadGirls.flac
├── [   10070076]  M.I.A.-BadGirls_ima_adpcm.wav
├── [    8418661]  Nina-NobodyKnowsYouWhenYou'reDownandOut.flac
├── [   28565084]  Nina-NobodyKnowsYouWhenYou'reDownandOut.wav
├── [  123080056]  Tom-American Girl.aif
└── [    3538576]  Tom-American Girl.ogg
```

# Author
* [**Alex Allen**](https://github.com/sanjurosaves)
