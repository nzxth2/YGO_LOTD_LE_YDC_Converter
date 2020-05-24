# YGO LOTD Link Evolution YDC Converter
 Converter for .ydc deck files of Yu-Gi-Oh! Legacy of the Duelist Link Evolution 

## Features
 - Converts .ydc deck file to humanly readable .txt files
 - Converts said .txt files back to .ydc files
 - Allows for modification of existing decks or addition of new decks
 - Deck size check is optional, disable at your own risk
 - Capitalization and spacing in card names ignored while converting to .ydc, so you don't have to worry about that
 - If a card in a .txt file can not be identified, it will be recorded in missingCards.txt
 - Check the /extra directory in the repository for a mass-converted list of all decks
  
 ### TXT File Format
 ```
 [main]
 CardName1
 .
 .
 .
 CardName40
 [extra]
 CardName1
 .
 .
 .
 CardName15
 [side]
 CardName1
 .
 .
 .
 CardName15
 [unknown]
 0
 ```
  
 Note that comments start with `#` and are always ignored, as are empty lines  
 The headers [main], [extra] and [side] mark the start of the main, extra and side deck respectively. 
 The [unknown] header is for a field that I haven't figured out yet, seems to have no effect either. 
 Leave as it is or set it to 0. Alternatively just omit the [unknown] header and corresponding value.
 
## Building
  Not crossplatform at all, only builds for Windows  
  Included the makefile used with gcc, a simple `make` should do the trick  
  No external libraries should be required

## Credits
 Developed by nzxth2  
 [Nuklear GUI library](https://github.com/Immediate-Mode-UI/Nuklear) developed by Micha Mettke and others  
 [Notes](https://github.com/MoonlitDeath/Legacy-of-the-Duelist-notes/wiki) and [guide](https://github.com/MoonlitDeath/Link-Evolution-Editing-Guide/wiki) by MoonlitDeath

## Gallery
 ![Screenshot](https://i.imgur.com/TSchiAe.png)