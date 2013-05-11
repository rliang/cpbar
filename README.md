cpbar
=====

XCB-based panel that displays text from STDIN using Cairo and Pango markup.

Usage
-----

	pbar [-m] [-g HEIGHT] [-l LEFT] [-r RIGHT] [-c CENTER] [-t FONT] [-f FOREGROUND] [-b BACKGROUND]

Starting from `0`, the first `LEFT` identifiers are allocated for the left part
of the panel, then the next `RIGHT` for the right part, and the remaining
`CENTER` for the center. Each part is drawn left-to-right. Input must start with
the identifier and a non-digit character. e.g.:

	$ pbar --center 1 --left 2 --right 3
	0 my info
	3 info on right
	4 more on right
	5 center info
	4 updated info

Result:

	[my info][]				[center info]		[][info on right][updated info]

Options
-------

`-m --bottom` Place bar on the bottom of the screen.

`-g --height HEIGHT` Set bar height to `HEIGHT`. Default `16`

`-l --left LEFT` Set first `LEFT` identifiers to the left part of the bar. Default `0`

`-r --right RIGHT` Set the following `RIGHT` identifiers to the right part of the bar. Default `0`

`-c --center CENTER` Set the last `CENTER` identifiers to the center part of the bar. Default `0`

`-t --font FONT` Set the default font to `FONT`. Default `Sans-Serif Bold 9`

`-f --foreground FOREGROUND` Set the default foreground color to `FOREGROUND`. Default `#FFFFFF`

`-b --background BACKGROUND` Set the default background color to `BACKGROUND`. Default `#000000`

To Do:
------

* PKGBUILD
* Improve parsing functions
* Respond to X events such as EXPOSE
* Detect errors in XCB/Cairo/Pango calls
