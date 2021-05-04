# Surge XT

**If you are a musician looking to use Surge, please download the appropriate binary
[from our website](https://surge-synthesizer.github.io). Surge Synth Team makes regular releases for all supported platforms.**

**If you are a developer looking to compile a stable production version of Surge, please do not
use the main branch; instead, use a release tag (such as `release_1.9.0`) or branch (such as
`release/1.9.0`). Surge is undergoing substantial development right now, and the `main` branch is currently an
incomplete alpha version for our Fall 2021 release.**


CI: [![CI Build Status](https://dev.azure.com/surge-synthesizer/surge/_apis/build/status/surge-synthesizer.surge?branchName=main)](https://dev.azure.com/surge-synthesizer/surge/_build/latest?definitionId=2&branchName=main)
Release: [![Release Build Status](https://dev.azure.com/surge-synthesizer/surge/_apis/build/status/surge-synthesizer.releases?branchName=master)](https://dev.azure.com/surge-synthesizer/surge/_build/latest?definitionId=1&branchName=master)
Release-XT: [![Release-XT Build Status](https://dev.azure.com/surge-synthesizer/surge/_apis/build/status/surge-synthesizer.releases-xt?branchName=master)](https://dev.azure.com/surge-synthesizer/surge/_build/latest?definitionId=13&branchName=master)

Surge is a free and open-source hybrid synthesizer, originally written and sold as a commercial product
by @kurasu/Claes Johanson at [Vember Audio](http://vemberaudio.se). In September 2018,
Claes decided to release a partially completed version of Surge 1.6 under GPL3, and a group
of developers have been improving it since. You can learn more about the team at https://surge-synth-team.org/ or
connect with us on [Discord](https://raw.githubusercontent.com/surge-synthesizer/surge-synthesizer.github.io/master/_includes/discord_invite_link).

If you would also like to participate in discussions, testing and design of Surge, we have
details below and also in the [contributors section of the Surge website](https://surge-synthesizer.github.io/#contributors).

In Spring 2021, after the release of Surge 1.9, Surge Synth Team embarked on a plan to replatform Surge as a JUCE plugin.
There are a variety of reasons for this choice, including the difficulty of maintaining hand-rolled wrappers around VST3, AU and LV2
and limitations in the VSTGUI framework.

As such, if you are looking to build Surge in the 1.9 family, you need to use the GitHub
branch `classic` (for the head of the code; although it has no diffs since 1.9 of note)
or the tag `release_1.9.0` to build exactly the 1.9 release.

This readme serves as the root of developer documentation for Surge.


# Developing Surge XT

We welcome developers! Our workflow revolves around GitHub issues in this repository
and conversations at our Discord server and IRC chatroom. You can read our developer guidelines
in [our developer guide document](doc/Developer%20Guide.md). If you want to contribute and are new to Git,
we also have a [Git How To](doc/git-howto.md), tailored at Surge development.

The developer guide also contains information about testing and debugging in particular hosts
on particular platforms.

Surge XT uses CMake for all of its build-related tasks, and requires a set of free tools to build the synth. If you
have a development environment set up, you almost definitely have what you need, but if not, please check out:

- [Setting up Build Environment on Windows](#windows)
- [Setting up Build Environment on macOS](#macos)
- [Setting up Build Environment on Linux](#linux)

Once you have set your environment up, you need to checkout the Surge code with Git, grab submodules, run CMake
to configure, then run CMake to build. Your IDE may support CMake (more on that below), but a reliable way to build
Surge on all platforms is:

```
git clone https://github.com/surge-synthesizer/surge.git
cd surge
git submodule update --init --recursive
cmake -Bbuild
cmake --build build --config Release --target surge-staged-assets
```

This will build all the Surge binary assets in the directory `build/surge_xt_products` and is often enough of a formula
to do a build.

## Developing from your own fork

Our [Git How To](doc/How%20to%20Git.md) explains how we are using Git. If you want to develop
from your own fork, please consult there, but the short version is (1) fork this project
on GitHub and (2) clone your fork, rather than the main repo as described above. So press the `Fork`
button here and then:

```
git clone git@github.com:youruserid/surge.git
```

and the rest of the steps are unchanged.

## Building projects for your IDE

When you run the first CMake step, CMake will generate IDE-compatible files for you.
On Windows, it will generate Visual Studio files. On Mac it will generate
makefiles by default, but if you add the argument `-GXcode` you can get an XCode project if you want.

Surge developers regularly develop with all sorts of tools. CLion, Visual Studio, vim, emacs,
VS Code, and many others can work properly with the software.

## Building a VST2

Due to licensing restrictions, VST2 builds of Surge **may not** be re-distributed.
However, it is possible to build a VST2 of Surge for your own personal use.
First, obtain a local copy of the VST2 SDK, and unzip it to a folder of your choice.
Then set `VST2SDK_DIR` to point to that folder:

```
export VST2SDK_DIR="/your/path/to/VST2SDK"
```

or, in the Windows command prompt:

```
set VST2SDK_DIR=c:\path\to\VST2SDK
```

Finally, run a fresh CMake, and build the VST2 targets:

```
cmake -Bbuild_vst2
cmake --build build_vst2 --config Release --target surge-xt_VST --parallel 4
cmake --build build_vst2 --config Release --target surge-fx_VST --parallel 4
```

You will then have VST2 plugins in `build_vst2/surge-xt_artefacts/Release/VST` and  `build_vst2/surge-fx_artefacts/Release/VST` respectively.
Adjust the number of cores that will be used for building process by modifying the value of `--parallel` argument.

## Building with support for ASIO

On Windows, building with ASIO is often preferred for Surge standalone,
since it enables users to use the ASIO low-latency audio driver.

Unfortunately, due to licensing conflicts, binaries of Surge that are built
with ASIO **may not** be re-distributed. However, you can build Surge with ASIO
for your own personal use, provided you do not re-distribute those builds.

If you already have a copy of the ASIO SDK, simply set the following environment variable:

```
set ASIOSDK_DIR=c:\path\to\asio
```

If you do not have a copy of the ASIO SDK, CMake can download it for you, and
allow you to build with ASIO under your own personal license. To enable this
functionality, run your CMake configuration command as follows:

```
cmake -Bbuild -DBUILD_USING_MY_ASIO_LICENSE=True
```

## Building an LV2

On Linux, using a community fork of JUCE, you can build an LV2. Here's how. We assume you have checked out Surge and can build.

First, clone https://github.com/lv2-porting-project/JUCE/tree/lv2 on branch lv2, to some directory of your chosing.

```
sudo apt-get install -y lv2-dev
cd /some/location
git clone --branch lv2 https://github.com/lv2-porting-project/JUCE JUCE-lv2
```

then run a fresh CMake to (1) point to that JUCE fork and (2) activate LV2

```
cmake -Bbuild_lv2 -DCMAKE_BUILD_TYPE=Release -DJUCE_SUPPORTS_LV2=True -DSURGE_ALTERNATE_JUCE=/some/location/JUCE-lv2/
cmake --build build_lv2 --config Release --target surge-xt_LV2 --parallel 4
cmake --build build_lv2 --config Release --target surge-fx_LV2 --parallel 4
```

You will then have LV2s in `build_lv2/surge-xt_artefacts/Release/LV2` and  `build_lv2/surge-fx_artefacts/Release/LV2`
respectively.

## Building an Installer

The CMake target `surge-xt-distribution` builds an install image on your platform
at the end of the build process. On Mac and Linux, the installer generator is built
into the platform; on Windows, our CMake file uses NuGet to download InnoSetup, so
you will need the [nuget.exe CLI](https://nuget.org/) in your path.

## Platform Specific Choices

### Building 32- vs 64-bit on Windows

If you are building with Visual Studio 2019, then use the `-A` flag in your CMake command to specify 32/64-bit:

```bash
# 64-bit
cmake -Bbuild -G"Visual Studio 16 2019" -A x64

# 32-bit
cmake -Bbuild -G"Visual Studio 16 2019" -A Win32
```

If you are using an older version of Visual Studio, you must specify your preference with your choice
of CMake generator:

```bash
# 64-bit
cmake -Bbuild -G"Visual Studio 15 2017 Win64"

# 32-bit
cmake -Bbuild -G"Visual Studio 15 2017"
```

### Building a Mac Fat Binary (ARM/Intel)

### Building for Raspberry Pi

To build for a Raspberry Pi, you want to add the `LINUX_ON_ARM` CMake variable when you first run CMake. Otherwise,
the commands are unchanged. So, on a Pi, you can do:

```
cmake -Bbuild -DLINUX_ON_ARM=True
cmake --build build --config Release --target surge-staged-assets
```

Cross-compiling should also work, but we've not tried it in this cycle. If you get it to work with one of the
CMake toolchain files in CMake, we would welcome a pull request to this documentation with information.

# Setting up for Your OS

## Windows

You need to install the following:

* [Visual Studio 2017, 2019, or later(version 15.5 or newer)](https://visualstudio.microsoft.com/downloads/)
* Install [Git](https://git-scm.com/downloads), [Visual Studio 2017 or newer](https://visualstudio.microsoft.com/downloads/)
* When you install Visual Studio, make sure to include CLI tools and CMake, which are included in
'Optional CLI support' and 'Toolset for Desktop' install bundles

## macOS

To build on macOS, you need `Xcode`, `Xcode Command Line Utilities`, and CMake. Once you have installed
`Xcode` from the App Store, the command line to install the `Xcode Command Line Utilities` is:

```
xcode-select --install
```

There are a variety of ways to install CMake. If you use [homebrew](https://brew.sh) you can:

```
brew install cmake
```

## Linux

Most Linux systems have CMake, Git and a modern C++ compiler installed. Make sure yours does.
We test with most gccs older than 7 or so and clangs after 9 or 10.
You will also need to install a set of dependencies:

- build-essential
- libcairo-dev
- libxkbcommon-x11-dev
- libxkbcommon-dev
- libxcb-cursor-dev
- libxcb-keysyms1-dev
- libxcb-util-dev

# Continuous Integration

In addition to the build commands above, we use Azure pipelines for continuous integration.
This means that each and every pull request will be automatically built on all our environments,
and a clean build on all platforms is an obvious pre-requisite. If you have questions about
our CI tools, don't hesitate to ask on our [Discord](https://raw.githubusercontent.com/surge-synthesizer/surge-synthesizer.github.io/master/_includes/discord_invite_link)
server. We are grateful to Microsoft for providing Azure pipelines for free to the open-source community!

# References

  * Most Surge-related conversation happens on the Surge Synthesizer Discord server. You can join via [this link](https://raw.githubusercontent.com/surge-synthesizer/surge-synthesizer.github.io/master/_includes/discord_invite_link)
  * IRC channel at `#surgesynth` at [irc.freenode.net](https://irc.freenode.net). The logs are available at https://freenode.logbot.info/surgesynth/.
  * Discussion at KvR forum [here](https://www.kvraudio.com/forum/viewtopic.php?f=1&t=511922)
