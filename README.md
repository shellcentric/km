# Kookie Monster


Some applications written for Apple macOS create cookie files but offer
no method to manage cookie privacy. Kookie Monster was written to address the
problem for myself. 

This isn't the same as cookie management; it's cookie **file** management.
For cookie management, I use SweetP Productions' [Cookie](https://cookieapp.com/).
Kookie Monster simply finds and deletes cookie **files** in such a way as to
keep some of the cookie files if desired (by maintaining a list of cookie
files). In this way, I fill the gap not yet closed by Cookie.

`km -f` finds `*.binarycookies` files and writes results to `~/.config/km/cookies`.
`km -e` deletes all files listed in `~/.config/km/cookies`.
Any lines in `~/.config/km/cookies` that begin with a hash/pound (#) are ignored.
When run from cron, cookie files I don't want don't stand a chance.

