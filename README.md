cpbar
=====

Lightweight XCB-based panel for displaying textual information from STDIN using Cairo and Pango markup.

Usage
-----

	pbar [-m] [-g HEIGHT] [-l LEFT] [-r RIGHT] [-c CENTER] [-t FONT] [-f FOREGROUND] [-b BACKGROUND]

Each position of the panel, left, right and center, get `LEFT`, `RIGHT` and
`CENTER` identifiers, starting from zero.  of the panel, then the next `RIGHT`
for the right part, and the remaining `CENTER` for the center. Each part is
drawn left-to-right. Input must start with the position and the identifier and a
non-digit character, before the textual information. e.g.:

	$ pbar --center 1 --left 2 --right 3
	l0 my info
	r1 info on right
	r2 more on right
	c1 center info
	r2 updated info

Result:

	[my info][]				[center info]		[][info on right][updated info]

Example scripts [here][gist].

[gist]: http://gist.github.com/shian5/5602383

Options
-------

`-m --bottom` Place bar on the bottom of the screen.

`-g --height HEIGHT` Set bar height to `HEIGHT`. Default `16`

`-l --left LEFT` Set `LEFT` identifiers to the left part of the bar. Default `0`

`-r --right RIGHT` Set `RIGHT` identifiers to the right part of the bar. Default `0`

`-c --center CENTER` Set `CENTER` identifiers to the center part of the bar. Default `0`

`-t --font FONT` Set the default font to `FONT`. Default `Sans-Serif Bold 9`

`-f --foreground FOREGROUND` Set the default foreground color to `FOREGROUND`. Default `#FFFFFF`

`-b --background BACKGROUND` Set the default background color to `BACKGROUND`. Default `#000000`

To Do:
------

* PKGBUILD
* Improve parsing functions
* Dynamically-allocated identifiers
