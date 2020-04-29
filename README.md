# bverbose
Simple software for compiling posts and pages into a fully featured blog or news site.

# what?
A CMS.

# why?
Because any CMS that you didn't make yourself is hard to configure. I wanted to make a blog with WordPress, and I realised that it's too much PHP code that I will never bother to go through. Then I tried jekyll; another story --- I don't know Ruby. Then I used Publii, soon discarded it as it did not work on Linux.

So here I am, making a readme of a CMS.

# why? #2
Because most top CMS software suites are simply bulky. They do not "compile" posts into HTML files; they render them on the fly, for every page requested. With PHP's performance on Drupal and WordPress, I got 300 miliseconds before the page even got sent to the browser. So all that great performance of nginx got lost because PHP was the great bottleneck. I don't want to say that PHP sucks, but it is just dumb to render a page to HTML _EVERY SINGLE TIME_, even though it didn't change.

So bVerbose compiles the page into HTML files, allowing nginx to show it's potential.

# should I use it?
No. bVerbose sucks. You are allowed to use it, but please don't. It is made for me and it won't work on your machine. And besides, there are already way better CMS tools you can use.

You should rather use jekyll or Publii.

The aim of this is not to be better, it is worse than any other CMS.

# folder structure:
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

