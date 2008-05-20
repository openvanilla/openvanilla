iVanilla SimpleCJ: An Experimental OpenVanilla Module on
Toolchain iPhone

May 20, 2008.

SimpleCJ is an experiment. It implements a minimal "loader" of the 
OpenVanilla Framework and makes use of the Cangjie module,
a popular Traditional Chinese input method in Taiwan, and forms a
dynamically loadable bundle that you can "inject" into your running
iPhone application to achieve the effect of an input method plug-in.

The experiment was started by Yao-Chung Chang and Lukhnos D. Liu on
an 1.1.2 iPhone/iPod Touch "toolchain".

To build, use the Makefile. We don't guarantee it will build with your
own toolchain.

If you do come up with a built product, load the app in shall with 
DYLD_INSERT_LIBRARIES=[product name] to inject the code.

Also, please place cj.cin under /tmp. Actually SimpleCJ should work
as a generic input method. But only cj.cin has ever been used in the
experiment.

The load speed is terrible. It takes 5 seconds to load the entire cj.cin.
The CIN engine of OpenVanilla would require refactoring to fit in
iPhone/iPod Touch's OS and hardware constraints if it was to be
practical.

Type '@' to enable the input method, and type Canjie radicals.
Type '@' again to disable the input method.

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
