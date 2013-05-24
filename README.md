cpbar
=====

Lightweight XCB-based panel for displaying textual information from STDIN using Cairo and Pango markup.

Usage
-----

	pbar [-m] [-g HEIGHT] [-t FONT] [-f FOREGROUND] [-b BACKGROUND]

This program displays three sets of textual information, on the left of the bar,
right and center. Input must start with the position - `l`, `r` or `c`; the
index, and a non-digit separator character before the textual information. Its
main feature is keeping track of those, so one doesn't need to pipe everything
on each update. e.g.:

	$ pbar
	l0 my info
	r1 info on right
	r2 more on right
	c0 center info
	r2 updated info

Result:

	[my info][]				[center info]		[][info on right][updated info]

Example scripts [here][gist].

[gist]: http://gist.github.com/shian5/5602383

Options
-------

`-m --bottom` Place bar on the bottom of the screen.

`-g --height HEIGHT` Set bar height to `HEIGHT`. Default `16`

`-t --font FONT` Set the default font to `FONT`. Default `Sans-Serif Bold 9`

`-f --foreground FOREGROUND` Set the default foreground color to `FOREGROUND`. Default `#FFFFFF`

`-b --background BACKGROUND` Set the default background color to `BACKGROUND`. Default `#000000`

To Do:
------

* PKGBUILD
* Improve parsing functions
