# Overview #

**Never-out-of-tune Instrument Tuner** is a cross-platform and open source application that can turn a computer and a microphone into a very accurate instrument tuner.

Even if it was conceived especially for tuning a piano, it can be used with any musical instrument.

**Latest version: 0.3.0 (2010-10-14)** ([Changes](News))

[Go to download page](http://code.google.com/p/noot-tuner/downloads/list)

## Features ##
  * Easy to use (default settings will work in most cases)
  * It works with almost any instrument
  * Half-cent precision (0.005 semitones) or even better
  * Octave and/or note selection
  * Clock correction
  * Transposition
  * Clipping detection
  * Selectable temperament
  * Internationalisation (English and Italian for now, other languages will follow)
  * Cross-platform (it can be compiled on virtually any OS; Windows, Ubuntu and generic Linux packages available now, other packages available soon)
  * Basic documentation included
  * Two different frequency refinement algorithms

## Future plans ##
  * Fully customisable temperament.
  * More ports (OSX, ...) and languages.
  * Migrate from wxWidget to Qt and from automake to CMake.

![http://lh4.ggpht.com/_MDBz3xURjQA/TIEmgBs2P1I/AAAAAAAAACA/TYcd2k0KnBQ/mainwindow.png](http://lh4.ggpht.com/_MDBz3xURjQA/TIEmgBs2P1I/AAAAAAAAACA/TYcd2k0KnBQ/mainwindow.png)

# Installation #

## All users ##
### General minimum requirements ###
  * An audio card with a configured microphone

## Windows users ##
### Minimum requirements ###
  * Windows 2000 or newer
  * About 10 MB of disk space

### Installation ###
Download the installer from the [Download page](https://github.com/kastel/noot-tuner/releases) and launch it. Then follow the instructions on-screen.

## Ubuntu users ##
Packages and updates for Ubuntu are downloaded automatically if you add the PPA for NOOT. Just run

```
sudo apt-add-repository ppa:kastel88/noot-ppa
```

in a terminal. Then update the repository and install noot-tuner with the package manager you prefer. You can do this by typing the following commands in a terminal:

```
sudo apt-get update
sudo apt-get install noot-tuner
```

For more information, visit [the PPA for NOOT](https://launchpad.net/~kastel88/+archive/noot-ppa).

## Mac OSX and other Linux distributions ##

Binary installers are not currently supported. See section on [compiling from source](#Compiling).

# Compiling #

This section will show you how to compile NOOT Tuner from source. This step is needed if there isn't any binary package for your operating system.

## Step 1: Requirements ##

In order to compile from source you must have the following libraries installed (with their headers):
  * [wxWidgets](http://www.wxwidgets.org/) version 2.8 or 2.9
  * [FFTW](http://www.fftw.org/) v. 3
  * [Portaudio](http://www.portaudio.com/) v. 19

## Step 2: Download ##

Download the pre-packaged source from [https://github.com/kastel/noot-tuner/releases] (files ending in `.tar.gz`). This is different from downloading the source from the GitHub repository (not recommended unless you are a developer; see alternate instruction at the end of the section).

## Step 3: Unpack ##

Unpack the source package in a directory of your preference: open the terminal, type

> `cd ` _your directory_

> `tar -xf ` _package file_

> `cd noot-tuner`

## Step 3: Configure & Make ##

After unpacking, type the following commands in the terminal:

```
./configure
make
sudo make install
```

NOOT should be immediately usable. If it is not listed in the Applications menu, you can launch it from the terminal by typing `noot-tuner`.

## Building from git ##

If you check out the source code from GitHub directly, you will also need the following packages, including header files:
  * automake
  * autoconf
  * libtool
  * gettext

After extracting (or cloning) the repository, enter the repository directory and run

```
./reconfigure.sh
```

(The script may require pressing ENTER at a certain stage.) This will recreate all the files that are not part of the repository but are distributed in the source package. Then you can run `./configure` and `make install` as usual.
