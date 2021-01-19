# this repository has been forked over to git.šijanec.eu. please adjust your remotes and pull from https://git.šijanec.eu/sijanec/bverbose. no further commits will be done here. LP

---

# bVerbose
Simple software for compiling posts and pages into a fully featured blog or news site.

# what?
A HTPCMS.

# what ??
A hypertext preprocessor content management system. You can call it BVR HTPCMS.

# why?
Because any CMS that you didn't make yourself is hard to configure. I wanted to make a blog with WordPress, and I realised that it's too much PHP code
that I will never bother to go through. Then I tried jekyll; another story --- I don't know Ruby. Then I used Publii, soon discarded it as it did not
work on Linux.

So here I am, making a readme of a CMS.

# why? #2
Because most top CMS software suites are simply bulky. They do not "compile" posts into HTML files; they render them on the fly, for every page requested.
With PHP's performance on Drupal and WordPress, I got 300 miliseconds of waiting before the page even got sent to the browser. So all that great
performance of nginx got lost because PHP was the great bottleneck. I don't want to say that PHP sucks, but it is just dumb to render a page to HTML
_EVERY SINGLE TIME_, even though it didn't change.

So bVerbose compiles the page into HTML files, allowing nginx to show it's potential.

# should I use it?
[//] # No. bVerbose sucks.
You are allowed to use it.
[//] # , but please don't. It is made for me and it won't work on your machine. And besides, there are already waybetter CMS tools you can use.

[//] # You should rather use 
Also check out jekyll or Publii.

[//]: # The aim of this is not to be better, it is worse than any other CMS.

# folder structure:
```
bVerbose
├── assets (files for compiling the blog together)
│   ├── content (user/writer-created content, bvr files and media)
│   │   ├── authors (author's descriptions and bios)
│   │   ├── global.bvr (file that is loaded on every page compilation)
│   │   ├── media (images and videos, copied directly to dist)
│   │   └── posts (pages and posts in bvr format for compiling in the blog)
│   └── layout (theme, may be possible to create multiple themes and switch seamlessly)
│       ├── css (css files that get bundled and minified)
│       ├── fonts (fonts, directly copied to dist)
│       ├── html (bvr files that make up the layout of the webpage)
│       └── js (javascript files that get *bundled together and minified*(!))
├── dist (output website, set as root folder of nginx)
├── lib (libraries and other stuff I borrowed from other people)
├── README.md (this file)
├── src (source files in C, made by me)
├── test (test scripts for individual functions in src and lib)
└── tmp (temp folder of bVerbose, used whilst compiling bvr into HTML)
```

# language syntax

<@enter command string here@>

commands are characters, such as `s` for `set`, `g` for `get` and `i` for `include` and `m` to `move`.

commands are prefixed with `?` --- question marks.

commands are followed by their arguments.

pound signs (#) as the first characters in a line represent comments. That line will be replaced with a blank line, two blank lines, or completely removed,
depending on the mood of the Off-By-One Error Gods.

including a pound sign between the opening command tag (<@) and the first question mark-prefixed command will cause that command block to surpress output.

this is useful particulary for inclusion of file to obtain variables from it in the beginning of the layout template theme to display for example author
and title before reading the body. This would require two file reads, but that doesn't really matter, as everyone has SSDs nowadays.

To undefine a variable, use the move command and rename it to BVR_UNDEFINED. The program treats BVR_UNDEFINED variables as garbage/undefined and free to
use. Note that after the movement, the old variable and it's value will be hard to reach again and possibly lost. One may be able to assign a new variable
and chances are, it will have the value of the deleted variable. Variables contain garbage values when the program gets freshly started, and then a garbage
values from previous variable values, after moves are made. If a variable is not defined/found when you call get, it will return BVR_UNDEFINED.

The BVR_UNDEFINED string is a C preprocessor definition/directive, and can be adjusted at program compile time to a different value.
# limitations

most of the control characters, such as command block opening and closing tags, comment characters, and command characters, cna be edited easily in the
bvr.h file. They are preprocessor definitions. Variables are fixed size and can be changed the same way. By default, you have 128 variables, each maximum
127 bytes in length (Size is 128 bytes including the null character).

Variable values are not quoted any can't contain control characters, but can contain spaces, depending on the command. Variable names should not contain
spaces nor control characters.

There are only strings in variables, no other datatypes, no arithmetic. I plan to add if-statements and loops. This is a HTML preprocessor, do not make it into a Turing machine, unless
you really want to.

# compilation and instalation (not yet working)

`gcc src/main.c -I lib -I src`

test script:

`gcc test/tape-test.c -I lib -I src`


