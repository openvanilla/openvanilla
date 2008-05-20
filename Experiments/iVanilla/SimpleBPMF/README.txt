iVanilla SimpleBPMF: An Experimental OpenVanilla Module on
Toolchain iPhone

May 20, 2008.

SimpleBPMF is an experiment. It implements a minimal "loader" of the 
OpenVanilla Framework and makes use of the Phonetic (Bopomofo) module,
a popular Traditional Chinese input method in Taiwan, and forms a
dynamically loadable bundle that you can "inject" into your running
iPhone application to achieve the effect of an input method plug-in.

The experiment was started by Yao-Chung Chang and Lukhnos D. Liu on
an 1.1.2 iPhone/iPod Touch "toolchain".

To build, use the Makefile. We don't guarantee it will build with your
own toolchain.

If you do come up with a built product, load the app in shall with 
DYLD_INSERT_LIBRARIES=[product name] to inject the code.

Type '@' to enable the input method, and type Bopomofo syllables.
Type '@' again to disable the input method.

SimpleBPMF uses "Standard" BPMF keyboard layout. This caused serious
HCI problem because Standard layout requires the use of numeric keys
(0-9). On a limited device like iPhone/iPod Touch, you'll have to
switch betweem the alphabet keyboard and numeric keyboard, and this
slows typing down.

The experiment was done during the course of October-November 2007,
but we stopped doing it because there is no point developing a system
software with unofficial means. The main point has been to demonstrate
that it is possible to come up with a quick, minimal OpenVanilla
implementation.

Since we stopped the experiment we didn't even try to catch up with
the iPhone/iPod Touch's firmware update, and we didn't catch up with
the "Toolchain" too. Apple then announced its official API. 

We won't be able to answer questions on this experiment. The source code
you see here is provided AS IS. Know your own risk when running the code
here.
