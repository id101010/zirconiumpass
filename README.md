# zirconiumpass
A simple password manager written in C++


## Build dependencies

* gcrypt


## Tips

For the mainwindow:

* Double-click on a title to edit the entry. Or just select the row and press enter. 
* Double-click on any other column to copy the value into the clipboard
* Right-click on a cell to see all options. The bold option is the one that is executed on double-click.
* Right-click on the header in the mainwindow to add/remove columns for values that you often need. This setting will be saved on your computer
* Check for shortcuts in menu


For the Entry Edit Dialog:
* Double-click a cell to edit it
* Right-click a cell to get more options (like copy to clipboard)
* The clipboard will automatically be cleared after 10 seconds, if the value is a crypted value

